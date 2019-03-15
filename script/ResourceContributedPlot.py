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

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

def ExtractTreat(filename):
    return next(
        str for str in os.path.basename(filename).split('+') if "treat=" in str
    )

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Filter'] = df['Filter'].apply(lambda raw : {
    'none' : 'None',
    'samechannel_lev_0' : 'Level 0\nChannel\nMatch',
    'samechannel_lev_1' : 'Level 1\nChannel\nMatch',
    'cell_parent' : 'Cell\nParent',
    'cell_child' : 'Cell\nChild',
    'propagule_parent' : 'Propagule\nParent',
    'propagule_child' : 'Propagule\nChild'
    }[raw]
)

df['State'] = df['Applied']

df = df[df['Filter'] != 'None']

print(df)

ax = sns.barplot(
    x="Filter",
    y="Shared Resource Per Cell Pair Update",
    hue="State",
    order=['Level 0\nChannel\nMatch', 'Level 1\nChannel\nMatch', 'Cell\nParent', 'Cell\nChild', 'Propagule\nParent', 'Propagule\nChild'],
    # hue_order=['True','False'],
    data=df
)
plt.xticks(rotation=30)


ax.get_figure().savefig(
    ('script_hash=TODO_source_hash=TODO_emp_hash=TODO+'
        + ExtractTreat(dataframe_filename)
        + '+title=resource_contributed.pdf'
    ),
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)
