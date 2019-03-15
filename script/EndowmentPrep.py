# usage:
# first_update last_update h5_filename

import numpy as np
import h5py
import sys
import scipy.stats as stats
from tqdm import tqdm
import os
import pandas as pd

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filename = sys.argv[3]

def ExtractTreat(filename):
    return next(
        str for str in os.path.basename(filename).replace('.','+').split('+') if "treat=" in str
    )

def ExtractSeed(filename):
    return next(
        str for str in os.path.basename(filename).replace('.','+').split('+') if "seed=" in str
    )

print("TREATMENT:", ExtractTreat(filename))
print("SEED:", ExtractSeed(filename))

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


outfile = (
    'script_hash=TODO_source_hash=TODO_emp_hash=TODO_' +
    ExtractTreat(filename) + "+" +
    ExtractSeed(filename) +
    '+title=resource_contributed_propagule_parent.csv'
    )

pd.DataFrame.from_dict([
    {
        'Shared Resource Per Cell Pair Update' : v,
        'Propagule Parent' : 'true',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : ExtractTreat(filename),
        'Seed' : ExtractSeed(filename)
    }
    for v in propagule_parent
    for idx, v in enumerate(tup)
] + [
    {
        'Shared Resource Per Cell Pair Update' : v,
        'Propagule Parent' : 'false',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : ExtractTreat(filename),
        'Seed' : ExtractSeed(filename)
    }
    for v in not_propagule_parent
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
