# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from keyname import keyname as kn
from fileshash import fileshash as fsh
import numpy as np
import scipy as sp
from astropy.stats import bootstrap
from tqdm import tqdm

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

# replicability info
print({
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
})

print(
    "Area",
    "Wild Type mean / std:",
    np.mean(
        df[
            (df["Genotype"] == "Wild Type")
        ]["Area"]
    ),
    np.std(
        df[
            (df["Genotype"] == "Wild Type")
        ]["Area"]
    ),
)
print(
    "Area",
    "Messaging Knockout mean:",
    np.mean(
        df[
            (df["Genotype"] == "Messaging Knockout")
        ]["Area"]
    ),
    np.std(
        df[
            (df["Genotype"] == "Messaging Knockout")
        ]["Area"]
    ),
)

# do bootstrap statistics
bootsamples = 100000

boots = zip(
    bootstrap(
        np.array(
            df[
                (df["Genotype"] == "Wild Type")
            ]["Area"]
        ),
        bootsamples
    ),
    bootstrap(
        np.array(
            df[
                (df["Genotype"] == "Messaging Knockout")
            ]["Area"]
        ),
        bootsamples
    ),
)

bootstats = [
    np.mean(std_boot) - np.mean(control_boot)
    for std_boot, control_boot in tqdm(boots, total=bootsamples)
]

print(
    "Area",
    "p:",
    # divide by 100 b/c percentileofscore returns 0-100 and we want 0-1
    sp.stats.percentileofscore(bootstats, 0, 'rank')/100,
)


for key in 4, 5, 9,:
    print(
        key,
        "Wild Type mean / std:",
        np.mean(
            df[
                (df["Genotype"] == "Wild Type")
                & (df["Area"] == key)
            ]["Shape Factor"]
        ),
        np.std(
            df[
                (df["Genotype"] == "Wild Type")
                & (df["Area"] == key)
            ]["Shape Factor"]
        ),
    )
    print(
        key,
        "Messaging Knockout mean:",
        np.mean(
            df[
                (df["Genotype"] == "Messaging Knockout")
                & (df["Area"] == key)
            ]["Shape Factor"]
        ),
        np.std(
            df[
                (df["Genotype"] == "Messaging Knockout")
                & (df["Area"] == key)
            ]["Shape Factor"]
        ),
    )

    # do bootstrap statistics
    bootsamples = 1000000

    boots = zip(
        bootstrap(
            np.array(
                df[
                    (df["Genotype"] == "Wild Type")
                    & (df["Area"] == key)
                ]["Shape Factor"]
            ),
            bootsamples
        ),
        bootstrap(
            np.array(
                df[
                    (df["Genotype"] == "Messaging Knockout")
                    & (df["Area"] == key)
                ]["Shape Factor"]
            ),
            bootsamples
        ),
    )

    bootstats = [
        np.mean(std_boot) - np.mean(control_boot)
        for std_boot, control_boot in tqdm(boots, total=bootsamples)
    ]

    print(
        key,
        "p:",
        # divide by 100 b/c percentileofscore returns 0-100 and we want 0-1
        sp.stats.percentileofscore(bootstats, 0, 'rank')/100,
    )

# draw graphs
g = sns.barplot(
    data=df[(df['Area'] <= 9) & (df['Area'] > 3)],
    x="Area",
    y="Shape Factor",
    hue="Genotype",
)

g.set_xlabel("Level-zero Group Size")
g.set_ylabel("Shape Factor ($P/\\sqrt{A}$)")

g.set_ylim(ymin=0, ymax=8)

bak_map = dict()

for gt in df["Genotype"].unique():
    for a in df["Area"].unique():
        fil = df[(df["Genotype"] == gt) & (df["Area"] == a)]
        bak_map[fil["Shape Factor"].mean()] = len(fil["Shape Factor"])

def show_values_on_bars(axs, h_v="v", space=0.4):
    def _show_on_single_plot(ax):
        if h_v == "v":
            for p in ax.patches:
                _x = p.get_x() + p.get_width() / 2
                _y = p.get_y()
                value = bak_map[p.get_height()]
                ax.text(
                    _x,
                    _y,
                    "  $n=" + str(value) + "$",
                    color='w',
                    ha="center",
                    rotation=90
                )
        elif h_v == "h":
            for p in ax.patches:
                _x = p.get_x() + p.get_width() + float(space)
                _y = p.get_y() + p.get_height()
                value = int(p.get_width())
                ax.text(_x, _y, value, ha="left")

    if isinstance(axs, np.ndarray):
        for idx, ax in np.ndenumerate(axs):
            _show_on_single_plot(ax)
    else:
        _show_on_single_plot(axs)

show_values_on_bars(g)

outfile = kn.pack({
    'title' : 'group_perimeter_area',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : '.pdf',
})

plt.gcf().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0,
)

print('Output saved to', outfile)

plt.clf()

sns.barplot(
    data=df,
    x="Genotype",
    y="Area",
    hue="Genotype",
    dodge=False,
).get_legend().remove()

outfile = kn.pack({
    'title' : 'group_size',
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
    'ext' : '.pdf',
})

plt.gcf().savefig(
    outfile,
    transparent=True,
    bbox_inches='tight',
    pad_inches=0,
)

print('Output saved to', outfile)

plt.clf()
