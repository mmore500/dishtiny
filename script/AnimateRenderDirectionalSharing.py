# usage: file updates...

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap

import sys
import os
import pandas as pd
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh
import h5py
import colorsys
from tqdm import tqdm
from joblib import delayed, Parallel

matplotlib.rcParams['pdf.fonttype'] = 42

filename = sys.argv[1]
updates = [int(v) for v in sys.argv[2:]]

file = h5py.File(filename, 'r')
most = max([
    np.max(np.array(
        file['ResourceContributed']['dir_'+str(dir)]['upd_'+str(upd)]
    ))
    for dir in range(4)
    for upd in updates
])
if not most:
    most = 1.0

def ColorMap(val):
    return (
        1.0 - val/most,
        1.0 - val/most,
        1.0,
        1.0
    )

def RenderTriangles(
        top_val,
        bottom_val,
        left_val,
        right_val,
        live_val,
        radius=21
    ):

    top = ColorMap(top_val)
    bottom = ColorMap(bottom_val)
    left = ColorMap(left_val)
    right = ColorMap(right_val)

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
    ]) if live_val else np.full((radius, radius), (0.0, 0.0, 0.0, 1.0))


def RenderAndSave(upd, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    top = np.array(file['ResourceContributed']['dir_0']['upd_'+str(upd)])
    bottom = np.array(file['ResourceContributed']['dir_1']['upd_'+str(upd)])
    left = np.array(file['ResourceContributed']['dir_3']['upd_'+str(upd)])
    right = np.array(file['ResourceContributed']['dir_2']['upd_'+str(upd)])
    live = np.array(file['Live']['upd_'+str(upd)])

    data_0 = np.array(file['Channel']['lev_0']['upd_'+str(upd)])
    data_1 = (
        np.array(file['Channel']['lev_0']['upd_'+str(upd)])
        if nlev == 1 else
        np.array(file['Channel']['lev_1']['upd_'+str(upd)])
    )

    image = np.transpose(np.block([
        [
            np.transpose(RenderTriangles(
                val_top,
                val_bottom,
                val_left,
                val_right,
                val_live
            )) for val_top, val_bottom, val_left, val_right, val_live in zip(
                row_top,
                row_bottom,
                row_left,
                row_right,
                row_live
            )
        ]
        for row_top, row_bottom, row_left, row_right, row_live
        in zip(
            top,
            bottom,
            left,
            right,
            live
        )
    ]))

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
        for x in range(data_0.shape[0])
        for y in range(data_0.shape[1])
        for dest in ((x+1,y), (x,y+1))
        if data_0[y][x] != data_0[dest[1]-1][dest[0]-1]
    ], linestyle=(0, (1, 3)), colors='0.5')
    plt.gca().add_collection(lines_0)

    lines_1 = LineCollection([
        [ rescale(coord) for coord in ((x,y), dest) ]
        for x in range(data_1.shape[0])
        for y in range(data_1.shape[1])
        for dest in ((x+1,y), (x,y+1))
        if data_1[y][x] != data_1[dest[1]-1][dest[0]-1]
    ], linestyle='solid', colors='black')
    plt.gca().add_collection(lines_1)

    plt.savefig(
        kn.pack({
            'title' : 'directional_sharing_viz',
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
