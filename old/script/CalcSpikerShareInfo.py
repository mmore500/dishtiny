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

def CalcCountSpikerShare(filename):
    file = h5py.File(filename, 'r')
    zips = zip(*(
        zip(
            np.array(file['Live'][upd_key]).flatten(),
            np.array(
                file['ResourceContributed']['dir_4'][upd_key]
            ).flatten(),
        ) for upd_key in natsorted(
            [key for key in file['InboxTraffic']['dir_4']]
        )[-16:]
    ))
    proportion = sum([
        any(val for live, val in z)
        for z in zips
        if any(live for live, val in z)
    ])

    return proportion

def SafeCalcCountSpikerShare(filename):
    try:
        return CalcCountSpikerShare(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Mean Spiker Share Activity' : res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcCountSpikerShare)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
])

print("num files:" , len(filenames))

for treat in df['Treatment'].unique():
    fil = df.loc[(df['Treatment'] == treat)]
    print(fil['Mean Spiker Share Activity'])
    print("TREATMENT:", treat)
    print(
        "   ",
        "nreps:",
        len(fil)
    )
    print(
        "   ",
        "Mean Spiker Share Activity (mean / std):",
        np.mean(
            fil['Mean Spiker Share Activity']
        ),
        " / ",
        np.std(
            fil['Mean Spiker Share Activity']
        )
    )
    print(
        "   ",
        "Mean Spiker Share Activity (min / max):",
        np.min(
            fil['Mean Spiker Share Activity']
        ),
        " / ",
        np.max(
            fil['Mean Spiker Share Activity']
        )
    )
    print()
    print()
