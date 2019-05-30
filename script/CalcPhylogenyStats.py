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

upd = int(sys.argv[1])
filenames = sys.argv[2:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

res = []
for filename in tqdm(filenames):
    df = pd.read_csv(filename)
    df['treat'] = kn.unpack(filename)['treat']
    res.append(df)

df = pd.concat(res)

df = df[df['update'] == upd]

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
