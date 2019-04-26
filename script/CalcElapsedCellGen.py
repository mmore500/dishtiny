# usage:
# h5_filenames

import numpy as np
import h5py
import sys
from tqdm import tqdm
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import os
from keyname import keyname as kn
from fileshash import fileshash as fsh

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def MeanCellAge(file, start_update, end_update):
    return np.mean([
        cage[idx]
        for cage, live in [
            (
            np.array(
                file['CellAge']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Live']['upd_'+str(upd)]
            ).flatten()
            )
            for upd in range(start_update, end_update)
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx]
    ])

def ExtractSnapshotBlocks(file):
    upds = sorted([
        int(s)
        for upd_key in file['Live'] for s in upd_key.split('_') if s.isdigit()
    ])

    break_idxs = [0] + [
        idx
        for idx in range(1,len(upds))
        if upds[idx] - upds[idx-1] > 1
    ] + [len(upds)]

    return [(upds[b],upds[e-1]+1) for b,e in zip(break_idxs,break_idxs[1:])]

def CalcElapsedCellGen(file):
    snaps = ExtractSnapshotBlocks(file)
    last_snap_begin, last_snap_end = snaps[-1]

    return np.sum([
        (b2-b1)/MeanCellAge(file, b1, e1)
        for (b1,e1), (b2,__) in zip(snaps,snaps[1:])
    ]) + (
        (last_snap_end - last_snap_begin) /
        MeanCellAge(file, last_snap_begin, last_snap_end)
    )


df = pd.DataFrame.from_dict([
    {
        'Treat' : treat,
        'ElapsedCellGen' : CalcElapsedCellGen(file),
        'FinalMeanCellAge' : MeanCellAge(file, last_snap_begin, last_snap_end)
    }
    for treat, file in [
        (kn.unpack(filename)['treat'], h5py.File(filename, 'r')) for filename in filenames
    ] for last_snap_begin, last_snap_end in (ExtractSnapshotBlocks(file)[-1],)
])

print("num files:" , len(filenames))

for treat in df['Treat'].unique():
    print("TREAT:", treat)
    print(
        "   ",
        "nreps:",
        len(df.loc[df['Treat'] == treat])
    )
    print(
        "   ",
        "ElapsedCellGen mean / std:",
        np.mean(df.loc[df['Treat'] == treat]['ElapsedCellGen']),
        " / ",
        np.std(df.loc[df['Treat'] == treat]['ElapsedCellGen'])
    )
    print(
        "   ",
        "FinalMeanCellAge mean / std:",
        np.mean(df.loc[df['Treat'] == treat]['FinalMeanCellAge']),
        " / ",
        np.std(df.loc[df['Treat'] == treat]['FinalMeanCellAge'])
    )
