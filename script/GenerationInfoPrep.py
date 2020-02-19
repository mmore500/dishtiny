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
    nlev = int(file.attrs.get('NLEV'))
    return [
        np.mean([
            cgen
            for cgen, live in zip(
                np.array(
                    file['CellGen']['lev_'+str(lev)]['upd_'+str(update)]
                ).flatten(),
                np.array(
                    file['Live']['upd_'+str(update)]
                ).flatten()
            )
            if live
        ])
        for lev in range(nlev+1)
    ]

def SafeCalcMeanCellGen(filename):
    try:
        return CalcMeanCellGen(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Mean Cell Generation' : res,
        'Level' : lev,
        'Update' : update
    }
    for filename, cellgen in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcMeanCellGen)(filename)
            for filename in tqdm(filenames)
        )
    )
    if cellgen is not None
    for lev, res in enumerate(cellgen)
])

print("num files:" , len(filenames))

for lev in df['Level'].unique():
    print("="*20)
    print("NLEV", lev)
    print()
    for treat in df['Treatment'].unique():
        fil = df.loc[(df['Treatment'] == treat) & (df['Level'] == lev)]
        print("TREATMENT:", treat)
        print(
            "   ",
            "nreps:",
            len(fil)
        )
        print(
            "   ",
            "Elapsed Cell Generation (mean / std):",
            np.mean(
                fil['Mean Cell Generation']
            ),
            " / ",
            np.std(
                fil['Mean Cell Generation']
            )
        )
        print()
        print()
