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

matplotlib.rcParams['pdf.fonttype'] = 42

filename = sys.argv[1]
updates = (int(v) for v in sys.argv[2:])

def RenderTriangles(
        top,
        bottom,
        left,
        right,
        live_val,
        radius=21
    ):

    b = (0.5, 0.5, 0.5)
    res = np.array([
        [b] * (radius * 2)
    ] + [
        [b]
        + [left] * idx
        + [b]
        + [top] * (2 * (radius - idx) - 3)
        + [b]
        + [right] * idx
        for idx in range(radius - 1)
    ] + [
        [b] + [left] * (radius - 1) + [b] + [right] * (radius - 1)
    ] + [
        [b]
        + [left] * (radius - idx - 1)
        + [b]
        + [bottom] * (2 * idx - 1)
        + [b]
        + [right] * (radius - idx - 1)
        for idx in range(1, radius)
    ]) if live_val else np.full((radius*2, radius*2, 3), 0.0)

    return res

def RenderAndSave(upd, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    channel = np.array(
        file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
    ).flatten()
    function = [
        np.array(
            file['Functions']['dir_'+str(dir)]['upd_'+str(upd)]
        ).flatten()
        for dir in range(4)
    ]
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
    cmapper = {}
    for id in ids:
        fps_to_counts = []
        idxs = []
        for flat_idx, idx in enumerate(index.flatten()):
            if channel[flat_idx] == id:
                idxs.append(idx)
                if live.flatten()[flat_idx]:
                    fpcounts = defaultdict(lambda: 0)
                    for dir in range(4):
                        fps = map(
                            lambda x: x['value0']['value0'],
                            json.loads(
                                function[dir][flat_idx].decode("utf-8")
                            )['value0']
                        )
                        for fp in fps:
                            fpcounts[fp] += 1
                    fps_to_counts.append(fpcounts)

        df = pd.DataFrame.from_records(fps_to_counts).fillna(0)

        pca = PCA(n_components=min(3, len(df.columns)))

        n=min(3, len(df.columns), len(df))
        if n:
            pca = PCA(n_components=n)

            pc = None
            with warnings.catch_warnings():
                # ignore sklearn and divide by zero warnings
                # (we handle them below)
                warnings.simplefilter("ignore")
                pc = pca.fit_transform(df.to_numpy())
                pc = (pc - pc.min(0)) / pc.ptp(0)

            for idx, row in zip(idxs, pc):
                cmapper[idx] = (
                    row[0] if row.size >= 1 and not np.isnan(row[0]) else 0.5,
                    row[1] if row.size >= 2 and not np.isnan(row[1]) else 0.5,
                    row[2] if row.size >= 3 and not np.isnan(row[2]) else 0.5,
                )
        else:
            for idx in idxs:
                cmapper[idx] = (0.5, 0.5, 0.5)

    image = np.array([
        [
            cmapper[val_index] if val_live else (0.0,0.0,0.0)
            for val_index, val_live in zip(row_index, row_live)
        ]
    for row_index, row_live in zip(index, live)])

    plt.figure(figsize=(18,18))

    plt.imshow(
        image,
        extent = (0, image.shape[1], image.shape[0], 0)
    )

    plt.axis('off')
    plt.grid(b=None)

    lines_0 = LineCollection([
        ((x,y), dest)
        for x in range(image.shape[0])
        for y in range(image.shape[1])
        for dest in ((x+1,y), (x,y+1))
        if data_0[y][x] != data_0[dest[1]-1][dest[0]-1]
    ], linestyle='solid', colors='white')
    plt.gca().add_collection(lines_0)

    lines_1 = LineCollection([
        ((x,y), dest)
        for x in range(image.shape[0])
        for y in range(image.shape[1])
        for dest in ((x+1,y), (x,y+1))
        if data_1[y][x] != data_1[dest[1]-1][dest[0]-1]
    ], linestyle='solid', colors='black')
    plt.gca().add_collection(lines_1)

    plt.savefig(
        kn.pack({
            'title' : 'function_viz',
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
