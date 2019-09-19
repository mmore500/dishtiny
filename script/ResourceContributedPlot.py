# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')


dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

print("Data loaded!")

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'resource-even__channelsense-no__nlev-two__mute' : 'even+blind+mute',
    'resource-even__channelsense-no__nlev-two' : 'even+blind',
    'resource-wave__channelsense-no__nlev-two' : 'blind',
    'resource-wave__channelsense-yes__nlev-onesmall' : 'small wave',
    'resource-even__channelsense-yes__nlev-two' : 'even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'large wave',
    'resource-wave__channelsense-yes__nlev-two' : 'nested wave'
    }[raw]
)

df['Relationship Category'] = df.apply(
    lambda x: (
        'Neighbor' if 'Neighbor' in x['Relationship']
        else 'Channel' if 'mate' in x['Relationship']
        else 'Cell' if 'Cell' in x['Relationship']
        else 'Propagule' if 'Propagule' in x['Relationship']
        else 'Unknown'
    ),
    axis=1
)

print("Data crunched!")

g = sns.FacetGrid(
    df,
    col="Treatment",
    hue="Relationship Category",
    col_wrap=4,
    sharey=False
)
g.map(
    sns.barplot,
    "Relationship",
    "Shared Resource Per Cell Pair Update",
    order=[
        'Neighbor',
        'Related Neighbor',
        'Unrelated Neighbor',
        ] + sorted([
            r for r in df['Relationship'].unique() if 'Channelmate' in r
        ]) + [
        'Nonchannelmate',
        'Cell Child',
        'Cell Parent',
        'Propagule Child',
        'Propagule Parent',
    ],
)

for ax in g.axes.flat:
    for label in ax.get_xticklabels():
        label.set_rotation(-90)

outfile = kn.pack({
    'title' : 'resource_contributed',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                    file_parcel="full_parcel",
                                    files_join="cat_join"
                                ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : ".pdf"
})

plt.gcf().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)

print("Output saved to", outfile)
