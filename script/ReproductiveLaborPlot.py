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

# Setup dataframe

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

df['Channel Relationship'] = df['Level 0 Channel ID']

# Setup left subplot
ax1 = plt.subplot(121)
sns.barplot(
    y="Mutational Load",
    x="Incoming Reproduction Rate",
    hue="Channel Relationship",
    hue_order=["Matching", "Differing"],
    data=df[df["Wave Size"] == 'small']
).get_legend().remove()
ax1.invert_xaxis()
ax1.invert_yaxis()
ax1.set_title('Small Resource Wave')
ax1.xaxis.label.set_visible(False)

# Setup right subplot
ax2 = plt.subplot(122)
sns.barplot(
    y="Mutational Load",
    x="Incoming Reproduction Rate",
    hue="Channel Relationship",
    hue_order=["Matching", "Differing"],
    data=df[df["Wave Size"] == 'big']
)
ax2.invert_yaxis()
ax2.set_title('Large Resource Wave')
ax2.xaxis.label.set_visible(False)
ax2.set_yticklabels([])
ax2.yaxis.label.set_visible(False)
handles, labels = ax2.get_legend_handles_labels()
legend = ax2.legend(
    handles[::-1],
    labels[::-1],
    title='Channel Relationship',
    loc='upper right',
    fancybox=False,
    shadow=False
)
legend.get_frame().set_alpha(1.0)
plt.setp(legend.get_title(),fontsize='small')

# Tightly join y axes
ax2.get_shared_y_axes().join(ax2, ax2)
plt.subplots_adjust(wspace=.0)

# Set one x axis label
plt.gcf().text(0.5, 0.0, 'Incoming Reproduction Rate', ha='center')

# Ensure identical scaling
ax1.set_xlim(0.002, 0)
ax2.set_xlim(0, 0.002)

# Save figure
plt.gcf().savefig(
    kn.pack({
        'title' : 'reproductive_labor',
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
