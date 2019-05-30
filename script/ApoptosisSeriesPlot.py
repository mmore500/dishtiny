# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from keyname import keyname as kn
from fileshash import fileshash as fsh
import numpy as np
import more_itertools as mit

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

raw = pd.read_csv(dataframe_filename)

upd_groups = mit.consecutive_groups(sorted(list(set(raw['Update']))))

proc = pd.DataFrame.from_dict([
    {
        'Update' : int(np.mean(lupds)),
        'Per-Cell-Update Apoptosis Rate' : row['Per-Cell-Update Apoptosis Rate'],
        'Type' : row['Type'],
        'Observation' : obs
    }
    for upds in upd_groups
    for lupds in (list(upds),)
    for obs, upd in enumerate(lupds)
    for idx, row in raw[raw['Update'] == upd].iterrows()
])

ax = sns.lineplot(
    x="Update",
    y="Per-Cell-Update Apoptosis Rate",
    hue="Type",
    data=proc
)

ax.set_ylim(
    ymin=0.0,
    ymax=0.0003
)

outfile = kn.pack({
    'title' : 'apoptosis_series',
    'treat' : kn.unpack(dataframe_filename)['treat'],
    'seed' : kn.unpack(dataframe_filename)['seed'],
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : '.pdf'
})

ax.get_figure().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)

print('Output saved to', outfile)
