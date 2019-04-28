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

df['Wave Size'] = df['Treatment'].apply(lambda raw : {
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

df['Mutational Load'] = df['Treatment'].apply(lambda raw : {
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
).astype('category')

df['Channel Match'] = df['Channel Match'].astype(str)

ax1 = plt.subplot(121)
ax1.invert_xaxis()

sns.barplot(
    y="Mutational Load",
    x="Shared Resource Per Cell Pair Update",
    hue="Channel Match",
    hue_order=["True", "False"],
    data=df[df["Wave Size"] == 'small']
)

ax1.set_title('Small Wave')
ax1.xaxis.label.set_visible(False)

ax2 = plt.subplot(122)

sns.barplot(
    y="Mutational Load",
    x="Shared Resource Per Cell Pair Update",
    hue="Channel Match",
    hue_order=["True", "False"],
    data=df[df["Wave Size"] == 'big']
).get_legend().remove()

ax2.set_title('Big Wave')

ax2.set_yticklabels([])
ax2.yaxis.label.set_visible(False)
ax2.xaxis.label.set_visible(False)

ax2.get_shared_y_axes().join(ax2, ax2)
plt.subplots_adjust(wspace=.0)
plt.gcf().text(0.5, 0.0, 'Shared Resource Per Cell Pair Update', ha='center')

bigger = max((ax1, ax2), key=lambda x: max(x.get_xlim()))
print(bigger)
ax1.set_xlim(0.2, 0)
ax2.set_xlim(0, 0.2)

plt.gcf().savefig(
    kn.pack({
        'title' : 'resource_contributed',
        '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
        '_script_fullcat_hash' : fsh.FilesHash(
                                        file_parcel="full_parcel",
                                        files_join="cat_join"
                                    ).hash_files([sys.argv[0]]),
        '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
        'ext' : ".pdf"
    }),
    transparent=True,
    bbox_inches='tight',
    pad_inches=0.1
)
