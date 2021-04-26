import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from keyname import keyname as kn
from slugify import slugify
import subprocess
import sys
from iterpop import iterpop as ip

def facet(df):
    return sns.FacetGrid(
        df,
        col='Overloaded',
        row='Mesh',
        margin_titles="true",
        sharex=False,
    )


def draw_plots(measurement, df):

    facet(df).map(
        sns.barplot,
        'Threads',
        measurement,
        'Implementation',
        hue_order=sorted(df['Implementation'].unique()),
        palette=sns.color_palette(),
    ).add_legend()

    plt.savefig(
      kn.pack({
        'measurement' : slugify(measurement),
        'time_type' : ip.pophomogeneous(df['time_type']),
        'ext' : '.png',
      }),
      transparent=True,
      dpi=300,
    )

    plt.clf()

    for showfliers in True, False:
        facet(df).map(
            sns.boxplot,
            'Threads',
            measurement,
            'Implementation',
            hue_order=sorted(df['Implementation'].unique()),
            palette=sns.color_palette(),
            showfliers=showfliers,
        ).add_legend()

        plt.savefig(
          kn.pack({
            'fliers' : showfliers,
            'measurement' : slugify(measurement),
            'time_type' : ip.pophomogeneous(df['time_type']),
            'ext' : '.png',
          }),
          transparent=True,
          dpi=300,
        )

        plt.clf()

__, filename = sys.argv

nproc = int(subprocess.check_output(['nproc']))

df = pd.read_csv(filename)

for k, v in kn.unpack(filename).items():
    df[k] = v

df['Overloaded'] = (df['Threads'] > nproc)

for measurement in [
        'Wall Nanoseconds',
        'CPU Nanoseconds',
        'Latency',
        'Lossiness',
    ]:
        draw_plots(measurement, df)
