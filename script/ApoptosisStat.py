# usage:
# dataframe_filename

import numpy as np
import sys
import os
from tqdm import tqdm
import pandas as pd
from astropy.stats import bootstrap
from scipy import stats

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

nlev_two = df[
        df['Treatment'] == 'resource-wave__channelsense-yes__nlev-two'
    ]['Per-Cell-Update Apoptosis Rate']

nlev_onesmall = df[
        df['Treatment'] == 'resource-wave__channelsense-yes__nlev-onesmall'
    ]['Per-Cell-Update Apoptosis Rate']

nlev_onebig = df[
        df['Treatment'] == 'resource-wave__channelsense-yes__nlev-onebig'
    ]['Per-Cell-Update Apoptosis Rate']

control = df[
        df['Treatment'] == 'resource-even__channelsense-no__nlev-two__mute'
    ]['Per-Cell-Update Apoptosis Rate']


print("nlev_two / control:")
bootstats = [
    np.mean(nlev_two_boot) - np.mean(control)
    for nlev_two_boot, control_boot
    in tqdm(zip(
            bootstrap(np.array(nlev_two), 100000),
            bootstrap(np.array(control), 100000)
        ))
    ]
print(
    "   ",
    stats.percentileofscore(bootstats, 0, 'rank')
    )


print("nlev_onesmall_boot / control:")
bootstats = [
    np.mean(nlev_onesmall_boot) - np.mean(control)
    for nlev_onesmall_boot, control_boot
    in tqdm(zip(
            bootstrap(np.array(nlev_onesmall), 100000),
            bootstrap(np.array(control), 100000)
        ))
    ]
print(
   "   ",
   stats.percentileofscore(bootstats, 0, 'rank')
   )

print("nlev_onebig_boot / control:")
bootstats = [
    np.mean(nlev_onebig_boot) - np.mean(control)
    for nlev_onebig_boot, control_boot
    in tqdm(zip(
            bootstrap(np.array(nlev_onebig), 100000),
            bootstrap(np.array(control), 100000)
        ))
    ]
print(
   "   ",
   stats.percentileofscore(bootstats, 0, 'rank')
   )
