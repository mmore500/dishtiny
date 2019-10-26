# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from keyname import keyname as kn
from fileshash import fileshash as fsh
import numpy as np
import scipy as sp
from astropy.stats import bootstrap
from tqdm import tqdm

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

# replicability info
print({
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
})
for key in "Maximum Quadrant Resource-Sharing Fraction",:
    print(
        key,
        "Wild Type mean / std:",
        np.mean(
            df[
                df["Genotype"] == "Wild Type"
            ][key]
        ),
        np.std(
            df[
                df["Genotype"] == "Wild Type"
            ][key]
        ),
    )
    print(
        key,
        "Messaging Knockout mean:",
        np.mean(
            df[
                df["Genotype"] == "Messaging Knockout"
            ][key]
        ),
        np.std(
            df[
                df["Genotype"] == "Messaging Knockout"
            ][key]
        ),
    )

    # do bootstrap statistics
    bootsamples = 1000000

    boots = zip(
        bootstrap(
            np.array(
                df[
                    df["Genotype"] == "Wild Type"
                ][key]
            ),
            bootsamples
        ),
        bootstrap(
            np.array(
                df[
                    df["Genotype"] == "Messaging Knockout"
                ][key]
            ),
            bootsamples
        ),
    )

    bootstats = [
        np.mean(std_boot) - np.mean(control_boot)
        for std_boot, control_boot in tqdm(boots, total=bootsamples)
    ]

    print(
        key,
        "p:",
        # divide by 100 b/c percentileofscore returns 0-100 and we want 0-1
        sp.stats.percentileofscore(bootstats, 0, 'rank')/100,
    )

# draw graphs
sns.barplot(
    data=df,
    x="Genotype",
    y="Maximum Quadrant Resource-Sharing Fraction",
)

outfile = kn.pack({
    'title' : 'sharingquadrant',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : '.pdf',
})

plt.gcf().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0,
)

print('Output saved to', outfile)

plt.clf()
