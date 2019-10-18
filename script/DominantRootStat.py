# usage:
# dataframe_filename

import numpy as np
import sys
import os
from tqdm import tqdm
import pandas as pd
from scipy import stats

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

print(
    "num trials coalsesced",
    len(df[df['Dominant Root Proportion'] == 1])
)

print(
    "mean dominant proportion",
    np.mean(df['Dominant Root Proportion'])
)

print("num successes")
print(len(df[df['Dominant Root'] == 1]))


print("exact test p-value")
print(stats.binom_test(
    sum(df['Dominant Root'] == 1),
    n=len(df),
    p=0.5,
    alternative='two-sided',
))
