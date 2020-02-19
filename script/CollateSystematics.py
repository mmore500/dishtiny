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
from pathlib import Path

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

dfs = []

for filename in tqdm(filenames):
    df = pd.read_csv(filename)
    df["seed"] = kn.unpack(Path(filename).parts[-2])["seed"]
    df["step"] = kn.unpack(Path(filename).parts[-2])["step"]
    dfs.append(df)

df = pd.concat(dfs)

df = df[df["step"] <= 1049]

outfile = kn.pack({
    'title' : 'mastersystematics',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : '.csv'
})

df.to_csv(outfile, index=False)

print('Output saved to', outfile)

x = df.groupby(["step", "seed", "update"]).mean()
y = x.reset_index().groupby(["step", "seed"]).max()
z = y.reset_index().groupby(["seed"]).sum()

print("mean update", z["update"].mean())
print("std update", z["update"].std())
print("max update", z["update"].max())
print("min update", z["update"].min())

print()

print("mean ave_depth", z["ave_depth"].mean())
print("std ave_depth", z["ave_depth"].std())
print("max ave_depth", z["ave_depth"].max())
print("min ave_depth", z["ave_depth"].min())
