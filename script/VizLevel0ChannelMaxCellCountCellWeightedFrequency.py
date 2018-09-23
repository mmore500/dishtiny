import glob
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import seaborn as sns
import pandas as pd

# open-type fonts
matplotlib.rcParams['pdf.fonttype'] = 42

res = []
for f in glob.glob("level-0-channel-max-cell-count-cell-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f)
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,int(df['ch0_max_cell_count'].max()),1):
        if len(df[(df['seed'] == s) & (df['ch0_max_cell_count'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch0_max_cell_count' : d, 'cell_weighted_freq' : 0}, ignore_index=True)


sns.tsplot(df, time='ch0_max_cell_count', unit = "seed", value='cell_weighted_freq',title="Maximum Cell Counts of Level 2 Channels",xlabel="Cell Count",ylabel="Per-Cell Frequency")

plt.savefig("level-0-channel-max-cell-count-cell-weighted-frequency.pdf",transparent=True)
