from collections import defaultdict
from tqdm import tqdm
import pandas as pd
import sys

# preliminaries

seed = None

BIN_SIZE = 1

assert len(sys.argv) == 2, \
    "First argument is filename."

counter = defaultdict(int)

# import data (files created from CleanChannelMap.py)
df = pd.read_csv(sys.argv[1])

seed = df.iloc[0]['seed']

cell_update_counts_by_ch0 = df.groupby('channel0')['count'].sum()

max_cell_counts = df.groupby(['channel0','update'])['count'].sum().groupby('channel0').max()


for ch0, c in tqdm(max_cell_counts.iteritems()):
    counter[(c // BIN_SIZE)*BIN_SIZE] += cell_update_counts_by_ch0[ch0]

res = pd.DataFrame([{'ch0_max_cell_count' : k, 'cell_weighted_freq' : v, 'seed' : seed} for k,v in counter.items()])

res.to_csv(
        "level-0-channel-max-cell-count-cell-weighted-frequency_" + str(int(seed)) + ".csv",
        index=False
    )
