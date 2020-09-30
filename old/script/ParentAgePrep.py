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

def ParentAge(filename):

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

    res = []
    for update in tqdm(updates):
        chans = np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(update)]
        ).flatten()
        cages = np.array(file['CellAge']['upd_'+str(update)]).flatten()

        chanages = {
            channel :
                max(age for age, chan in zip(cages, chans) if chan == channel)
            for channel in set(chans)
        }

        lives = np.array(file['Live']['upd_'+str(update)]).flatten()
        pvchs = np.array(file['PrevChan']['upd_'+str(update)]).flatten()
        ppos = np.array(file['ParentPos']['upd_'+str(update)]).flatten()


        for i in range(len(indices)):
            for neigh in neighs:
                if (
                    lives[i]
                    and lives[indices[neigh[i]]]
                    and chans[indices[neigh[i]]] != chans[i]
                    and chans[indices[neigh[i]]] == pvchs[i]
                    and indices[neigh[i]] == ppos[i]
                    and cages[indices[neigh[i]]] > cages[i]
                    and cages[i] < 8
                    ):
                        res.append(chanages[chans[indices[neigh[i]]]])

    return np.mean(res)

def SafeParentAge(filename):
    try:
        return ParentAge(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", e)
        return None


print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'parentage',
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
        'Propagule Parent Age': res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeParentAge)(filename)
            for filename in tqdm(filenames)
        )
    )
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
