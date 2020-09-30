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
from scipy import stats

import warnings
warnings.filterwarnings("error")


num_files = int(sys.argv[1])
filenames = sys.argv[2:num_files+2]
updates = [int(v) for v in sys.argv[num_files+2:]]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

def GroupConflict(filename):

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

    res = defaultdict(lambda: defaultdict(list))
    for update in tqdm(updates):
        chans = np.array(
            file['Channel']['lev_'+str(nlev-1)]['upd_'+str(update)]
        ).flatten()
        chan_counts = Counter(chans)

        lives = np.array(file['Live']['upd_'+str(update)]).flatten()
        cages = np.array(file['CellAge']['upd_'+str(update)]).flatten()
        pvchs = np.array(file['PrevChan']['upd_'+str(update)]).flatten()
        ppos = np.array(file['ParentPos']['upd_'+str(update)]).flatten()
        cage = np.array(file['CellAge']['upd_'+str(update)]).flatten()

        for i in range(len(indices)):
            for neigh in neighs:
                if (
                    lives[i]
                    and lives[indices[neigh[i]]]
                    # and chan_counts[chans[i]] >= 9
                    # and chan_counts[chans[indices[neigh[i]]]] >= 9
                    and chans[indices[neigh[i]]] != chans[i]
                    # no propagule parent/propagule child
                    # relationship registered
                    and pvchs[i] != chans[indices[neigh[i]]]
                    and pvchs[indices[neigh[i]]] != chans[i]
                    and not ( # not cell parent
                        ppos[i] == indices[neigh[i]]
                        and cage[i] < cage[indices[neigh[i]]]
                    ) and not ( # not cell child
                        ppos[indices[neigh[i]]] == indices[i]
                        and cage[indices[neigh[i]]] < cage[i]
                    )):

                        # border age
                        res[chans[i]][chans[indices[neigh[i]]]].append(
                            min(cages[i], cages[indices[neigh[i]]])
                            # fuzz results so kruskal doesn't throw up
                            + np.random.normal(0, 1e-9)
                        )

    pvals = [
        stats.kruskal(*[
            samp for samp in bychan.values() if len(samp) >= 2
        ])[1] # p value
        for bychan in res.values()
        if sum(1 for samp in bychan.values() if len(samp) >= 2) >= 2
    ]

    # proprotion significant with bonferoni correction
    return sum(1 for p in pvals if p < 0.05/len(pvals)) / len(pvals)

def SafeGroupConflict(filename):
    try:
        return GroupConflict(filename)
    except Exception as e:
        print("warning: corrupt or incomplete data file... skipping")
        print("   ", e)
        return None


print("num files:" , len(filenames))

outfile = kn.pack({
    'title' : 'borderagevariance',
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
        'Fraction Asymmetric Groups' : res,
    }
    for filename, res in zip(
        filenames,
        Parallel(n_jobs=-1)(
            delayed(SafeGroupConflict)(filename)
            for filename in tqdm(filenames)
        )
    )
]).to_csv(outfile, index=False)

print('Output saved to', outfile)
