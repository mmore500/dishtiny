# usage:
# dataframe_filename

import numpy as np
import sys
import os
from tqdm import tqdm
import pandas as pd
from astropy.stats import bootstrap

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

print(df)

std = df[df['Treatment'] == 'treat=resource-wave__channelsense-yes__nlev-two'
    ]['Incoming Reproduction Rate']
control = df[df['Treatment'] == 'treat=resource-wave__channelsense-yes__nlev-onesmall'
    ]['Incoming Reproduction Rate']

boots = zip(bootstrap(np.array(std), 100000), bootstrap(np.array(control), 100000))

bootstats = [np.mean(std_boot) - np.mean(control_boot)
    for std_boot, control_boot in tqdm(boots)]

print("std/control")
print("99.99% {}".format(np.percentile(bootstats,99.99)))
print("99.9% {}".format(np.percentile(bootstats,99.9)))
print("99% {}".format(np.percentile(bootstats,99)))
print("95% {}".format(np.percentile(bootstats,95)))
print("5% {}".format(np.percentile(bootstats,5)))
print("1% {}".format(np.percentile(bootstats,1)))
print("0.1% {}".format(np.percentile(bootstats,0.1)))
print("0.01% {}".format(np.percentile(bootstats,0.01)))
