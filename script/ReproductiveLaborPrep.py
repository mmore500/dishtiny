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

def CalcSameChannelRate(file):
    return np.mean([
        1 if ro[idx] != -1 else 0
        for dir_key in file['RepOutgoing']
        for ch, pc, dir, live, ro in [
                (
                np.array(
                    file['Channel']['lev_'+str(file.attrs['NLEV'][0]-1)]['upd_'+str(upd)]
                ).flatten(),
                np.array(
                    file['PrevChan']['upd_'+str(upd)]
                ).flatten(),
                np.array(
                    file['Index'][dir_key]
                ).flatten(),
                np.array(
                    file['Live']['upd_'+str(upd)]
                ).flatten(),
                np.array(
                    file['RepOutgoing'][dir_key]['upd_'+str(upd)]
                ).flatten()
                )
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx] and live[dir[idx]] and ch[idx] == ch[dir[idx]]
    ])

            #and (
            #(pc[idx] == ch[dir[idx]])
            #or ()
        #)


def CalcDiffChannelRate(file):
    return np.mean([
        1 if ro[idx] != -1 else 0
        for dir_key in file['RepOutgoing']
        for ch, pc, dir, live, ro in [
                (
                np.array(
                    file['Channel']['lev_'+str(file.attrs['NLEV'][0]-1)]['upd_'+str(upd)]
                ).flatten(),
                np.array(
                    file['PrevChan']['upd_'+str(upd)]
                ).flatten(),
                np.array(
                    file['Index'][dir_key]
                ).flatten(),
                np.array(
                    file['Live']['upd_'+str(upd)]
                ).flatten(),
                np.array(
                    file['RepOutgoing'][dir_key]['upd_'+str(upd)]
                ).flatten()
                )
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx] and live[dir[idx]] and ch[idx] != ch[dir[idx]]
    ])

            #and (
            #(pc[idx] == ch[dir[idx]])
            #or ()
        #)


print("num files:" , len(files))

outfile = (
    'script_hash=TODO+source_hash=TODO+emp_hash=TODO' +
    '+title=reproductive_labor.csv'
    )

pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Incoming Reproduction Rate' :
            CalcSameChannelRate(h5py.File(filename, 'r')),
        'Level 1 Channel ID' : 'Matching',
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for filename in tqdm(filenames)
] + [
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Incoming Reproduction Rate' :
            CalcDiffChannelRate(h5py.File(filename, 'r')),
        'Level 1 Channel ID' : 'Differing',
        'First Update' : first_update,
        'Last Update' : last_update
    }
    for filename in tqdm(filenames)
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
