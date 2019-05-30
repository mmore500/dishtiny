# usage:
# h5_filename

import numpy as np
import h5py
import sys
import os
from tqdm import tqdm
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
import re

filename = sys.argv[1]
file = h5py.File(filename, 'r')

def FracApoptosis(file, upd):
    return np.mean([
        1 if apop[idx] else 0 # 1 and 2 are apoptosis types
        for apop in [
            np.array(
                file['Apoptosis']['upd_'+str(upd)]
            ).flatten()
        ]
        for idx in range(file['Index']['own'].size)
    ])

def ExtractUpdates(file):
    return [int(re.findall('\d+', key)[0]) for key in file['Apoptosis'].keys()]

outfile = kn.pack({
    'title' : 'apoptosis_series',
    'seed' : kn.unpack(filename)['seed'],
    'treat' : kn.unpack(filename)['treat'],
    '_data_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filename)['_source_hash'],
    'ext' : '.csv'
})

pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Per-Cell-Update Apoptosis Rate' : FracApoptosis(file, upd),
        'Update' : upd,
    }
    for upd in ExtractUpdates(file)
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
