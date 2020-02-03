# usage:
# h5_filenames

import numpy as np
import h5py
import sys
import os
from tqdm import tqdm
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
import re
from collections import Counter
from joblib import delayed, Parallel
import json
from natsort import natsorted

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def Root(filename):

    file = h5py.File(filename, 'r')
    index = np.array(file['Index']['own']).flatten()

    upd_key = max(natsorted([ k for k in file['Live'] ]))

    lives = np.array(file['Live'][upd_key]).flatten()
    rootids = np.array(file['RootID'][upd_key]).flatten()
    valid_rootids = list(
        id for live, id in zip(lives, rootids) if live
    )

    dominant = max(set(valid_rootids), key=valid_rootids.count)

    return {
        "Dominant Root" : dominant,
        "Dominant Root Proportion" : valid_rootids.count(dominant) / sum(lives),
    }

def SafeRoot(filename):
    try:
        return Root(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print(filename)
        print("   ", e)
        return None


print("num files:" , len(filenames))

assert len({kn.unpack(filename)['treat'] for filename in filenames}) == 1

outfile = kn.pack({
    'title' : 'rootids',
    'treat' : kn.unpack(filenames[0])['treat'],
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : '.csv'
})

pd.DataFrame.from_dict([
    {
        'Seed' : kn.unpack(filename)['seed'],
        'Dominant Root' : res['Dominant Root'],
        'Dominant Root Proportion' : res['Dominant Root Proportion'],
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeRoot)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
