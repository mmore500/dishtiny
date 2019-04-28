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

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'wave-big__mut-a_low' : 'big+low',
    'wave-big__mut-b_medlow' : 'big+medlow',
    'wave-big__mut-c_medhigh' : 'big+medhigh',
    'wave-big__mut-d_high' : 'big+high',
    'wave-big__mut-e_extreme' : 'big+extreme',
    'wave-small__mut-a_low' : 'small+low',
    'wave-small__mut-b_medlow' : 'small+medlow',
    'wave-small__mut-c_medhigh' : 'small+medhigh',
    'wave-small__mut-d_high' : 'small+high',
    'wave-small__mut-e_extreme' : 'small+extreme',
    }[raw]
)

ax = sns.barplot(
    x="Treatment",
    y="Incoming Reproduction Rate",
    hue="Level 0 Channel ID",
    data=df
)
plt.xticks(rotation=30)


ax.get_figure().savefig(
    kn.pack({
        'title' : 'reproductive_labor',
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
