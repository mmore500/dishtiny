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

files = [h5py.File(filename, 'r') for filename in filenames]

def CalcSameChannelRate(file):
    return np.mean([
        1 if any(rep[idx] > 0 for rep in reps) else 0
        for dir_key in file['RepCount']
        for ch, pc, dir, live, reps in [
                (np.array(
                    file['Channel']['lev_'+str(file.attrs['NLEV'][0]-1)]['upd_'+str(upd-1)]
                ).flatten(),
                np.array(
                    file['PrevChan']['upd_'+str(upd-1)]
                ).flatten(),
                np.array(
                    file['Index'][dir_key]
                ).flatten(),
                np.array(
                    file['Live']['upd_'+str(upd-1)]
                ).flatten(),
                [
                    np.array(
                        file['RepCount'][dir_key][replev_key]['upd_'+str(upd)]
                    ).flatten()
                    for replev_key in file['RepCount'][dir_key]
                ])
            for upd in range(first_update+1, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx] and live[dir[idx]] and (
            (pc[idx] == ch[dir[idx]])
            or (pc[dir[idx]] == ch[dir[idx]])
            or (ch[idx] == ch[dir[idx]])
        )
    ])

def CalcDiffChannelRate(file):
    return np.mean([
        1 if any(rep[idx] > 0 for rep in reps) else 0
        for dir_key in file['RepCount']
        for ch, pc, dir, live, reps in [
                (np.array(
                    file['Channel']['lev_'+str(file.attrs['NLEV'][0]-1)]['upd_'+str(upd-1)]
                ).flatten(),
                np.array(
                    file['PrevChan']['upd_'+str(upd-1)]
                ).flatten(),
                np.array(
                    file['Index'][dir_key]
                ).flatten(),
                np.array(
                    file['Live']['upd_'+str(upd-1)]
                ).flatten(),
                [
                    np.array(
                        file['RepCount'][dir_key][replev_key]['upd_'+str(upd)]
                    ).flatten()
                    for replev_key in file['RepCount'][dir_key]
                ])
            for upd in range(first_update+1, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx] and live[dir[idx]] and not (
            (pc[idx] == ch[dir[idx]])
            or (pc[dir[idx]] == ch[dir[idx]])
            or (ch[idx] == ch[dir[idx]])
        )
    ])


print("num files:" , len(files))

outfile = (
    'script_hash=TODO_source_hash=TODO_emp_hash=TODO' +
    '+title=reproductive_labor.csv'
    )

pd.DataFrame.from_dict([
    {
        'Treatment' : treat,
        'Incoming Reproduction Rate' : CalcSameChannelRate(file),
        'Level 1 Channel ID' : 'Matching',
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for treat, file in (
        (ExtractTreat(filename), h5py.File(filename, 'r')) for filename in tqdm(filenames)
    )
] + [
    {
        'Treatment' : treat,
        'Incoming Reproduction Rate' : CalcDiffChannelRate(file),
        'Level 1 Channel ID' : 'Differing',
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for treat, file in (
        (ExtractTreat(filename), h5py.File(filename, 'r')) for filename in tqdm(filenames)
    )
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
