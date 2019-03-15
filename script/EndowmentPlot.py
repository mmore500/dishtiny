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

def ExtractSeed(filename):
    return next(
        str for str in os.path.basename(filename).replace('.','+').split('+') if "seed=" in str
    )

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

print(df)

ax = sns.barplot(
    x="Propagule Parent",
    y="Shared Resource Per Cell Pair Update",
    data=df
)
plt.xticks(rotation=30)


ax.get_figure().savefig(
    ('script_hash=TODO_source_hash=TODO_emp_hash=TODO_'
        + ExtractTreat(dataframe_filename) + "+" +
        ExtractSeed(dataframe_filename)
        + '+title=propagule_parent_resource_contributed.pdf'
    ),
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)
