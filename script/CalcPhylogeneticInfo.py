# usage:
# update csv_filenames

import numpy as np
import sys
from tqdm import tqdm
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import os
from keyname import keyname as kn
from fileshash import fileshash as fsh
from joblib import delayed, Parallel

upd = int(sys.argv[1])
filenames = sys.argv[2:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def LoadRelevantData(filename):
    return pd.concat([
        chunk[chunk['update'] == upd]
        for chunk in pd.read_csv(filename, iterator=True, chunksize=2048)
    ])

def SafeLoadRelevantData(filename):
    try:
        res = LoadRelevantData(filename)
        if not len(res):
            raise ValueError("update " + str(upd) + " not found")
        return res
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.concat([
    pd.concat(
        [
            res.drop("seed", axis=1).reset_index(),
            pd.DataFrame.from_records([kn.unpack(filename)]).reset_index()
        ], axis=1
    )
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeLoadRelevantData)(filename)
            for filename in tqdm(filenames)
        )
    )
    if res is not None
])

print("num files:" , len(filenames))

for treat in df['treat'].unique():
    print("TREAT:", treat)
    print(
        "   ",
        "nreps:",
        len(df.loc[df['treat'] == treat])
    )
    print(
        "   ",
        "Phylogenetic Roots mean / std:",
        np.mean(df.loc[df['treat'] == treat]['num_roots']),
        " / ",
        np.std(df.loc[df['treat'] == treat]['num_roots'])
    )
    print(
        "   ",
        "Number runs with > 1 phylogenetic root:",
        len([r for r in df.loc[df['treat'] == treat]['num_roots'] if r > 1])
    )
    print(
        "   ",
        "Number runs with > 1 phylogenetic root:",
        len([r for r in df.loc[df['treat'] == treat]['num_roots'] if r > 1])
    )
    print(
        "   ",
        "Average Phylogenetic Depth mean / std:",
        np.mean(df.loc[df['treat'] == treat]['ave_depth']),
        " / ",
        np.std(df.loc[df['treat'] == treat]['ave_depth'])
    )
    print(
        "   ",
        "Average MRCA Depth mean / std:",
        np.mean(df.loc[df['treat'] == treat]['mrca_depth']),
        " / ",
        np.std(df.loc[df['treat'] == treat]['mrca_depth'])
    )
