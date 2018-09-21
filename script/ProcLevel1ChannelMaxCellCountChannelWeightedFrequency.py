from collections import defaultdict
from tqdm import tqdm
import pandas as pd
import sys

# preliminaries

seed = None

BIN_SIZE = 10

assert len(sys.argv) == 2, \
    "First argument is filename."

counter = defaultdict(int)

# import data (files created from CleanChannelMap.py)
df = pd.read_csv(sys.argv[1])

seed = df.iloc[0]['seed']

cell_update_counts_by_ch1 = df.groupby('channel1')['count'].sum()

max_cell_counts = df.groupby(['channel1','update'])['count'].sum().groupby('channel1').max()


for ch1, c in tqdm(max_cell_counts.iteritems()):
    counter[(c // BIN_SIZE)*BIN_SIZE] += 1

res = pd.DataFrame([{'ch1_max_cell_count' : k, 'channel_weighted_freq' : v, 'seed' : seed} for k,v in counter.items()])

res.to_csv(
        "level-1-channel-max-cell-count-channel-weighted-frequency_" + str(seed) + ".csv",
        index=False
    )
