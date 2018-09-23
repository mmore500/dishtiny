import glob
import seaborn as sns
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import pandas as pd

res = []
for f in glob.glob("level-1-channel-dur-channel-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f)
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,df['ch1_dur'].max(),50000):
        if len(df[(df['seed'] == s) & (df['ch1_dur'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch1_dur' : d, 'channel_weighted_freq' : 0}, ignore_index=True)

fig, ax = plt.subplots()
# Set the scale of the x-and y-axis
ax.set(yscale="log")
ax.set(xlabel="Channel 1 Maximum Cell Count", ylabel="Log Per-Channel 1 Frequency")

sns.tsplot(df, time='ch1_dur', unit = "seed", value='channel_weighted_freq',ax=ax)

plt.show()
