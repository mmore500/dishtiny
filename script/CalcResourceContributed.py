# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
import scipy.stats as stats
from tqdm import tqdm

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

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
    for file in files
],
None
)

for lev_key in files[0]['Channel']:
    res['means_samechannel_'+lev_key] = (
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

res['means_cell_parent'] = (
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

res['means_cell_child'] = (
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

res['means_propagule_parent'] = (
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
        if (pc[idx] == ch[dir[idx]])
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
        if (pc[idx] != ch[dir[idx]])
    ]) for file in tqdm(files)
]
)

res['means_propagule_child'] = (
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
        if (ch[idx] == pc[dir[idx]])
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
        if (ch[idx] != pc[dir[idx]])
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
