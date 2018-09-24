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
for f in glob.glob("level-1-channel-dur-cell-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f)
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,int(df['ch1_dur'].max()),50000):
        if len(df[(df['seed'] == s) & (df['ch1_dur'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch1_dur' : d, 'cell_weighted_freq' : 0}, ignore_index=True)


ax = sns.tsplot(df, time='ch1_dur', unit = "seed", value='cell_weighted_freq')

ax.set(
        title="Duration of Level 2 Channels",
        xlabel="Updates",
        ylabel="Per-Cell-Update Frequency"
    )
ax.ticklabel_format(axis='y',style='sci')

plt.savefig("level-1-channel-dur-cell-weighted-frequency.pdf",transparent=True)

print("Level 1 Cell-Weighted Duration")
print(
        np.average(df['ch1_dur'],weights=df['cell_weighted_freq'])
    )
