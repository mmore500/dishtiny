#!/usr/bin/python3

import sys

import pandas as pd
import statsmodels.formula.api as smf
from scipy.stats import stats

import matplotlib.pyplot as plt
from matplotlib.offsetbox import AnchoredText
import seaborn as sns

import itertools as it
import numpy as np
from slugify import slugify
from keyname import keyname as kn
from iterpop import iterpop as ip

try:
    __, synchronous = sys.argv
except:
    raise ValueError('specify "synchronous" as argument')

try:
    synchronous = int(synchronous)
except:
    raise ValueError('"synchronous" should be an integer')

assert synchronous in (0, 1), '"synchronous" should be 0 or 1'

def estimate_latency(group):
  '''
  Use a linear regression to estimate time per work from a Pandas GroupBy.
  '''
  model = smf.glm(
    formula='Time ~ Work',
    data=group,
  ).fit()

  # pack up information about parameter estimates
  # so they can be programatically unpacked later
  decoder = {
    'Intercept' : 'Overhead',
    'Work' : 'Latency',
  }

  for spec, val in (
        (
            {
              'statistic' : statistic,
              'name' : decoder[parameter],
            },
            getter(parameter)
        ) for parameter in model.params.keys()
        for statistic, getter in (
          ('Estimated', lambda param: model.params[parameter]),
          ('Lower Bound', lambda param: model.conf_int()[0][param]),
          ('Upper Bound', lambda param: model.conf_int()[1][param]),
         )
    ):
    group['{statistic} {name}'.format(**spec)] = val

  return group

def _do_plot(df, x, y, ax=None):
  '''
  Wrap a call to Seaborn's regplot.
  '''
  res = sns.regplot(
    data=df,
    x=x,
    y=y,
    truncate=False,
    ax=ax
  )
  res.set_xlim(0, )
  res.set_ylim(0, )

  return res


def plot_regression(df, x, y, extra_names={}):
  '''Plot a regression with annotated statistics.'''
  # ugly hack to include origin in plot bounds
  plt.clf()
  ax = _do_plot(df, x, y)
  xlim, ylim = ax.get_xlim(), ax.get_ylim()
  ax.cla()
  ax.set_xlim(*xlim)
  ax.set_ylim(*ylim)
  _do_plot(df, x, y, ax=ax)

  # calculate some regression statistics...
  info = [
    ("{} = " + ("{}" if isinstance(v, int) else "{:.2f}")).format(k, v)
    for k, v in it.chain(zip(
      [
        'Slope',
        'Intercept',
        '$R^2$',
        '$p$',
        'Standard Error',
      ],
      stats.linregress(df[x],df[y]),
    ), [
      ('$n$', len(df))
    ])
  ]

  # ... and annotate regression statistics onto upper left
  at = AnchoredText(
    '\n'.join(info),
    frameon=True,
    loc='upper left',
  )
  ax.add_artist(at)

  # save to file
  # and assert df['Load'] is homogeneous
  plt.savefig(
    kn.pack({**{
      'x' : slugify(x),
      'y' : slugify(y),
      'synchronous' : str(synchronous),
      'ext': '.png',
    },**extra_names}),
    transparent=True,
    dpi=300,
  )


def deduce_properties(**kwargs):
  '''
  Deduce scaling properties we're trying to estimate from params computed
  via linear regression between Inverse Threads and Estimated Latency.
  '''
  res = {}

  res['Execution Seconds per Unit Work'] =  (
    kwargs['Intercept'] + kwargs['Q("Inverse Threads")']
  )
  res['Serial Fraction'] = (
    kwargs['Intercept'] / res['Execution Seconds per Unit Work']
  )
  res['Parallel Fraction'] = 1.0 - res['Serial Fraction']
  res['Threading Efficiency'] = (
    res['Serial Fraction'] / kwargs['Q("Inverse Threads")']
  )

  return res

def estimate_properties(model):
  '''
  Calculate estimates and bounds on scaling properties of interest
  from estimates and confidence intervals of params computed
  via linear regression between Inverse Threads and Estimated Latency
  '''
  # from DataFrame...
  #             0     1
  # Parameter1  val1  val2
  # Parameter2  val3  val4
  # ... to dict of lists
  # {
  #   Parameter1 : [val1, val2]
  #   Parameter2 : [val3, val4]
  # }
  conf_ints = {
      col : list(bounds)
      for col, bounds in model.conf_int().iterrows()
  }

  # take the product of dicts
  # use .items() to preserve ordering
  # adapted from http://stephantul.github.io/python/2019/07/20/product-dict/
  #
  # from to dict of lists...
  # {
  #   Parameter1 : [val1, val2]
  #   Parameter2 : [val3, val4]
  # }
  # ... to list of dicts (product of value lists)
  # [
  #   {
  #     Parameter1 : val1,
  #     Parameter2 : val3,
  #   },
  #   {
  #     Parameter1 : val1,
  #     Parameter2 : val4,
  #   },
  #   {
  #     Parameter1 : val2,
  #     Parameter2 : val3,
  #   },
  #   {
  #     Parameter1 : val2,
  #     Parameter2 : val4,
  #   },
  # ]
  keys, values = zip(*conf_ints.items())
  corners = [
    dict(zip(keys, bundle))
    for bundle in it.product(*values)
  ]
  assert(
    len(corners) == np.prod([len(val) for val in conf_ints.values()])
  )

  # from to dict of lists of raw parameter values...
  # [
  #   {
  #     Parameter1 : val1,
  #     Parameter2 : val3,
  #   },
  #   {
  #     Parameter1 : val1,
  #     Parameter2 : val4,
  #   },
  #   etc.
  # ]
  # ... to DataFrame of deduced parameters
  #    Parameter3   Parameter4   Parameter5
  # 0  val          val          val
  # 1  val          val          val
  # etc.
  extremes = pd.DataFrame([
    deduce_properties(**corner)
    for corner in corners
  ])

  # from DataFrame of deduced parameters...
  #    Parameter3   Parameter4   Parameter5
  # 0  val5         val6         val7
  # 1  val8         val9         val10
  # etc.
  # ... to DataFrame of deduced parameter bounds and estimates
  #    Parameter    Lower Bound   Upper Bound   Estimate
  # 0  Parameter3   val           val           val
  # 1  Parameter4   val           val           val
  # 2  Parameter5   val           val
  res = pd.DataFrame([
    {
      'Parameter' : param,
      'Lower Bound' : extremes[param].min(),
      'Upper Bound' : extremes[param].max(),
      'Estimate' : deduce_properties(
        **model.params
      )[param],
    } for param in extremes
  ])

  return res


# load and set up DataFrame
df = pd.read_csv(kn.pack({
  "synchronous" : str(synchronous),
  "ext" : ".csv",
}))
df['Inverse Threads'] = 1 / df['Threads']


# independently for each replicate, estimate overhead and latency
df = df.groupby([
  'Threads',
  'Replicate',
]).apply(
  estimate_latency,
)

# model relationship between latency (inverse throughput) and thread count
model = smf.glm(
  formula='Q("Estimated Latency") ~ Q("Inverse Threads")',
  data=df.drop_duplicates([
    'Threads',
    'Replicate',
  ]),
).fit()

# start a dataframe to collect estimates of scaling properties
res = estimate_properties(model)

# visualize regression
plot_regression(
  df.drop_duplicates([
    'Threads',
    'Replicate',
  ]),
  'Inverse Threads',
  'Estimated Latency',
)

# now, for posterity...
# estimate overhead and latency using /all/ replicates
df = df.groupby(
  'Threads',
).apply(
  estimate_latency,
)

# plot regressions used to estimate overhead and latency
for threads, chunk in df.groupby(
  'Threads',
):
  plot_regression(
    chunk,
    'Work',
    'Time',
    extra_names={'threads' : threads},
  )

# extract estimates of overhead and latency
res = res.append(pd.DataFrame([
    {
      'Parameter' : '{} @ {} Threads'.format(name, threads),
      'Lower Bound' : ip.pophomogeneous(chunk['Lower Bound ' + name]),
      'Upper Bound' : ip.pophomogeneous(chunk['Upper Bound ' + name]),
      'Estimate' : ip.pophomogeneous(chunk['Estimated ' + name]),
    } for threads, chunk in df.groupby('Threads')
    for name in ('Overhead', 'Latency')
]))

# consolidate and save computed estimates and bounds
res.sort_values([
  'Parameter',
]).to_csv(
  kn.pack({
    'title' : 'parameter_estimates',
    'synchronous' : str(synchronous),
    'ext' : '.csv',
  }),
  index=False,
)
