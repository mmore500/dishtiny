# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
import os
from tqdm import tqdm
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def FracApoptosis(file):
    return np.mean([
        1 if apop[idx] else 0 # 1 and 2 are apoptosis types
        for apop in [
            np.array(
                file['Apoptosis']['upd_'+str(upd)]
            ).flatten()
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
    ])

print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'apoptosis',
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
        'Per-Cell-Update Apoptosis Rate'
            : FracApoptosis(h5py.File(filename, 'r')),
        'First Update' : first_update,
        'Last Update' : last_update,
        'seed' : kn.unpack(filename)['seed']
    }
    for filename in tqdm(filenames)
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
