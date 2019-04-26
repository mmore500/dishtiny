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
sns.set_palette(sns.xkcd_palette(['medium blue', 'cherry']))

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)
df['Propagule Parent'] = df['Propagule Parent'].astype(str)

ax = sns.barplot(
    x="Propagule Parent",
    y="Shared Resource Per Cell Pair Update",
    data=df,
    hue_order=['True','False']
)
plt.xticks(rotation=30)

ax.get_figure().savefig(
    kn.pack({
        'title' : 'propagule_parent_resource_contributed',
        'treat' : kn.unpack(dataframe_filename)['treat'],
        'seed' : kn.unpack(dataframe_filename)['seed'],
        '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
        '_script_fullcat_hash' : fsh.FilesHash(
                                    file_parcel="full_parcel",
                                    files_join="cat_join"
                                ).hash_files([sys.argv[0]]),
        '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
        'ext' : '.pdf'
    }),
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)
