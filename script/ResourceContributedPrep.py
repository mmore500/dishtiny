# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
import scipy.stats as stats
from tqdm import tqdm
import os
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
from collections import defaultdict
from joblib import delayed, Parallel

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

nlev = h5py.File(filenames[0], 'r').attrs['NLEV'][0]
ntile = h5py.File(filenames[0], 'r')['Index']['own'].size

def CalcContrib(filename):
    file = h5py.File(filename, 'r')
    cumsum = defaultdict(float) # cumulative sum of observed values
    obscnt = defaultdict(int) # how many observations have been made

    nlev = int(file.attrs.get('NLEV'))
    ncell = file['Index']['own'].size

    cage_group = file['CellAge']
    live_group = file['Live']
    ppos_group = file['ParentPos']

    chan_groups = [
        file['Channel']['lev_' + str(idx)]
        for idx in range(nlev)
    ]
    pvch_group = file['PrevChan'] #TODO levels

    for dir_key in file['ResourceContributed']:

        drct = np.array(file['Index'][dir_key]).flatten()

        resc_group = file['ResourceContributed'][dir_key]

        for upd in range(first_update, last_update):

            cage = np.array(cage_group['upd_'+str(upd)]).flatten()
            live = np.array(live_group['upd_'+str(upd)]).flatten()
            ppos = np.array(ppos_group['upd_'+str(upd)]).flatten()

            pvch = np.array(ppos_group['upd_'+str(upd)]).flatten()

            resc = np.array(resc_group['upd_'+str(upd)]).flatten()

            chans = [
                np.array(g['upd_'+str(upd)]).flatten()
                for g in chan_groups
            ]


            for idx in range(ncell):

                # if either of the cells are not alive, there is no sharing
                if not live[idx] or not live[drct[idx]]:
                    continue

                related = False

                cumsum['Neighbor'] += resc[idx]
                obscnt['Neighbor'] += 1

                for lev in range(nlev):
                    # remember, these are mutually exclusive
                    if chans[lev][idx] == chans[lev][drct[idx]]:
                        cumsum['Channelmate ' + str(lev)] += resc[idx]
                        obscnt['Channelmate ' + str(lev)] += 1
                        related = True
                        break
                else:
                    # runs if no break is called
                    cumsum['Nonchannelmate'] += resc[idx]
                    obscnt['Nonchannelmate'] += 1

                if ppos[idx] == drct[idx] and cage[idx] < cage[drct[idx]]:
                    cumsum['Cell Parent'] += resc[idx]
                    obscnt['Cell Parent'] += 1
                    related = True
                elif ppos[drct[idx]] == idx and cage[idx] < cage[drct[idx]]:
                    cumsum['Cell Child'] += resc[idx]
                    obscnt['Cell Child'] += 1
                    related = True
                else:
                    cumsum['Nondirect Cell Relative'] += resc[idx]
                    obscnt['Nondirect Cell Relative'] += 1

                if pvch[idx] == chans[-1][drct[idx]]:
                    cumsum['Propagule Parent'] += resc[idx]
                    obscnt['Propagule Parent'] += 1
                    related = True
                elif pvch[drct[idx]] == chans[-1][idx]:
                    cumsum['Propagule Child'] += resc[idx]
                    obscnt['Propagule Child'] += 1
                    related = True
                else:
                    cumsum['Nondirect Propagule Relative'] += resc[idx]
                    obscnt['Nondirect Propagule Relative'] += 1

                if related:
                    cumsum['Related Neighbor'] += resc[idx]
                    obscnt['Related Neighbor'] += 1
                else:
                    cumsum['Unrelated Neighbor'] += resc[idx]
                    obscnt['Unrelated Neighbor'] += 1

    assert(set(cumsum.keys()) == set(obscnt.keys()))

    return {
        key : cumsum[key] / obscnt[key]
        for key in cumsum.keys()
    }

print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'resource_contributed',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                    file_parcel="full_parcel",
                                    files_join="cat_join"
                                ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : ".csv"
})

pd.DataFrame.from_dict([
    {
        'Shared Resource Per Cell Pair Update' : value,
        'Relationship' : relationship,
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed']
    }
    for res, filename in zip(
        Parallel(n_jobs=-1)(
            delayed(CalcContrib)(filename) for filename in tqdm(filenames)
        ), filenames
    )
    for relationship, value in res.items()
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
