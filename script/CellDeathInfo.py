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

raw = pd.read_csv(dataframe_filename)

upd_groups = mit.consecutive_groups(sorted(list(set(raw['Update']))))

proc = pd.DataFrame.from_dict([
    {
        'Update' : int(np.mean(lupds)),
        'Per-Cell-Update Death Rate' : row['Per-Cell-Update Death Rate'],
        'Cause' : row['Cause'],
        'Treatment' : row['Treatment'],
        'Observation' : obs,
        'Seed' : row['Seed'],
    }
    for upds in upd_groups
    for lupds in (list(upds),)
    for obs, upd in enumerate(lupds)
    for idx, row in raw[raw['Update'] == upd].iterrows()
])

proc['Percent Mortality'] = proc.apply(
    lambda row: np.sum(proc[
            (proc['Update'] == row['Update']) &
            (proc['Treatment'] == row['Treatment']) &
            (proc['Cause'] == row['Cause']) &
            (proc['Seed'] == row['Seed'])
        ]['Per-Cell-Update Death Rate'].values) / np.sum(proc[
            (proc['Update'] == row['Update']) &
            (proc['Treatment'] == row['Treatment']) &
            (proc['Seed'] == row['Seed'])
        ]['Per-Cell-Update Death Rate'].values),
    axis=1
)


for treat in proc['Treatment'].unique():
    print('TREATMENT', treat)
    fil_t = proc[proc['Treatment'] == treat]
    for seed in fil_t['Seed'].unique():
        print('   ', 'SEED', seed)
        fil_s = fil_t[fil_t['Seed'] == seed]

        upd = fil_s['Update'].max()
        print('   '*2, 'UPDATE', upd)
        fil_u = fil_s[fil_s['Update'] == upd]

        for cause in fil_u['Cause'].unique():
            print('   '*3, 'CAUSE', cause)
            fil_c = fil_u[fil_u['Cause'] == cause]

            print(
                '   '*4,
                'mean Percent Mortality',
                np.mean(fil_c['Percent Mortality'])
            )
            print(
                '   '*4,
                'stddev Percent Mortality',
                np.std(fil_c['Percent Mortality'])
            )
