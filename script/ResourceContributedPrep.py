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

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

# check there's only one treatment being analyzed
assert len({ kn.unpack(filename)['treat'] for filename in filenames }) == 1

print("TREATMENT:", kn.unpack(filenames[0])['treat']))

nlev = h5py.File(filenames[0], 'r').attrs['NLEV'][0]
ntile = h5py.File(filenames[0], 'r')['Index']['own'].size

files = [h5py.File(filename, 'r') for filename in filenames]

res = {}

res['none'] = (
[
    np.mean([
        np.mean(file['ResourceContributed'][dir_key]['upd_'+str(upd)])
        for dir_key in file['ResourceContributed']
        for upd in range(first_update, last_update)
    ])
    for file in tqdm(files)
],
None
)

for lev_key in files[0]['Channel']:
    res['samechannel_'+lev_key] = (
    [
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel'][lev_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[idx] == ch[dir[idx]])
    ]) for file in tqdm(files)
    ],
    [
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel'][lev_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[idx] != ch[dir[idx]])
    ]) for file in tqdm(files)
    ]
    )

res['cell_parent'] = (
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, par, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['ParentPos']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (par[idx] == dir[idx])
    ]) for file in tqdm(files)
],
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, par, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['ParentPos']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (par[idx] != dir[idx])
    ]) for file in tqdm(files)
]
)

res['cell_child'] = (
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, par, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['ParentPos']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (idx == par[dir[idx]])
    ]) for file in tqdm(files)
],
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, par, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['ParentPos']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (idx != par[dir[idx]])
    ]) for file in tqdm(files)
]
)

res['propagule_parent'] = (
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, pc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['PrevChan']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[dir[idx]] != ch[idx] and pc[idx] == ch[dir[idx]])
    ]) for file in tqdm(files)
],
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, pc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['PrevChan']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[dir[idx]] != ch[idx] and pc[idx] != ch[dir[idx]])
    ]) for file in tqdm(files)
]
)

res['propagule_child'] = (
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, pc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['PrevChan']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[dir[idx]] != ch[idx] and ch[idx] == pc[dir[idx]])
    ]) for file in tqdm(files)
],
[
    np.mean([
        rc[idx]
        for dir_key in file['ResourceContributed']
        for rc, pc, ch, dir in [
            (np.array(
                file['ResourceContributed'][dir_key]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['PrevChan']['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
            ).flatten(),
            np.array(
                file['Index'][dir_key]
            ).flatten())
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if (ch[dir[idx]] != ch[idx] and ch[idx] != pc[dir[idx]])
    ]) for file in tqdm(files)
]
)

print("num files:" , len(files))

for k, (v,not_v) in sorted(res.items()):
    print()
    print("FILTER: ", k)
    print("   YES mean/std: ", np.mean(v), "/", np.std(v))
    if not_v:
        print("   NOT mean/std: ", np.mean(not_v), "/", np.std(not_v))
        print(
            "   WELCH TEST: ",
            stats.ttest_ind(
                v,
                not_v,
                equal_var=False
            )
        )
    print()

outfile = kn.pack({
    'treat' : kn.unpack(filenames[0])['treat'],
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
        'Filter' : k,
        'Shared Resource Per Cell Pair Update' : v,
        'Applied' : 'true' if idx == 0 else 'false',
        'First Update' : first_update,
        'Last Update' : last_update,
        'Treatment' : kn.unpack(fname)['treat'],
        'Seed' : kn.unpack(fname)['seed']
    }
    for k, tup in res.items()
    for idx, vs in enumerate(tup)
    if vs
    for v, fname in zip(vs,filenames)
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
