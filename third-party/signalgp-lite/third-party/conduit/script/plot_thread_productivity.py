import sys
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from keyname import keyname as kn
from iterpop import iterpop as ip
import itertools as it
from slugify import slugify

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

df['Work Rate'] = df['Unit Productivity'] * df['Grid Size'] * df['Resistance']

for treatment, chunk in df.groupby('Treatment'):

    g = sns.FacetGrid(
      chunk,
      col='Work',
      col_order=sorted(chunk['Work'].unique()),
      margin_titles='true',
      sharey=True,
    )

    g.map(
      sns.barplot,
      'Threads',
      'Work Rate',
      order=sorted(chunk['Threads'].unique()),
    ).add_legend()

    plt.savefig(
      kn.pack({
        'title' : 'thread-update-rate',
        'Treatment' : slugify(treatment),
        'Synchronous' : str(synchronous),
        'ext' : '.png',
      }),
      transparent=True,
      dpi=300,
    )
