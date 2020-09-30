# usage: file

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
from natsort import natsorted

matplotlib.rcParams['pdf.fonttype'] = 42

filename = sys.argv[1]

def RenderAndSave(upd_key, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    live = np.array(file['Live'][upd_key])
    traffic = np.array(file['InboxTraffic']['dir_4'][upd_key])

    image = np.array([
        [
            # dead
            (0.0, 0.0, 0.0)
            if not val_live else
            (lambda n: (1.0, 1.0, 0.0) if n > 4 else {
                0 : (1.0, 1.0, 1.0), # white
                1 : (0.0, 1.0, 0.0), # green
                2 : (0.0, 0.0, 1.0), # blue
                3 : (1.0, 0.0, 1.0), # purple
                4 : (1.0, 0.0, 0.0), # red
            }[n])(val_traffic)
            for val_traffic, val_live in zip(row_traffic, row_live)
        ]
    for row_traffic, row_live in zip(traffic, live)])

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
            'title' : 'messaging_spiker_viz',
            'update' : ''.join(c for c in upd_key if c.isdigit()),
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

file = h5py.File(filename, 'r')
upd_keys = natsorted(
    [key for key in file['InboxTraffic']['dir_4']]
)[-16:]

Parallel(n_jobs=-1)(
    delayed(RenderAndSave)(upd_key, filename) for upd_key in tqdm(upd_keys)
)
