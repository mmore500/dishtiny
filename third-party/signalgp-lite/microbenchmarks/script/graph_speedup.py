import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from keyname import keyname as kn
from slugify import slugify
import subprocess
import sys
from iterpop import iterpop as ip
import numpy as np

__, filename = sys.argv

nproc = int(subprocess.check_output(['nproc']))

df = pd.read_csv(filename)

for k, v in kn.unpack(filename).items():
    df[k] = v

df = df.astype({ 'num agents' : int })

df = df.groupby([
    'Library',
    'Implementation',
    'num agents',
]).mean().reset_index()

vanilla = df[df['Implementation'] == 'vanilla']
lite = df[df['Implementation'] == 'lite']

ratio = pd.DataFrame()
ratio['Library'] = vanilla['Library']
ratio['num agents'] = vanilla['num agents']

ratio['Wall Speedup'] = (
    np.array( vanilla['Wall Nanoseconds'] )
    / np.array( lite['Wall Nanoseconds'] )
)

ratio['CPU Speedup'] = (
    np.array( vanilla['CPU Nanoseconds'] )
    / np.array( lite['CPU Nanoseconds'] )
)

sns.barplot(
    data=ratio,
    x='num agents',
    y='Wall Speedup',
    hue='Library',
)

plt.savefig(
  kn.pack({
    'measurement' : slugify('Wall Time Speedup'),
    'ext' : '.png',
  }),
  transparent=True,
  dpi=300,
)

plt.clf()

sns.barplot(
    data=ratio,
    x='num agents',
    y='CPU Speedup',
    hue='Library',
)

plt.savefig(
  kn.pack({
    'measurement' : slugify('CPU Time Speedup'),
    'ext' : '.png',
  }),
  transparent=True,
  dpi=300,
)

plt.clf()
