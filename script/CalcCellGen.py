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
from joblib import delayed, Parallel

update = int(sys.argv[1])
filenames = sys.argv[2:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CalcMeanCellGen(filename):
    file = h5py.File(filename, 'r')
    return np.mean([
        cgen[idx]
        for cgen, live in [
            (
            np.array(
                file['CellGen']['upd_'+str(update)]
            ).flatten(),
            np.array(
                file['Live']['upd_'+str(update)]
            ).flatten()
            )
        ]
        for idx in range(file['Index']['own'].size)
        if live[idx]
    ])

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Mean Cell Generation' : cellgen,
        'Update' : update
    }
    for filename, cellgen in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(CalcMeanCellGen)(filename) for filename in tqdm(filenames)
        )
    )
])

print("num files:" , len(filenames))

for treat in df['Treatment'].unique():
    print("TREATMENT:", treat)
    print(
        "   ",
        "nreps:",
        len(df.loc[df['Treatment'] == treat])
    )
    print(
        "   ",
        "Elapsed Cell Generation (mean / std):",
        np.mean(df.loc[df['Treatment'] == treat]['Mean Cell Generation']),
        " / ",
        np.std(df.loc[df['Treatment'] == treat]['Mean Cell Generation'])
    )
    print()
