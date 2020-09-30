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
import json
from natsort import natsorted

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CalcCountRegulated(filename):

    file = h5py.File(filename, 'r')

    result = 0
    for upd_key in natsorted(
        [key for key in file['Regulators']['decoder']]
    )[-4:]:

        lives = np.array(file['Live'][upd_key]).flatten()

        decoder = np.array(
            file['Regulators']['decoder'][upd_key]
        ).flatten()
        codes =  np.array(
            file['Regulators']['dir_0'][upd_key]
        ).flatten()
        archives = [
            json.loads(decoder[code].decode("utf-8"))['value0']['regulators']
            for live, code in zip (lives, codes)
            if live
        ]

        result += sum(
            sum(
                True
                for item in archive
                if item['value']['state']
            )
            for archive in archives
        )

    return result

def SafeCalcCountRegulated(filename):
    try:
        return CalcCountRegulated(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Regulation Activity' : res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcCountRegulated)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
])

print("num files:" , len(filenames))

for treat in df['Treatment'].unique():
    fil = df.loc[(df['Treatment'] == treat)]
    print(fil['Regulation Activity'])
    print("TREATMENT:", treat)
    print(
        "   ",
        "nreps:",
        len(fil)
    )
    print(
        "   ",
        "Regulation Activity (mean / std):",
        np.mean(
            fil['Regulation Activity']
        ),
        " / ",
        np.std(
            fil['Regulation Activity']
        )
    )
    print(
        "   ",
        "Regulation Activity (min / max):",
        np.min(
            fil['Regulation Activity']
        ),
        " / ",
        np.max(
            fil['Regulation Activity']
        )
    )
    print()
    print()
