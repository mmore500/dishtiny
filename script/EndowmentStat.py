# usage:
# dataframe_filename

import sys
import os
import pandas as pd
import numpy as np
import scipy.stats as stats

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

propagule_parent = df[df['Propagule Parent'] == True
    ]['Shared Resource Per Cell Pair Update']
not_propagule_parent = df[df['Propagule Parent'] == False
    ]['Shared Resource Per Cell Pair Update']

print(len(propagule_parent))
print(len(not_propagule_parent))

print(
    "WELCH TEST: ",
    stats.ttest_ind(
        propagule_parent,
        not_propagule_parent,
        equal_var=False
    )
)
