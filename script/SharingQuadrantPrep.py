# usage:
# num_files h5_filenames updates

import numpy as np
import h5py
import sys
import os
from tqdm import tqdm
import pandas as pd
from keyname import keyname as kn
from fileshash import fileshash as fsh
import re
from collections import Counter, defaultdict
from joblib import delayed, Parallel
import json

num_files = int(sys.argv[1])
filenames = sys.argv[2:num_files+2]
updates = [int(v) for v in sys.argv[num_files+2:]]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def SharingQuadrant(filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))
    index = np.array(file['Index']['own'])
    coords = np.array([
        [{'r' : r, 'c' : c} for c, val in enumerate(row)]
        for r, row in enumerate(index)
    ]).flatten()
    coords = [(coord['r'], coord['c']) for coord in coords]
    quadrant_shares = defaultdict(lambda: [0 for __ in range(4)])
    quadrant_sizes = defaultdict(lambda: [0 for __ in range(4)])


    for update in tqdm(updates):

        chans = np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(update)]
        ).flatten()
        lives = np.array(
            file['Live']['upd_'+str(update)]
        ).flatten()
        total_shares = [
            sum(pos)
            for pos in zip(*(
                np.array(
                file['ResourceContributed']['dir_'+str(dir)]['upd_'+str(update)]
                ).flatten()
                for dir in range(4)
            ))
        ]

        unique_chans = [
            key
            for key, count in
            Counter(chans).items()
            if count >= 9
        ]

        chan_to_coords = defaultdict(set)

        for i in range(len(coords)):
            if lives[i] and chans[i] in unique_chans:
                chan_to_coords[chans[i]].add(coords[i])

        chan_to_centroid = {
            chan : (
                np.mean([
                    # toroidal grid math :(
                    r + 60 if r < 15 and any(r > 45 for r, c in coords) else r
                    for r, c in coords
                ]) % 60,
                np.mean([
                    # toroidal grid math :(
                    c + 60 if c < 15 and any(c > 45 for r, c in coords) else c
                    for r, c in coords
                ]) % 60,
            )
            for chan, coords in chan_to_coords.items()
            if len(coords) # exclude dead channel
        }

        for i in range(len(coords)):
            if lives[i] and chans[i] in chan_to_centroid:
                r, c = np.subtract(coords[i], chan_to_centroid[chans[i]])

                # toroidal grid math :(
                if r > 30:
                    r -= 60
                elif r < -30:
                    r += 60

                # toroidal grid math :(
                if c > 30:
                    c -= 60
                elif c < -30:
                    c += 60

                quadrant_shares[chans[i]][
                    (0 if c > 0 else 1) if r > 0
                    else (2 if c > 0 else 3)
                ] += total_shares[i]
                quadrant_sizes[chans[i]][
                    (0 if c > 0 else 1) if r > 0
                    else (2 if c > 0 else 3)
                ] += 1


    quadrant_share_densities = {
        key : [
            share / size
            for share, size in zip(quadrant_shares[key], quadrant_sizes[key])
        ]
        for key in quadrant_sizes.keys() & quadrant_shares.keys()
        if all(quadrant_sizes[key])
    }

    return [
        max(densities) / sum(densities) if sum(densities) else 0.25
        for densities in quadrant_share_densities.values()
    ]

def SafeSharingQuadrant(filename):
    try:
        return SharingQuadrant(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", e)
        return None


print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'sharingquadrant',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : '.csv'
})

pd.DataFrame.from_dict([
    {
        'Genotype' : (
            'Wild Type' if 'id=1' in filename
            else 'Messaging Knockout' if 'id=2' in filename
            else None
        ),
        'Seed' : kn.unpack(filename)['seed'],
        'Maximum Quadrant Resource-Sharing Fraction': entry,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeSharingQuadrant)(filename)
            for filename in tqdm(filenames)
        )
    )
    for entry in res
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
