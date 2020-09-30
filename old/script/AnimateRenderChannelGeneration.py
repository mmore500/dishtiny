# usage: file updates...

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh
import h5py
import colorsys
from tqdm import tqdm
from joblib import delayed, Parallel

matplotlib.rcParams['pdf.fonttype'] = 42

filename = sys.argv[1]
updates = (int(v) for v in sys.argv[2:])

def RenderAndSave(upd, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    changen = np.array(
        file['ChannelGeneration']['lev_'+str(nlev-1)]['upd_'+str(upd)]
    )
    most = max(changen.flatten())

    live = np.array(file['Live']['upd_'+str(upd)])

    data_0 = np.array(file['Channel']['lev_0']['upd_'+str(upd)])
    data_1 = (
        np.array(file['Channel']['lev_0']['upd_'+str(upd)])
        if nlev == 1 else
        np.array(file['Channel']['lev_1']['upd_'+str(upd)])
    )

    image = np.array([
        [
            # dead
            (0.0, 0.0, 0.0)
            if not val_live else
            (
                0.0,
                1.0,
                0.0,
            ) if val_changen < 0 else
            (
                1.0 - val_changen/most,
                1.0  - val_changen/most,
                1.0,
            ) if val_changen <= most else
            (
                1.0,
                0.0,
                0.0,
            )
            for val_changen, val_live in zip(row_changen, row_live)
        ]
    for row_changen, row_live in zip(changen, live)])

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
    ], linestyle=(0, (1, 3)), colors='0.5')
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
            'title' : 'channelgen_viz',
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
