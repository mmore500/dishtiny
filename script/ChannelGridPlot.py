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

matplotlib.rcParams['pdf.fonttype'] = 42


filename = sys.argv[1]
updates = [int(v) for v in sys.argv[2:]]

file = h5py.File(filename, 'r')

for upd in tqdm(updates):

    data = np.array(file['Channel']['lev_0']['upd_'+str(upd)])

    image = np.array([
        [
            tuple(colorsys.hsv_to_rgb(tv%1.0, tv%0.6 + 0.4,1.0))
            if val else (0,0,0)
        for val in row for tv in (val/2**63,)]
    for row in data])

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
        if data[y][x] != data[dest[1]-1][dest[0]-1]
    ], linestyle='solid', colors='black')
    plt.gca().add_collection(lines)

    plt.savefig(
        kn.pack({
            'title' : 'channel_viz',
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
