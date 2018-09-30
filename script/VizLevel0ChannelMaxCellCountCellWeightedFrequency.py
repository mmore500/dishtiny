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
for f in glob.glob("level-0-channel-max-cell-count-cell-weighted-frequency_*.csv"):
    dfin = pd.read_csv(f,comment='#')
    res.append(dfin)

df = pd.concat(res, ignore_index=True)

for s in df['seed'].unique():
    for d in range(0,int(df['ch0_max_cell_count'].max()),1):
        if len(df[(df['seed'] == s) & (df['ch0_max_cell_count'] == d)]) == 0:
            df = df.append({'seed' : s, 'ch0_max_cell_count' : d, 'cell_weighted_freq' : 0}, ignore_index=True)


ax = sns.tsplot(df, time='ch0_max_cell_count', unit = "seed", value='cell_weighted_freq')

ax.set(
        title="Maximum Cell Counts of Second-level Channels",
        xlabel="Cell Count",
        ylabel="Per-Cell Frequency"
    )
ax.ticklabel_format(axis='y',style='sci',scilimits=(0,0))


plt.savefig("level-0-channel-max-cell-count-cell-weighted-frequency.pdf",transparent=True)

def weighted_avg_and_std(values, weights):
    """
    Return the weighted average and standard deviation.

    values, weights -- Numpy ndarrays with the same shape.
    """
    average = np.average(values, weights=weights)
    # Fast and numerically precise:
    variance = np.average((values-average)**2, weights=weights)
    return (average, np.sqrt(variance))


print("Level 0 Cell-Weighted Mean Max Cell Count")
print(weighted_avg_and_std(df['ch0_max_cell_count'],df['cell_weighted_freq']))
