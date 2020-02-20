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
    # adapted from https://stackoverflow.com/a/48135340
    try:
        df = pd.read_csv(filename)
        df["seed"] = int(kn.unpack(Path(filename).parts[-2])["seed"])
        df["step"] = int(kn.unpack(Path(filename).parts[-2])["step"])
        dfs.append(df)
    except pd.io.common.EmptyDataError:
        print(filename, " is empty and has been skipped.")

df = pd.concat(dfs)

df = df[df["step"] < 1050]

outfile = kn.pack({
    'title' : 'mastergenerations',
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

x = df.groupby(["step", "seed", "Level"]).mean()
y = x.reset_index().groupby(["seed", "Level"]).sum().reset_index()

for level in df["Level"].unique():

    fil = y[y["Level"] == level]

    print("level", level)
    print("Generations Elapsed mean", fil["Generations Elapsed"].mean())
    print("Generations Elapsed std", fil["Generations Elapsed"].std())
    print("Generations Elapsed min", fil["Generations Elapsed"].min())
    print("Generations Elapsed max", fil["Generations Elapsed"].max())
