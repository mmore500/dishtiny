from collections import defaultdict
from tqdm import tqdm
import pandas as pd
import sys

# preliminaries

seed = None

BIN_SIZE = 50000

assert len(sys.argv) == 2, \
    "First argument is filename."

counter = defaultdict(int)

# import data (files created from CleanChannelMap.py)
df = pd.read_csv(sys.argv[1])

seed = df.iloc[0]['seed']

cell_update_counts_by_ch1 = df.groupby('channel1')['count'].sum()

durs = df.groupby(['channel1','update'])['count'].count().groupby('channel1').count()

for ch1, c in tqdm(durs.iteritems()):
    counter[(c * 5552 // BIN_SIZE)*BIN_SIZE] += cell_update_counts_by_ch1[ch1]

res = pd.DataFrame([{'ch1_dur' : k, 'cell_weighted_freq' : v, 'seed' : seed} for k,v in counter.items()])

res.to_csv(
        "level-1-channel-dur-cell-weighted-frequency_" + str(int(seed)) + ".csv",
        index=False
    )
