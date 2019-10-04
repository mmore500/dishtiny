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

def RenderAndSave(upd, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    channel = np.array(
        file['Channel']['lev_'+str(nlev-1)]['upd_'+str(upd)]
    ).flatten()
    regulators = [
        np.array(
            file['Regulators']['dir_'+str(dir)]['upd_'+str(upd)]
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

    # for each group, get all regulators
    cmapper = {}
    for id in ids:
        tags_to_regs = []
        idxs = []
        for idx in range(index.flatten().size):
            if channel[idx] == id and live.flatten()[idx]:
                idxs.append(idx)
                archives = [ json.loads(
                    regulator[idx].decode("utf-8")
                )['value0'] for regulator in regulators ]
                tags = {
                    d['key'] : d['value']['value0']['value0']
                    for d in archives[0]['tags']
                }

                regulatorsum = defaultdict(lambda: 0.0)
                for archive in archives:
                    for d in archive['regulators']:
                        regulatorsum[d['key']] += d['value']

                tags_to_regs.append({
                    tags[uid] : regulatorsum[uid] for uid in archives[0]['uids']
                })

        # if less than half the cells have the regulator, drop it
        # otherwise (e.g., probably a few cells lost it), assume it's default
        df = pd.DataFrame.from_records(
            tags_to_regs
        )
        df = df.dropna(thresh=len(df)/2).fillna(1)

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
            'title' : 'regulator_viz',
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
