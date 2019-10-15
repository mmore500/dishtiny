# usage: file updates...

import sys
import os
import warnings

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection

import seaborn as sns
import pandas as pd
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh
import h5py
import colorsys
from tqdm import tqdm
from joblib import delayed, Parallel
import json
from sklearn.decomposition import PCA
from collections import defaultdict
from itertools import product

matplotlib.rcParams['pdf.fonttype'] = 42

filename = sys.argv[1]
updates = [int(v) for v in sys.argv[2:]]

def RenderTriangles(
        right,
        left,
        bottom,
        top,
        live_val,
        radius=21
    ):

    return np.array([
        [top] * (radius * 2)
    ] + [
        [left]
        + [left] * idx
        + [top]
        + [top] * (2 * (radius - idx) - 3)
        + [right]
        + [right] * idx
        for idx in range(radius - 1)
    ] + [
        [left] + [left] * (radius - 1) + [top] + [right] * (radius - 1)
    ] + [
        [left]
        + [left] * (radius - idx - 1)
        + [bottom]
        + [bottom] * (2 * idx - 1)
        + [right]
        + [right] * (radius - idx - 1)
        for idx in range(1, radius)
    ]) if live_val else np.full((radius*2, radius*2, 3), 0.0)

# for each group, get all regulators
def MakePCA(id, updates, filename):

    # make pcamapper using ALL updates
    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    index = np.array(file['Index']['own'])

    tags_to_regs = []

    for upd, dir in product(updates, range(4)):

        decoder = np.array(
            file['Regulators']['decoder']['upd_'+str(upd)]
        ).flatten()
        channel = np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
        ).flatten()
        regulator = np.array(
            file['Regulators']['dir_'+str(dir)]['upd_'+str(upd)]
        ).flatten()
        live = np.array(
            file['Live']['upd_'+str(upd)]
        ).flatten()

        for idx in index.flatten():

            if not (channel[idx] == id and live[idx]):
                continue

            archive = json.loads(
                decoder[regulator[idx]].decode("utf-8")
            )['value0']

            tags = {
                d['key'] : d['value']['value0']['value0']
                for d in archive['tags']
            }
            regs = {
                pair['key'] : pair['value']
                for pair in archive['regulators']
            }
            tags_to_regs.append({
                tags[uid] : regs[uid]
                for uid in archive['uids']
            })

    df = pd.DataFrame.from_records(
        tags_to_regs
    )
    # if less than half the cells have the regulator, drop it
    # otherwise (e.g., probably a few cells lost it), assume it's default
    df = df.dropna(thresh=len(df)/2, axis=1).fillna(1)

    n = min(3, len(df.columns), len(df))

    pca = None
    res = None
    with warnings.catch_warnings():
        # ignore sklearn warnings
        # (we handle them below)
        warnings.simplefilter("ignore")
        pca = PCA(n_components=n).fit(df.to_numpy()) if n else None
        res = pca.transform(df.to_numpy()) if n else None

    return (id, (
        pca,
        list(df.columns),
        res.min(0),
        res.ptp(0)
    ) if n else None)

# get unique group IDs
file = h5py.File(filename, 'r')
nlev = int(file.attrs.get('NLEV'))

id_upds = defaultdict(list)
for id, upd in {
    (id, upd)
    for upd in updates
    for id in np.array(
        file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
    ).flatten()
}:
    id_upds[id].append(upd)

results = Parallel(n_jobs=-1)(
    delayed(MakePCA)(id, upds, filename)
    for id, upds in tqdm(id_upds.items())
)

pcamapper = { id : res for id, res in results }

def RenderAndSave(upd, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    channel = np.array(
        file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
    ).flatten()
    regulator = [
        np.array(
            file['Regulators']['dir_'+str(dir)]['upd_'+str(upd)]
        ).flatten()
        for dir in range(4)
    ]
    decoder = np.array(
        file['Regulators']['decoder']['upd_'+str(upd)]
    ).flatten()
    live = np.array(file['Live']['upd_'+str(upd)])
    index = np.array(file['Index']['own'])

    data_0 = np.array(file['Channel']['lev_0']['upd_'+str(upd)])
    data_1 = (
        np.array(file['Channel']['lev_0']['upd_'+str(upd)])
        if nlev == 1 else
        np.array(file['Channel']['lev_1']['upd_'+str(upd)])
    )

    # get unique group IDs
    ids = { id for id in channel.flatten() }

    # for each group, get all functions
    cmapper = [ {} for dir in range(4) ]
    for id in ids:
        tags_to_regs = []
        idxs = []
        dirs = []
        for flat_idx, idx in enumerate(index.flatten()):
            if channel[flat_idx] == id:
                for dir in range(4):
                    idxs.append( idx )
                    dirs.append( dir )
                    if live.flatten()[flat_idx]:
                        archive = json.loads(
                            decoder[regulator[dir][flat_idx]].decode("utf-8")
                        )['value0']
                        tags = {
                            d['key'] : d['value']['value0']['value0']
                            for d in archive['tags']
                        }
                        regulators = {
                            d['key'] : d['value']
                            for d in archive['regulators']
                        }
                        tags_to_regs.append({
                            tags[uid] : regulators[uid] for uid in archive['uids']
                        })

        df = pd.DataFrame.from_records(tags_to_regs).fillna(1)

        if pcamapper[id] is not None:
            pca, cols, minv, ptpv = pcamapper[id]

            # some regulators may not be present during a particular update
            # due to mutational gain/loss
            # pad 'em in
            for col in cols:
                if col not in df:
                    df[col] = 1

            pc = pca.transform(df[cols].to_numpy())

            with warnings.catch_warnings():
                # ignore divide by zero warnings
                # (we handle them below)
                warnings.simplefilter("ignore")
                pc = (pc - minv) / ptpv

            for idx, dir, row in zip(idxs, dirs, pc):
                cmapper[dir][idx] = (
                    row[0] if row.size >= 1 and not np.isnan(row[0]) else 0.5,
                    row[1] if row.size >= 2 and not np.isnan(row[1]) else 0.5,
                    row[2] if row.size >= 3 and not np.isnan(row[2]) else 0.5,
                )
        else:
            for idx, dir in zip(idxs, dirs):
                cmapper[dir][idx] = (0.5, 0.5, 0.5)

    image = np.flip(np.rot90(np.transpose(np.block([
        [
            np.transpose(RenderTriangles(
                cmapper[0][val_index],
                cmapper[1][val_index],
                cmapper[2][val_index],
                cmapper[3][val_index],
                val_live
            )) for val_live, val_index in zip(row_live, row_index)
        ]
        for row_live, row_index
        in zip(live, index)
    ])),k=1),axis=0)

    plt.figure(figsize=(18,18))

    plt.imshow(
        image,
        extent = (0, image.shape[1], image.shape[0], 0)
    )

    plt.axis('off')
    plt.grid(b=None)

    rescale = lambda coord: [v * 42 for v in coord]
    lines_0 = LineCollection([
        [ rescale(coord) for coord in ((x,y), dest) ]
        for x in range(index.shape[0])
        for y in range(index.shape[1])
        for dest in ((x+1,y), (x,y+1))
        if data_0[y][x] != data_0[dest[1]-1][dest[0]-1]
    ], linestyle='solid', colors='white', linewidths=(2,))
    plt.gca().add_collection(lines_0)

    lines_1 = LineCollection([
        [ rescale(coord) for coord in ((x,y), dest) ]
        for x in range(index.shape[0])
        for y in range(index.shape[1])
        for dest in ((x+1,y), (x,y+1))
        if data_1[y][x] != data_1[dest[1]-1][dest[0]-1]
    ], linestyle='solid', colors='black', linewidths=(2,))
    plt.gca().add_collection(lines_1)

    plt.savefig(
        kn.pack({
            'title' : 'directional_regulator_consistent_viz',
            'update' : str(upd),
            'seed' : kn.unpack(filename)['seed'],
            'treat' : kn.unpack(filename)['treat'],
            '_data_hathash_hash' : fsh.FilesHash().hash_files([filename]),
            '_script_fullcat_hash' : fsh.FilesHash(
                                                file_parcel="full_parcel",
                                                files_join="cat_join"
                                            ).hash_files([sys.argv[0]]),
            '_source_hash' :kn.unpack(filename)['_source_hash'],
            'ext' : '.png'
        }),
        transparent=True,
        bbox_inches='tight',
        pad_inches=0
    )

    plt.clf()
    plt.close(plt.gcf())

Parallel(n_jobs=-1)(
    delayed(RenderAndSave)(upd, filename) for upd in tqdm(updates)
)
