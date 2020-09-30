# usage:
# num_files h5_filenames updates

import numpy as np
import h5py
import sys
import os
from tqdm import tqdm
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
import re
from collections import Counter, defaultdict
from joblib import delayed, Parallel
import json

num_files = int(sys.argv[1])
filenames = sys.argv[2:num_files+2]
updates = [int(v) for v in sys.argv[num_files+2:]]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def Sharing(filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    sharing = 0

    stockpile = 0

    for update in tqdm(updates):
        stocks = np.array(
            file['Stockpile']['upd_'+str(update)]
        ).flatten()
        sharings = np.array(
            file['TotalContribute']['upd_'+str(update)]
        ).flatten()

        for v in stocks:
            stockpile += v

        for v in sharings:
            sharing += v

    return sharing / stockpile

def SafeSharing(filename):
    try:
        return Sharing(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", e)
        return None


print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'sharing',
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
        'Genotype' : (
            'Wild Type' if 'id=1' in filename
            else 'Messaging Knockout' if 'id=2' in filename
            else None
        ),
        'Seed' : kn.unpack(filename)['seed'],
        'Fraction Resource Shared': res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeSharing)(filename)
            for filename in tqdm(filenames)
        )
    )
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
