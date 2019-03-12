# usage:
# first_update last_update h5_filenames

import numpy as np
import h5py
import sys
from tqdm import tqdm
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

first_update = int(sys.argv[1])
last_update = int(sys.argv[2])
filenames = sys.argv[3:]

def FracApoptosis(file):
    return np.mean([
        1 if apop[idx] else 0 # 1 and 2 are apoptosis types
        for apop in [
            np.array(
                file['Apoptosis']['upd_'+str(upd)]
            ).flatten()
            for upd in range(first_update, last_update)
        ]
        for idx in range(file['Index']['own'].size)
    ])

def ExtractTreat(filename):
    return next(str for str in filename.split('+') if "treat=" in str)

df = pd.DataFrame.from_dict([
    {
        'Treat' : treat,
        'FracApoptosis' : FracApoptosis(file),
    }
    for treat, file in (
        (ExtractTreat(filename), h5py.File(filename, 'r')) for filename in filenames
    )
])

print("num files:" , len(filenames))
sns.barplot(x="Treat", y="FracApoptosis", data=df)
plt.show()
