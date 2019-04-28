# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
import scipy.stats as stats
from tqdm import tqdm
import os
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

nlev = h5py.File(filenames[0], 'r').attrs['NLEV'][0]
ntile = h5py.File(filenames[0], 'r')['Index']['own'].size

def CalcSameChannelContrib(filename):
    file = h5py.File(filename, 'r')
    res = [
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel']['lev_0']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[idx] == ch[dir[idx]])
    ]
    return np.mean(res) if len(res) else 0

def CalcDiffChannelContrib(filename):
    file = h5py.File(filename, 'r')
    res = [
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel'][lev_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[idx] != ch[dir[idx]])
    ]
    return np.mean(res) if len(res) else 0

print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'resource_contributed',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                    file_parcel="full_parcel",
                                    files_join="cat_join"
                                ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : ".csv"
})

pd.DataFrame.from_dict([
    {
        'Shared Resource Per Cell Pair Update'
            : CalcSameChannelContrib(filename),
        'Channel Match' : 'true',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed']
    }
    for filename in tqdm(filenames)
] + [
    {
        'Shared Resource Per Cell Pair Update'
            : CalcDiffChannelContrib(filename),
        'Channel Match' : 'false',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed']
    }
    for filename in tqdm(filenames)
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
