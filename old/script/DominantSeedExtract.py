# usage:
# update h5_filenames

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

update = int(sys.argv[1])
filenames = sys.argv[2:]

def ExtractSeed(filename):
    file = h5py.File(filename, 'r')

    lives = list(np.array(
        file['Live']['upd_'+str(update)]
    ).flatten())

    prog_decoder = list(np.array(
        file['Population']['decoder']['upd_'+str(update)]
    ).flatten())
    progs = list(np.array(
        file['Population']['upd_'+str(update)]
    ).flatten())

    trigger_decoder = list(np.array(
        file['Triggers']['decoder']['upd_'+str(update)]
    ).flatten())
    triggers = list(np.array(
        file['Triggers']['upd_'+str(update)]
    ).flatten())

    reg_decoder = list(np.array(
        file['Regulators']['decoder']['upd_'+str(update)]
    ).flatten())
    regs = list(zip(*[
        list(np.array(
            file['Regulators']['dir_'+str(dir)]['upd_'+str(update)]
        ).flatten())
        for dir in range(4)
    ]))

    dominant_prog = max(
        set(
            prog for prog, live in zip(progs,lives) if live
        ),
        key=progs.count
    )

    triggers = [t for p, t in zip(progs, triggers) if p == dominant_prog]
    dominant_trigger = max(set(triggers), key=triggers.count)

    regs = [t for p, t in zip(progs, regs) if p == dominant_prog]
    dominant_regs = max(set(regs), key=regs.count)

    with open(kn.pack({
        "component" : "program",
        "treat" : kn.unpack(filename)["treat"],
        "seed" : kn.unpack(filename)["seed"],
        '_data_hathash_hash' : fsh.FilesHash().hash_files([filename]),
        '_script_fullcat_hash' : fsh.FilesHash(
                                            file_parcel="full_parcel",
                                            files_join="cat_join"
                                        ).hash_files([sys.argv[0]]),
        "ext" : ".txt",
    }), 'w') as file:
        file.write(
            prog_decoder[dominant_prog].decode("utf-8")
        )

    with open(kn.pack({
        "component" : "triggers",
        "treat" : kn.unpack(filename)["treat"],
        "seed" : kn.unpack(filename)["seed"],
        '_data_hathash_hash' : fsh.FilesHash().hash_files([filename]),
        '_script_fullcat_hash' : fsh.FilesHash(
                                            file_parcel="full_parcel",
                                            files_join="cat_join"
                                        ).hash_files([sys.argv[0]]),
        "ext" : ".json",
    }), 'w') as file:
        file.write(
            trigger_decoder[dominant_trigger].decode("utf-8")
        )

    for dir, reg in enumerate(dominant_regs):
        with open(kn.pack({
            "component" : "regulator",
            "dir" : dir,
            "treat" : kn.unpack(filename)["treat"],
            "seed" : kn.unpack(filename)["seed"],
            '_data_hathash_hash' : fsh.FilesHash().hash_files([filename]),
            '_script_fullcat_hash' : fsh.FilesHash(
                                                file_parcel="full_parcel",
                                                files_join="cat_join"
                                            ).hash_files([sys.argv[0]]),
            "ext" : ".json",
        }), 'w') as file:
            file.write(
                reg_decoder[reg].decode("utf-8")
            )

    return None

def SafeExtractSeed(filename):
    # try:
    return ExtractSeed(filename)
    # except Exception as e:
    #     print("warning: corrupt or incomplete data file... skipping")
    #     print("   ", e)
    #     return None

Parallel(n_jobs=-1)(
    delayed(SafeExtractSeed)(filename) for filename in tqdm(filenames)
)
