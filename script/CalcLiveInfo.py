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

def CalcMeanLive(filename):
    file = h5py.File(filename, 'r')
    return np.mean([
        live
        for update in range(first_update, last_update)
        for live in np.array(
                file['Live']['upd_'+str(update)]
        ).flatten()
    ])

def SafeCalcMeanLive(filename):
    try:
        return CalcMeanLive(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Mean Fraction Live' : res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcMeanLive)(filename)
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
        "Mean Fraction Live (mean / std):",
        np.mean(
            fil['Mean Fraction Live']
        ),
        " / ",
        np.std(
            fil['Mean Fraction Live']
        )
    )
    print(
        "   ",
        "Mean Fraction Live (min / max):",
        np.min(
            fil['Mean Fraction Live']
        ),
        " / ",
        np.max(
            fil['Mean Fraction Live']
        )
    )
    print()
    print()
