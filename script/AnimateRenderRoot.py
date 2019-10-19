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

    rootids = np.array(file['RootID']['upd_'+str(upd)])
    live = np.array(file['Live']['upd_'+str(upd)])

    image = np.array([
        [
            # dead
            (0.0, 0.0, 0.0)
            if not val_live else
            {
                1 : (1.0, 0.0, 0.0),
                2 : (0.0, 1.0, 0.0),
                3 : (0.0, 0.0, 1.0),
            }[val_rootid]
            for val_rootid, val_live in zip(row_rootid, row_live)
        ]
    for row_rootid, row_live in zip(rootids, live)])

    plt.figure(figsize=(18,18))

    plt.imshow(
        image,
        extent = (0, image.shape[1], image.shape[0], 0)
    )

    plt.axis('off')
    plt.grid(b=None)

    lines = LineCollection([
        ((x,y), dest)
        for x in range(image.shape[0])
        for y in range(image.shape[1])
        for dest in ((x+1,y), (x,y+1))
    ], linestyle='solid', colors='black')
    plt.gca().add_collection(lines)

    plt.savefig(
        kn.pack({
            'title' : 'root_viz',
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
