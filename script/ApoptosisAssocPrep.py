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
import more_itertools as mit

filename = sys.argv[1]
file = h5py.File(filename, 'r')

def PropApoptosisDebtor(file, upds):
    res = [
        1 if apop[idx] else 0
        for upd in upds
        for apop, stock in [(
            np.array(
                file['Apoptosis']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Stockpile']['upd_'+str(upd)]
            ).flatten()
        )]
        for idx in range(file['Index']['own'].size)
        if stock[idx] < 0
    ]
    return np.mean(res) if res else 0

def PropApoptosisNondebtor(file, upds):
    res = [
        1 if apop[idx] else 0
        for upd in upds
        for apop, stock, live in [(
            np.array(
                file['Apoptosis']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Stockpile']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Live']['upd_'+str(upd)]
            ).flatten()
        )]
        for idx in range(file['Index']['own'].size)
        if stock[idx] >= 0 and live[idx]
    ]
    return np.mean(res) if res else 0

def ExtractUpdateGroups(file):
    upds = [int(re.findall('\d+', key)[0]) for key in file['Apoptosis'].keys()]
    return [
        [upd for upd in group]
        for group in mit.consecutive_groups(sorted(upds))
    ]


outfile = kn.pack({
    'title' : 'apoptosis_assoc',
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
        'Per-Cell-Update Apoptosis Rate' : val,
        'Type' : type,
        'Update' : np.mean(upds),
    }
    for upds in ExtractUpdateGroups(file)
    for type, val in [
        ('Debtor', PropApoptosisDebtor(file, upds)),
        ('Nondebtor', PropApoptosisNondebtor(file, upds))
    ]
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
