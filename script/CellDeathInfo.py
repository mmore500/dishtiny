# usage:
# dataframe_filename

import sys
import os
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
import numpy as np
import more_itertools as mit

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Percent Mortality'] = df.apply(
    lambda row: np.sum(df[
            (df['Treatment'] == row['Treatment']) &
            (df['Cause'] == row['Cause']) &
            (df['Seed'] == row['Seed'])
        ]['Per-Cell-Update Death Rate'].values) / np.sum(df[
            (df['Treatment'] == row['Treatment']) &
            (df['Seed'] == row['Seed'])
        ]['Per-Cell-Update Death Rate'].values),
    axis=1
)


for name, group in df.groupby(["Treatment", "Cause"]):
    print(name)
    print(
        '   '*4,
        'mean Percent Mortality',
        np.mean(group['Percent Mortality'])
    )
    print(
        '   '*4,
        'stddev Percent Mortality',
        np.std(group['Percent Mortality'])
    )
