import sys
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from keyname import keyname as kn
from iterpop import iterpop as ip
import itertools as it

try:
    __, synchronous = sys.argv
except:
    raise ValueError('specify "synchronous" as argument')

try:
    synchronous = int(synchronous)
except:
    raise ValueError('"synchronous" should be an integer')

assert synchronous in (0, 1), '"synchronous" should be 0 or 1'

counter = it.count()

treatments = {
    'Compute Lean' : {
        'Grid Size' : lambda work: work,
        'Resistance' : lambda work: 1,
        'Order' : next(counter),
    },
    'Compute Moderate' : {
        'Grid Size' : lambda work: work,
        'Resistance' : lambda work: 64,
        'Order' : next(counter),
    },
    'Compute Intensive' : {
        'Grid Size' : lambda work: work,
        'Resistance' : lambda work: 4096,
        'Order' : next(counter),
    },
    'Memory Lean' : {
        'Grid Size' : lambda work: 1,
        'Resistance' : lambda work: work,
        'Order' : next(counter),
    },
    'Memory Moderate' : {
        'Grid Size' : lambda work: 64,
        'Resistance' : lambda work: work,
        'Order' : next(counter),
    },
    'Memory Intensive' : {
        'Grid Size' : lambda work: 4096,
        'Resistance' : lambda work: work,
        'Order' : next(counter),
    },
}

# load and set up DataFrame
res = []
for treatment in treatments:
    df = pd.read_csv(kn.pack({
      'Synchronous' : str(synchronous),
      'Treatment' : treatment.replace(' ', '_'),
      'ext' : '.csv',
    }))
    df['Treatment'] = treatment
    df['Grid Size'] = df.apply(
        lambda row: treatments[treatment]['Grid Size'](row['Work']),
        axis=1,
    )
    df['Resistance'] = df.apply(
        lambda row: treatments[treatment]['Resistance'](row['Work']),
        axis=1,
    )
    res.append(df)

df = pd.concat(res)

df = df[df['Threads'] == 1]

df['Work Rate'] = df['Unit Productivity'] * df['Grid Size'] * df['Resistance']

g = sns.FacetGrid(
  df,
  col='Treatment',
  col_wrap=3,
  col_order=sorted(
    treatments.keys(),
    key=lambda treatment: treatments[treatment]['Order'],
  ),
  margin_titles='true',
  sharey=True,
)

g.map(
  sns.barplot,
  "Work",
  "Work Rate",
  order=sorted(df["Work"].unique()),
).add_legend()

plt.savefig(
  kn.pack({
    "title" : "tile-update-rate",
    "Synchronous" : str(synchronous),
    "ext" : ".png",
  }),
  transparent=True,
  dpi=300,
)
