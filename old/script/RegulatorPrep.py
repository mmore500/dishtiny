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

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def Regulator(filename):

    file = h5py.File(filename, 'r')
    index = np.array(file['Index']['own']).flatten()

    sett = []
    unique = []
    for upd in range(first_update, last_update, 4):
        live = np.array(file['Live']['upd_'+str(upd)]).flatten()
        decoder = np.array(
            file['Regulators']['decoder']['upd_'+str(upd)]
        ).flatten()

        for dir in range(4):
            keys = np.array(
                file['Regulators']['dir_'+str(dir)]['upd_'+str(upd)]
            ).flatten()

            for idx in index:
                if live[idx]:
                    archive = json.loads(
                        decoder[keys[idx]].decode("utf-8")
                    )['value0']
                    sett.append(sum(
                        1
                        for k, v in archive['regulators']
                        if v != 1.0
                    ))
                    unique.append(len(archive['regulators']))


    return np.mean(sett), np.mean(unique)

def SafeRegulator(filename):
    try:
        return Regulator(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", e)
        return None


print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'regulator',
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
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed'],
        'Mean Set Regulators' : res[0],
        'Mean Unique Regulators' : res[1],
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeRegulator)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
