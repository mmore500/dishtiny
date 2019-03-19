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


matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'resource-even__channelsense-no__nlev-two' : 'even+blind',
    'resource-wave__channelsense-no__nlev-two' : 'blind',
    'resource-wave__channelsense-yes__nlev-onesmall' : 'small wave',
    'resource-even__channelsense-yes__nlev-two' : 'even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'large wave',
    'resource-wave__channelsense-yes__nlev-two' : 'standard'
    }[raw]
)

ax = sns.barplot(
    x="Treatment",
    y="Reproduction Rate",
    hue="Channel 1 Surrounded",
    data=df
)
plt.xticks(rotation=30)


ax.get_figure().savefig(
    kn.pack({
        '_source_hash' : 'TODO',
        '_emp_hash' : 'TODO',
        'title' : 'reproductive_labor_surrounded',
        'ext' : '.pdf'
    }),
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)
