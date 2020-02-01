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
from joblib import delayed, Parallel
from natsort import natsorted

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CalcCountShare(filename):
    file = h5py.File(filename, 'r')
    zips = zip(*(
        zip(
            np.array(file['Live'][upd_key]).flatten(),
            (sum(z) for z in zip(*(
                np.array(
                    file['ResourceContributed']['dir_'+str(dir)][upd_key]
                    ).flatten()
                for dir in range(5)
            ))),
        ) for upd_key in natsorted([key for key in file['Live']])[-16:]
    ))
    proportion = sum([
        any(val for live, val in z)
        for z in zips
        if any(live for live, val in z)
    ])

    return proportion

def SafeCalcCountShare(filename):
    try:
        return CalcCountShare(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Mean Share Activity' : res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcCountShare)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
])

print("num files:" , len(filenames))

for treat in df['Treatment'].unique():
    fil = df.loc[(df['Treatment'] == treat)]
    print(fil['Mean Share Activity'])
    print("TREATMENT:", treat)
    print(
        "   ",
        "nreps:",
        len(fil)
    )
    print(
        "   ",
        "Mean Share Activity (mean / std):",
        np.mean(
            fil['Mean Share Activity']
        ),
        " / ",
        np.std(
            fil['Mean Share Activity']
        )
    )
    print(
        "   ",
        "Mean Share Activity (min / max):",
        np.min(
            fil['Mean Share Activity']
        ),
        " / ",
        np.max(
            fil['Mean Share Activity']
        )
    )
    print()
    print()
