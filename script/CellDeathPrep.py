# usage:
# h5_filenames

import numpy as np
import h5py
import sys
import os
from tqdm import tqdm
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
import re
from collections import Counter

KILL_THRESH = -5.0

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CauseOfDeath(file, upd):

    deaths = [None for idx in range(file['Index']['own'].size)]

    # replaced
    for dir_key in file['RepOutgoing']:
        rog = np.array(file['RepOutgoing'][dir_key]['upd_'+str(upd)]).flatten()
        facing = np.array(file['Index'][dir_key]).flatten()

        for idx in range(file['Index']['own'].size):
            if rog[idx] != -1:
                deaths[facing[idx]] = 'Replaced'

    # bankrupt
    stock = np.array(file['Stockpile']['upd_'+str(upd)]).flatten()

    for idx in range(file['Index']['own'].size):
        if stock[idx] < -5.0:
            deaths[idx] = 'Bankrupt'

    # apoptosis
    apop = np.array(file['Apoptosis']['upd_'+str(upd)]).flatten()

    for idx in range(file['Index']['own'].size):
        if apop[idx] == 1:
            deaths[idx] = 'Partial Apoptosis'
        elif apop[idx] == 2:
            deaths[idx] = 'Complete Apoptosis'

    # already dead cells can't die again
    live = np.array(file['Live']['upd_'+str(upd)]).flatten()

    for idx in range(file['Index']['own'].size):
        if not live[idx]:
            deaths[idx] = None

    res = Counter(deaths)

    for cause in (
        'Replaced',
        'Bankrupt',
        'Partial Apoptosis',
        'Complete Apoptosis',
        None
    ):
        if cause not in res:
            res[cause] = 0

    res.pop(None)

    return res

def ExtractUpdates(file):
    return [int(re.findall('\d+', key)[0]) for key in file['Apoptosis'].keys()]

print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'cell_death',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : '.csv'
})

pd.DataFrame.from_dict([
    {
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed'],
        'Per-Cell-Update Death Rate' : val / file['Index']['own'].size,
        'Cause' : cause,
    'Update' : upd
    }
    for filename in tqdm(filenames)
    for file in (h5py.File(filename, 'r'),)
    for upd in ExtractUpdates(file)
    for cause, val in CauseOfDeath(file, upd).items()
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
