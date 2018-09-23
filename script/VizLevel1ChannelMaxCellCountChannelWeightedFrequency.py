import glob
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

# open-type fonts
matplotlib.rcParams['pdf.fonttype'] = 42

res = []
for f in glob.glob("level-1-channel-max-cell-count-channel-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f)
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,int(df['ch1_max_cell_count'].max()),10):
        if len(df[(df['seed'] == s) & (df['ch1_max_cell_count'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch1_max_cell_count' : d, 'channel_weighted_freq' : 0}, ignore_index=True)


fig, ax = plt.subplots()

# Set the scale of the y-axis
ax.set(yscale="log")

sns.tsplot(df, time='ch1_max_cell_count', unit = "seed", value='channel_weighted_freq',ax=ax,title="Maximum Cell Counts of Level 2 Channels",xlabel="Cell Count",ylabel="Log Per-Channel Frequency")

plt.savefig("level-1-channel-max-cell-count-channel-weighted-frequency.pdf",transparent=True)

print("Level 1 Channel-Weighted Mean Max Cell Count")
print(
        np.average(df['ch1_max_cell_count'],weights=df['channel_weighted_freq'])
    )
