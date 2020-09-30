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
    'resource-even__channelsense-no__nlev-two__mute' : 'Blind',
    'resource-even__channelsense-no__nlev-two__mute__mixed' : 'Mixed',
    'resource-even__channelsense-yes__nlev-two' : 'Even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'Flat',
    'resource-wave__channelsense-yes__nlev-two' : 'Nested'
    }[raw]
)

ax = sns.swarmplot(
    x="Treatment",
    y="Per-Cell-Update Death Rate",
    order=['Mixed','Blind','Even','Flat','Nested'],
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
    order=['Mixed','Blind','Even','Flat','Nested'],
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

plt.clf()

df['Fraction Mortality'] = df.apply(
    lambda row: np.sum(df[
            (df['Treatment'] == row['Treatment']) &
            (df['Cause'] == row['Cause']) &
            (df['Seed'] == row['Seed'])
        ]['Per-Cell-Update Death Rate'].values) / np.sum(df[
            (df['Treatment'] == row['Treatment']) &
            (df['Seed'] == row['Seed'])
        ]['Per-Cell-Update Death Rate'].values),
    axis=1
)

ax = sns.swarmplot(
 x="Treatment",
 y="Fraction Mortality",
 order=['Mixed','Blind','Even','Flat','Nested'],
 data=df[df["Cause"] == "Apoptosis"],
)
plt.xticks(rotation=-90)

outfile = kn.pack({
 'title' : 'apoptosis_pct_mortality',
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
 y="Fraction Mortality",
 order=['Mixed','Blind','Even','Flat','Nested'],
 data=df[df["Cause"] == "Apoptosis"],
)
plt.xticks(rotation=-90)

outfile = kn.pack({
 'title' : 'apoptosis_pct_mortality_barplot',
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
