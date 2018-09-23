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
for f in glob.glob("level-0-channel-dur-channel-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f)
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,int(df['ch0_dur'].max()),50000):
        if len(df[(df['seed'] == s) & (df['ch0_dur'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch0_dur' : d, 'channel_weighted_freq' : 0}, ignore_index=True)

fig, ax = plt.subplots()

# Set the scale of the y-axis
ax.set(yscale="log")

sns.tsplot(df, time='ch0_dur', unit = "seed", value='channel_weighted_freq',ax=ax)

ax.set(
        title="Duration of Level 1 Channels",
        xlabel="Updates",
        ylabel="Log Per-Channel Frequency"
    )

plt.savefig("level-0-channel-dur-channel-weighted-frequency.pdf",transparent=True)

print("Level 0 Channel-Weighted Duration")
print(
        np.average(df['ch0_dur'],weights=df['channel_weighted_freq'])
    )
