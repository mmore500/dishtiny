# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
import os
from keyname import keyname as kn

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]


# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

# check there's only one treatment being analyzed
assert len({ kn.unpack(filename)['treat'] for filename in filenames }) == 1

print("TREATMENT:", kn.unpack(filenames[0])['treat'])

files = [h5py.File(filename, 'r') for filename in filenames]

means = [
        np.mean([
            np.sum([
                np.array(file['ResourceHarvested'][lev_key]['upd_'+str(i)]).clip(min=0) / file['ResourceHarvested'][lev_key]['upd_'+str(i)].size
                for lev_key in file['ResourceHarvested']
            ])
            for i in range(first_update, last_update)
        ]) for file in tqdm(files)
    ]

print("num files:" , len(files))
print("per-cell mean: ", np.mean(means))
print("std: ", np.std(means))
