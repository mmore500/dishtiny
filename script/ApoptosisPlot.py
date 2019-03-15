# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'treat=resource-even__channelsense-no__nlev-two' : 'even+blind',
    'treat=resource-wave__channelsense-no__nlev-two' : 'blind',
    'treat=resource-wave__channelsense-yes__nlev-onesmall' : 'small wave',
    'treat=resource-even__channelsense-yes__nlev-two' : 'even',
    'treat=resource-wave__channelsense-yes__nlev-onebig' : 'large wave',
    'treat=resource-wave__channelsense-yes__nlev-two' : 'standard'
    }[raw]
)


print(df)

ax = sns.barplot(x="Treatment", y="Per-Cell-Update Apoptosis Rate", data=df)
plt.xticks(rotation=30)


ax.get_figure().savefig(
    'script_hash=TODO~source_hash=TODO~emp_hash=TODO~title=apoptosis.pdf',
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)
