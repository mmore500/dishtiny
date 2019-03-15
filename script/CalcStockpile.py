# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
from tqdm import tqdm
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import os

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

repThresh = 3

def FracOverRepThresh(file):
    return np.mean([
        1 if stock[idx] >= repThresh else 0
        for stock in [
            np.array(
                file['Stockpile']['upd_'+str(upd)]
            ).flatten()
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
    ])

def MeanOverRepThreshAmt(file):
    return np.mean([
        stock[idx]
        for stock in [
            np.array(
                file['Stockpile']['upd_'+str(upd)]
            ).flatten()
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
        if stock[idx] >= repThresh
    ])

def ExtractTreat(filename):
    return next(
        str for str in os.path.basename(filename).split('+') if "treat=" in str
    )

data = pd.DataFrame.from_dict([
    {
        'Treat' : treat,
        'FracOverRepThresh' : FracOverRepThresh(file),
        'MeanOverRepThreshAmt' : MeanOverRepThreshAmt(file)
    }
    for treat, file in (
        (ExtractTreat(filename), h5py.File(filename, 'r')) for filename in filenames
    )
])

print("num files:" , len(filenames))
sns.barplot(x="Treat", y="FracOverRepThresh", data=data)
plt.show()
sns.barplot(x="Treat", y="MeanOverRepThreshAmt", data=data)
plt.show()
