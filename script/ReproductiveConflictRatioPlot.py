# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
# matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Mutation Rate'] = df['Treatment'].apply(lambda raw : {
    'wave-big__mut-a_low' : '1',
    'wave-big__mut-b_medlow' : '2',
    'wave-big__mut-c_medhigh' : '3',
    'wave-big__mut-d_high' : '4',
    'wave-big__mut-e_extreme' : '5',
    'wave-small__mut-a_low' : '1',
    'wave-small__mut-b_medlow' : '2',
    'wave-small__mut-c_medhigh' : '3',
    'wave-small__mut-d_high' : '4',
    'wave-small__mut-e_extreme' : '5',
    }[raw]
)

df['Resource Wave Size'] = df['Treatment'].apply(lambda raw : {
    'wave-big__mut-a_low' : 'big',
    'wave-big__mut-b_medlow' : 'big',
    'wave-big__mut-c_medhigh' : 'big',
    'wave-big__mut-d_high' : 'big',
    'wave-big__mut-e_extreme' : 'big',
    'wave-small__mut-a_low' : 'small',
    'wave-small__mut-b_medlow' : 'small',
    'wave-small__mut-c_medhigh' : 'small',
    'wave-small__mut-d_high' : 'small',
    'wave-small__mut-e_extreme' : 'small',
    }[raw]
)


ax = sns.lineplot(
    x="Mutation Rate",
    y="Reproductive Conflict Ratio",
    hue="Resource Wave Size",
    data=df
)
plt.xticks(rotation=30)

ax.get_figure().savefig(
    kn.pack({
        'title' : 'reproductive_conflict_ratio',
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
