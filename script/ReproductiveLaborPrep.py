# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
import scipy.stats as stats
from tqdm import tqdm
import os
import pandas as pd

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

def ExtractTreat(filename):
    return next(
        str for str in os.path.basename(filename).split('+') if "treat=" in str
    )

def ExtractSeed(filename):
    return next(
        str for str in os.path.basename(filename).split('+') if "seed=" in str
    )

nlev = h5py.File(filenames[0], 'r').attrs['NLEV'][0]
ntile = h5py.File(filenames[0], 'r')['Index']['own'].size

files = [h5py.File(filename, 'r') for filename in filenames]

def CalcContainedRate(file):
    return np.mean([
        np.sum(rep for rep in reps)
        for ch, dirs, reps in [
                (np.array(
                    file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
                ).flatten(),
                [
                    np.array(file['Index'][dir_key]).flatten()
                    for dir_key in file['RepCount']
                ],
                [
                    np.array(
                        file['RepCount'][dir_key_inner][replev_key_inner]['upd_'+str(upd)]
                    ).flatten()
                    for dir_key_inner in file['RepCount']
                    for replev_key_inner in file['RepCount'][dir_key_inner]
                ])
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if all(ch[idx] == ch[dir[idx]] for dir in dirs)
    ])

def CalcFreeRate(file):
    return np.mean([
        np.sum(rep for rep in reps)
        for ch, dirs, reps in [
                (np.array(
                    file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
                ).flatten(),
                [
                    np.array(file['Index'][dir_key]).flatten()
                    for dir_key in file['RepCount']
                ],
                [
                    np.array(
                        file['RepCount'][dir_key_inner][replev_key_inner]['upd_'+str(upd)]
                    ).flatten()
                    for dir_key_inner in file['RepCount']
                    for replev_key_inner in file['RepCount'][dir_key_inner]
                ])
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if not all(ch[idx] == ch[dir[idx]] for dir in dirs)
    ])


print("num files:" , len(files))

outfile = (
    'script_hash=TODO_source_hash=TODO_emp_hash=TODO' +
    '+title=reproductive_labor.csv'
    )

pd.DataFrame.from_dict([
    {
        'Treatment' : treat,
        'Reproduction Rate Contained' : CalcContainedRate(file),
        'Reproduction Rate Free' : CalcFreeRate(file),
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for treat, file in (
        (ExtractTreat(filename), h5py.File(filename, 'r')) for filename in tqdm(filenames)
    )
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
