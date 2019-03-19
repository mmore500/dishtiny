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

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

files = [h5py.File(filename, 'r') for filename in filenames]

def CalcSurroundedRate(file):
    return np.mean([
        sum(ro[idx] != -1 for ro in ros)
        for ch, pc, dirs, live, ros in [
                (
                np.array(
                    file['Channel']['lev_'+str(file.attrs['NLEV'][0]-1)]['upd_'+str(upd-1)]
                ).flatten(),
                np.array(
                    file['PrevChan']['upd_'+str(upd)]
                ).flatten(),
                [np.array(
                    file['Index'][dir_key]
                ).flatten() for dir_key in file['RepOutgoing']],
                np.array(
                    file['Live']['upd_'+str(upd)]
                ).flatten(),
                [np.array(
                    file['RepOutgoing'][dir_key]['upd_'+str(upd)]
                ).flatten() for dir_key in file['RepOutgoing']]
                )
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx] and all(ch[idx] == ch[dir[idx]] for dir in dirs)
    ])

def CalcNotSurroundedRate(file):
    return np.mean([
        sum(ro[idx] != -1 for ro in ros)
        for ch, pc, dirs, live, ros in [
                (
                np.array(
                    file['Channel']['lev_'+str(file.attrs['NLEV'][0]-1)]['upd_'+str(upd-1)]
                ).flatten(),
                np.array(
                    file['PrevChan']['upd_'+str(upd)]
                ).flatten(),
                [np.array(
                    file['Index'][dir_key]
                ).flatten() for dir_key in file['RepOutgoing']],
                np.array(
                    file['Live']['upd_'+str(upd)]
                ).flatten(),
                [np.array(
                    file['RepOutgoing'][dir_key]['upd_'+str(upd)]
                ).flatten() for dir_key in file['RepOutgoing']]
                )
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx] and any(ch[idx] != ch[dir[idx]] for dir in dirs)
    ])

print("num files:" , len(files))

outfile = kn.pack({
    '_source_hash' : 'TODO',
    '_emp_hash' : 'TODO',
    'title' : 'reproductive_labor_surrounded',
    'ext' : '.csv'
})

pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Reproduction Rate' :
            CalcSurroundedRate(h5py.File(filename, 'r')),
        'Channel 1 Surrounded' : 'True',
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for filename in tqdm(filenames)
] + [
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Reproduction Rate' :
            CalcNotSurroundedRate(h5py.File(filename, 'r')),
        'Channel 1 Surrounded' : 'False',
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for filename in tqdm(filenames)
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
