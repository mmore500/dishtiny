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
        col='Execution',
        margin_titles="true",
        sharey=False,
    )


def draw_plots(measurement, df):

    facet(df).map(
        sns.barplot,
        'num cells',
        measurement,
        'Execution',
        hue_order=sorted(df['Execution'].unique()),
        palette=sns.color_palette(),
    )

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
            'num cells',
            measurement,
            'Execution',
            hue_order=sorted(df['Execution'].unique()),
            palette=sns.color_palette(),
            showfliers=showfliers,
        ).add_legend()

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

df = df.astype({ 'num cells' : int })

for measurement in [
        'Wall Nanoseconds',
        'CPU Nanoseconds',
        'Core-Nanoseconds per Cell Update',
    ]:
        draw_plots(measurement, df)
