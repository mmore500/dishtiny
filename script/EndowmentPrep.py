# usage:
# first_update last_update h5_filename

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
filename = sys.argv[3]

print("TREATMENT:", kn.unpack(filename)['treat'])
print("SEED:", kn.unpack(filename)['seed'])

file = h5py.File(filename, 'r')

nlev = file.attrs['NLEV'][0]
ntile = file['Index']['own'].size

propagule_parent = [
    rc[idx]
    for dir_key in file['ResourceContributed']
    for rc, pc, ch, dir in [
        (np.array(
            file['ResourceContributed'][dir_key]['upd_'+str(upd)]
        ).flatten(),
        np.array(
            file['PrevChan']['upd_'+str(upd)]
        ).flatten(),
        np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
        ).flatten(),
        np.array(
            file['Index'][dir_key]
        ).flatten())
        for upd in range(first_update, last_update)
    ]
    for idx in range(file['Index']['own'].size)
    if (ch[dir[idx]] != ch[idx] and pc[idx] == ch[dir[idx]])
]


not_propagule_parent = [
    rc[idx]
    for dir_key in file['ResourceContributed']
    for rc, pc, ch, dir in [
        (np.array(
            file['ResourceContributed'][dir_key]['upd_'+str(upd)]
        ).flatten(),
        np.array(
            file['PrevChan']['upd_'+str(upd)]
        ).flatten(),
        np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
        ).flatten(),
        np.array(
            file['Index'][dir_key]
        ).flatten())
        for upd in range(first_update, last_update)
    ]
    for idx in range(file['Index']['own'].size)
    if (ch[dir[idx]] != ch[idx] and pc[idx] != ch[dir[idx]])
]


outfile = kn.pack({
    'treat' : kn.unpack(filename)['treat'],
    'seed' : kn.unpack(filename)['seed'],
    'title' : 'propagule_parent_resource_contributed',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                    file_parcel="full_parcel",
                                    files_join="cat_join"
                                ).hash_files([sys.argv[0]]),
    '_source_hash' : kn.unpack(filename)['_source_hash'],
    'ext' : ".csv"
})

pd.DataFrame.from_dict([
    {
        'Shared Resource Per Cell Pair Update' : v,
        'Propagule Parent' : 'true',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed']
    }
    for v in propagule_parent
] + [
    {
        'Shared Resource Per Cell Pair Update' : v,
        'Propagule Parent' : 'false',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed']
    }
    for v in not_propagule_parent
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
