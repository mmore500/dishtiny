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
from collections import Counter
from joblib import delayed, Parallel
import json

num_files = int(sys.argv[1])
filenames = sys.argv[2:num_files+2]
updates = [int(v) for v in sys.argv[num_files+2:]]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def InteriorPropagule(filename):

    file = h5py.File(filename, 'r')
    indices = {
        idx : i
        for i, idx in enumerate(np.array(file['Index']['own']).flatten())
    }
    neighs = [
        np.array(file['Index']['dir_'+str(dir)]).flatten()
        for dir in range(4)
    ]
    nlev = int(file.attrs.get('NLEV'))

    res = {}
    for update in tqdm(updates):
        chans = np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(update)]
        ).flatten()

        lives = np.array(file['Live']['upd_'+str(update)]).flatten()
        cages = np.array(file['CellAge']['upd_'+str(update)]).flatten()
        pvchs = np.array(file['PrevChan']['upd_'+str(update)]).flatten()

        num_interior_propagules = sum(
            1
            for i in range(len(indices))
            if lives[i]
            if cages[i] < 8
            if 1 == len({
                chans[indices[neigh[i]]]
                for neigh in neighs
            })
            if pvchs[i] == chans[indices[neighs[0][i]]]
        )

        res[update] = num_interior_propagules

    return res

def SafeInteriorPropagule(filename):
    try:
        return InteriorPropagule(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", e)
        return None


print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'interior_propagules',
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
        'Update' : update,
        'Genotype' : (
            'Wild Type' if 'id=1' in filename
            else 'Propagule Knockout' if 'propagule' in filename
            else 'Regulation Knockout' if 'regulation' in filename
            else None
        ),
        'Interior Propagule Count' : num_interior_propagules,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeInteriorPropagule)(filename)
            for filename in tqdm(filenames)
        )
    )
    for update, num_interior_propagules in res.items()
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
