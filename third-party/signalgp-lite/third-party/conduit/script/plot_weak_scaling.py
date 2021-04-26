import sys
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from keyname import keyname as kn

try:
    __, synchronous = sys.argv
except:
    raise ValueError('specify "synchronous" as argument')

try:
    synchronous = int(synchronous)
except:
    raise ValueError('"synchronous" should be an integer')

assert synchronous in (0, 1), '"synchronous" should be 0 or 1'

# load and set up DataFrame
actual = pd.read_csv(kn.pack({
  "synchronous" : str(synchronous),
  "ext" : ".csv",
}))
actual["Type"] = "Actual"

# get ready to calculate ideal timings in terms actual timings
# with thread count 1
ideal = actual.copy()
ideal["Type"] = "Ideal"

# helper functions
get_baseline_idx = lambda row: actual[
    (actual["Load"] == row["Load"])
    & (actual["Replicate"] == row["Replicate"])
]["Threads"].idxmin()

get_baseline_time = lambda row: actual.iloc[
    get_baseline_idx(row)
]["Time"]

# calculate ideal time in terms of lowest-threaded observation
ideal["Time"] = ideal.apply(
    lambda row: get_baseline_time(row),
    axis=1,
)

# drop thread counts from ideal that were defined identically to actual
ideal = ideal.drop(ideal.index[
    ideal.apply(
        lambda row: row["Threads"] == ideal[
            ideal["Load"] == row["Load"]
        ]["Threads"].min(),
        axis=1,
    ),
])

# merge in newly-calculated ideal timings
df = pd.concat([ideal, actual])

# draw plot
g = sns.FacetGrid(
  df,
  col="Load",
  col_wrap=3,
  margin_titles="true",
  sharey=False,
)

g.map(
  sns.barplot,
  "Threads",
  "Time",
  "Type",
  hue_order=sorted(df["Type"].unique()),
  order=sorted(df["Threads"].unique()),
  palette=sns.color_palette(),
).add_legend()

plt.savefig(
  kn.pack({
    "title" : "weak-scaling",
    "synchronous" : str(synchronous),
    "ext" : ".png",
  }),
  transparent=True,
  dpi=300,
)
