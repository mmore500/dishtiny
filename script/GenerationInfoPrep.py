# usage:
# h5_filenames

# how to run on the big screen directory
# pwd=${PWD}; for f in seed*step*; do cd ${pwd}; cd $f; python3 ~/dishtiny/script/CalcGenerationInfo.py *.h5 &; while [ $(jobs -r | wc -l) -gt 16 ]; do sleep 1; done; done

import numpy as np
import h5py
import sys
from tqdm import tqdm
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import os
from keyname import keyname as kn
from fileshash import fileshash as fsh
from joblib import delayed, Parallel
from natsort import natsorted

filenames = sys.argv[1:]

print(filenames)

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def CalcMeanCellGen(filename):
    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))
    upd_key = natsorted(
        [key for key in file['Live']]
    )[-1]

    return [
        np.mean([
            cgen
            for cgen, live in zip(
                np.array(
                    file['CellGen']['lev_'+str(lev)][upd_key]
                ).flatten(),
                np.array(
                    file['Live'][upd_key]
                ).flatten()
            )
            if live
        ])
        for lev in range(nlev+1)
    ]

def SafeCalcMeanCellGen(filename):
    try:
        return CalcMeanCellGen(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", filename)
        print("   ", e)
        return None

df = pd.DataFrame.from_dict([
    {
        'Generations Elapsed' : res,
        'Level' : lev,
    }
    for filename, cellgen in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeCalcMeanCellGen)(filename)
            for filename in tqdm(filenames)
        )
    )
    if cellgen is not None
    for lev, res in enumerate(cellgen)
])

print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'elapsedgenerations',
    '_data_hathash_hash' : fsh.FilesHash().hash_files(filenames),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(filenames[0])['_source_hash'],
    'ext' : '.csv',
})

df.to_csv(outfile, index=False)

print('Output saved to', outfile)
