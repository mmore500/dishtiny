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

def CalcRepr(filename):
    file = h5py.File(filename, 'r')
    # relationship type -> metric -> value
    res = defaultdict(lambda: defaultdict(float))

    nlev = int(file.attrs.get('NLEV'))
    ncell = file['Index']['own'].size

    cage_group = file['CellAge']
    live_group = file['Live']
    ppos_group = file['ParentPos']
    pvch_group = file['PrevChan']
    stck_group = file['Stockpile']

    chan_groups = [
        file['Channel']['lev_' + str(idx)]
        for idx in range(nlev)
    ]

    for dir_key in file['ResourceContributed']:

        drct = np.array(file['Index'][dir_key]).flatten()

        repo_group = file['RepOutgoing'][dir_key]

        for upd in range(first_update, last_update):

            cage = np.array(cage_group['upd_'+str(upd)]).flatten()
            live = np.array(live_group['upd_'+str(upd)]).flatten()
            ppos = np.array(ppos_group['upd_'+str(upd)]).flatten()

            pvch = np.array(pvch_group['upd_'+str(upd)]).flatten()
            stck = np.array(stck_group['upd_'+str(upd)]).flatten()

            repo = np.array(repo_group['upd_'+str(upd)]).flatten()

            chans = [
                np.array(g['upd_'+str(upd)]).flatten()
                for g in chan_groups
            ]


            for idx in range(ncell):

                # if either of the cells are not alive, there is no sharing
                if not live[idx] or not live[drct[idx]]:
                    continue

                related = False

                dest = res['Neighbor']
                dest['Reproduction Incidence'] += (repo[idx] != -1)
                dest['Relationship Incidence'] += 1
                dest['Total Source Stockpile'] += stck[drct[idx]]

                for lev in range(nlev):
                    # remember, these are mutually exclusive
                    if chans[lev][idx] == chans[lev][drct[idx]]:
                        dest = res['Channelmate ' + str(lev)]
                        dest['Reproduction Incidence'] += (repo[idx] != -1)
                        dest['Relationship Incidence'] += 1
                        dest['Total Source Stockpile'] += stck[drct[idx]]
                        related = True
                        break
                else:
                    # runs if no break is called
                    dest = res['Nonchannelmate']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]

                if ppos[idx] == drct[idx] and cage[idx] < cage[drct[idx]]:
                    dest = res['Cell Parent']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]
                    related = True
                elif ppos[drct[idx]] == idx and cage[drct[idx]] < cage[idx]:
                    dest = res['Cell Child']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]
                    related = True
                else:
                    dest = res['Nondirect Cell Relative']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]

                if pvch[idx] == chans[-1][drct[idx]]:
                    dest = res['Propagule Parent']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]
                    related = True
                elif pvch[drct[idx]] == chans[-1][idx]:
                    dest = res['Propagule Child']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]
                    related = True
                else:
                    dest = res['Nondirect Propagule Relative']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]

                if related:
                    dest = res['Related Neighbor']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]
                else:
                    dest = res['Unrelated Neighbor']
                    dest['Reproduction Incidence'] += (repo[idx] != -1)
                    dest['Relationship Incidence'] += 1
                    dest['Total Source Stockpile'] += stck[drct[idx]]

    for data in res.values():
        data['Reproduction Rate'] = (
            data['Reproduction Incidence'] / data['Relationship Incidence']
        )
        data['Mean Source Stockpile'] = (
            data['Total Source Stockpile'] / data['Relationship Incidence']
        )
        data['Relationship Frequency'] = (
            data['Relationship Incidence']
            / res['Neighbor']['Relationship Incidence']
        )

    return res

print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'reproductive_labor',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(tqdm(filenames)),
    '_script_fullcat_hash' : fsh.FilesHash(
                                    file_parcel="full_parcel",
                                    files_join="cat_join"
                                ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : ".csv"
})

pd.DataFrame.from_dict([
    {
        'Measure' : measure,
        'Value' : value,
        'Relationship' : relationship,
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(filename)['treat'],
        'Seed' : kn.unpack(filename)['seed']
    }
    for res, filename in zip(
        Parallel(n_jobs=-1)(
            delayed(CalcRepr)(filename) for filename in tqdm(filenames)
        ), filenames
    )
    for relationship, data in res.items()
    for measure, value in data.items()
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
