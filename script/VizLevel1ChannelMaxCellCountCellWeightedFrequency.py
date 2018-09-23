import glob
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import seaborn as sns
import pandas as pd

res = []
for f in glob.glob("level-1-channel-max-cell-count-cell-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f)
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,int(df['ch1_max_cell_count'].max()),10):
        if len(df[(df['seed'] == s) & (df['ch1_max_cell_count'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch1_max_cell_count' : d, 'cell_weighted_freq' : 0}, ignore_index=True)

fig, ax = plt.subplots()
# Set the scale of the x-and y-axis
ax.set(xlabel="Channel 1 Maximum Cell Count", ylabel="Log Per-Channel 1 Frequency")

sns.tsplot(df, time='ch1_max_cell_count', unit = "seed", value='cell_weighted_freq',ax=ax)

plt.savefig("level-1-channel-max-cell-count-cell-weighted-frequency.pdf")
