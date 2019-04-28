# usage:
# update h5_filenames

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

upd = int(sys.argv[1])
filenames = sys.argv[2:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CalcMeanCellGen(file):
    return np.mean([
        cgen[idx]
        for cgen, live in [
            (
            np.array(
                file['CellGen']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Live']['upd_'+str(upd)]
            ).flatten()
            )
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx]
    ])

df = pd.DataFrame.from_dict([
    {
        'Treat' : treat,
        'CellGen' : CalcMeanCellGen(file),
        'Upd' : upd
    }
    for treat, file in [
        (kn.unpack(filename)['treat'], h5py.File(filename, 'r')) for filename in filenames
    ]
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
        np.mean(df.loc[df['Treat'] == treat]['CellGen']),
        " / ",
        np.std(df.loc[df['Treat'] == treat]['CellGen'])
    )
