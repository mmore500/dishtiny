from iterpop import iterpop as ip
from keyname import keyname as kn
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
from slugify import slugify
import subprocess
import sys

def facet(df):
    return sns.FacetGrid(
        df,
        col='Task',
        margin_titles="true",
        sharey=False,
    )


def draw_plots(measurement, df):

    facet(df).map(
        sns.barplot,
        'Num Cells',
        measurement,
        'Num Threads',
        hue_order=sorted(df['Num Threads'].unique()),
        palette=sns.color_palette(),
    ).add_legend().legend.set_title("Num Threads")

    plt.savefig(
      kn.pack({
        'measurement' : slugify(measurement),
        'ext' : '.png',
      }),
      transparent=True,
      dpi=300,
    )

    plt.clf()

    for showfliers in True, False:
        facet(df).map(
            sns.boxplot,
            'Num Cells',
            measurement,
            'Num Threads',
            hue_order=sorted(df['Num Threads'].unique()),
            palette=sns.color_palette(),
            showfliers=showfliers,
        ).add_legend().legend.set_title("Num Threads")

        plt.savefig(
          kn.pack({
            'fliers' : showfliers,
            'measurement' : slugify(measurement),
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

df = df.astype({ 'Num Cells' : int })

for measurement in [
        'Wall Nanoseconds',
        'CPU Nanoseconds',
        'Wall Core-Nanoseconds per Cell Update',
        'CPU Core-Nanoseconds per Cell Update',
    ]:
        draw_plots(measurement, df)
