# usage:
# first_update last_update h5_filenames

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

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CalcRepRate(filename):
    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))
    return np.sum(
        np.mean([
            1 if rep != -1 else 0
            for update in range(first_update, last_update)
            for rep, live in zip(
                np.array(
                    file['RepOutgoing']['dir_'+str(dir)]['upd_'+str(update)]
                ).flatten(),
                np.array(
                    file['Live']['upd_'+str(update)]
                ).flatten()
            )
            if live
        ])
        for dir in range(4)
    )

def SafeCalcRepRate(filename):
    try:
        return CalcRepRate(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Mean Cellular Reproduction Rate' : res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcRepRate)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
])

print("num files:" , len(filenames))

for treat in df['Treatment'].unique():
    fil = df.loc[(df['Treatment'] == treat)]
    print("TREATMENT:", treat)
    print(
        "   ",
        "nreps:",
        len(fil)
    )
    print(
        "   ",
        "Cellular Reproduciton Rate (mean / std):",
        np.mean(
            fil['Mean Cellular Reproduction Rate']
        ),
        " / ",
        np.std(
            fil['Mean Cellular Reproduction Rate']
        )
    )
    print()
    print()
