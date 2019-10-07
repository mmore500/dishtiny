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

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'resource-even__channelsense-no__nlev-two' : 'even+blind',
    'resource-even__channelsense-no__nlev-two__mute' : 'even+blind+mute',
    'resource-wave__channelsense-no__nlev-two' : 'blind',
    'resource-wave__channelsense-yes__nlev-onesmall' : 'small wave',
    'resource-even__channelsense-yes__nlev-two' : 'even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'large wave',
    'resource-wave__channelsense-yes__nlev-two' : 'nested wave'
    }[raw]
)

ax = sns.swarmplot(
    x="Treatment",
    y="Per-Cell-Update Death Rate",
    order=['even+blind+mute','even+blind','even','blind','large wave','small wave','nested wave'],
    data=df[df["Cause"] == "Apoptosis"],
)

ax.set(ylim=(
    0,
    df[df["Cause"] == "Apoptosis"]["Per-Cell-Update Death Rate"].max() * 1.1
))
plt.xticks(rotation=-90)

outfile = kn.pack({
    'title' : 'apoptosis',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : '.pdf'
})

ax.get_figure().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)

print('Output saved to', outfile)

plt.clf()

ax = sns.barplot(
    x="Treatment",
    y="Per-Cell-Update Death Rate",
    order=['even+blind+mute','even+blind','even','blind','large wave','small wave','nested wave'],
    data=df[df["Cause"] == "Apoptosis"],
)

plt.xticks(rotation=-90)

outfile = kn.pack({
    'title' : 'apoptosis_bar',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : '.pdf'
})

ax.get_figure().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)

print('Output saved to', outfile)
