# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

def ExtractTreat(filename):
    return next(str for str in filename.split('+') if "treat=" in str)

# check there's only one treatment being analyzed
assert len({ ExtractTreat(filename) for filename in filenames }) == 1

print("TREATMENT:", ExtractTreat(filenames[0]))

files = [h5py.File(filename, 'r') for filename in filenames]

means = [
        np.mean([
            np.sum([
                np.array(file['ResourceHarvested'][lev_key]['upd_'+str(i)]).clip(min=0) / file['ResourceHarvested'][lev_key]['upd_'+str(i)].size
                for lev_key in file['ResourceHarvested']
            ])
            for i in range(first_update, last_update)
        ]) for file in files
    ]

print("num files:" , len(files))
print("per-cell mean: ", np.mean(means))
print("std: ", np.std(means))
