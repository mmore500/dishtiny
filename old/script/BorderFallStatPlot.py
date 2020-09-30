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

# updates ==> how many reproduction cycles
df['Border Age'] /= 8
df['Cell Age'] /= 8

df["Internal Channel Change"] = (
    df["Channel Change"]
    & ~df["External Channel Change"]
)

df["Internal"] = ~df["External"]

print(df.corr())

# calculate separately
df_mean = df.groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_swap = df[df["Channel Swap"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_antiswap = df[~df["Channel Swap"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_change = df[df["Channel Change"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_antichange = df[~df["Channel Change"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_extchange = df[df["External Channel Change"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_antiextchange = df[~df["External Channel Change"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_intchange = df[df["Internal Channel Change"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_antiintchange = df[~df["Internal Channel Change"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_external = df[df["External"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_mean_internal = df[~df["External"]].groupby([
    'Genotype',
    'Seed',
]).mean().reset_index().rename({
    "Border Age" : "Mean Border Age",
    "Cold" : "Fraction Cold Borders",
}, axis=1)

df_agg = df.groupby([
    'Genotype',
    'Seed',
]).agg(
    lambda x: sum(1 for v in x if v < 1) / len(x)
).reset_index().rename({
    "Border Age" : "Border Turnover",
    "Cell Age" : "Cell Turnover",
}, axis=1)


# combine
df_res = df_mean

df_res["Border Turnover"] = df_agg["Border Turnover"]
df_res["Cell Turnover"] = df_agg["Cell Turnover"]

# border turnover -> cell birth rate -> expected border age
# see math in lab notes
df_res["Expected Border Age"] = (
    1 / (
        (2 - df_res["Cell Turnover"]) * df_res["Cell Turnover"]
    )
)

df_res["Scaled Mean Border Age"] = (
        df_res["Mean Border Age"] / df_res["Expected Border Age"]
    )

# replicability info
print({
    '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
    '_script_fullcat_hash' : fsh.FilesHash(
                                file_parcel="full_parcel",
                                files_join="cat_join"
                            ).hash_files([sys.argv[0]]),
    '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
})
for key in (
    "Mean Border Age",
    "Channel Swap",
    "Channel Change",
    "External Channel Change",
    "Internal Channel Change",
    "External",
    "Internal",
    ):
    print(
        key,
        "Wild Type mean / std:",
        np.mean(
            df_res[
                df_res["Genotype"] == "Wild Type"
            ][key]
        ),
        np.std(
            df_res[
                df_res["Genotype"] == "Wild Type"
            ][key]
        ),
    )
    print(
        key,
        "Messaging Knockout mean:",
        np.mean(
            df_res[
                df_res["Genotype"] == "Messaging Knockout"
            ][key]
        ),
        np.std(
            df_res[
                df_res["Genotype"] == "Messaging Knockout"
            ][key]
        ),
    )

    # do bootstrap statistics
    bootsamples = 1000000

    boots = zip(
        bootstrap(
            np.array(
                df_res[
                    df_res["Genotype"] == "Wild Type"
                ][key]
            ),
            bootsamples
        ),
        bootstrap(
            np.array(
                df_res[
                    df_res["Genotype"] == "Messaging Knockout"
                ][key]
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

for key in (
    "Mean Border Age",
    ):
    print(
        key,
        "Wild Type mean / std:",
        np.mean(
            df_mean_swap[
                (df_mean_swap["Genotype"] == "Wild Type")
            ][key]
        ),
        np.std(
            df_mean_swap[
                (df_mean_swap["Genotype"] == "Wild Type")
            ][key]
        ),
    )
    print(
        key,
        "Messaging Knockout mean:",
        np.mean(
            df_mean_swap[
                (df_mean_swap["Genotype"] == "Messaging Knockout")
            ][key]
        ),
        np.std(
            df_mean_swap[
                (df_mean_swap["Genotype"] == "Messaging Knockout")
            ][key]
        ),
    )

    # do bootstrap statistics
    bootsamples = 1000000

    boots = zip(
        bootstrap(
            np.array(
                df_mean_external[
                    df_mean_external["Genotype"] == "Wild Type"
                ][key]
            ),
            bootsamples
        ),
        bootstrap(
            np.array(
                df_mean_external[
                    df_mean_external["Genotype"] == "Messaging Knockout"
                ][key]
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
sns.barplot(
    data=df_mean_swap,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'borderswap',
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
    data=df_mean_antiswap,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'antiswapborderage',
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
    data=df_mean_change,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'borderchange',
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
    data=df_mean_antichange,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'antichangeborderage',
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
    data=df_mean_extchange,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'borderextchange',
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
    data=df_mean_intchange,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'borderintchange',
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
    data=df_mean_antiintchange,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'antiintchangeborderage',
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

df_mean_external["Aggressor"] = "Neighbor"
df_mean_internal["Aggressor"] = "Affiliate"

sns.barplot(
    data=df_mean_external.append(df_mean_internal),
    x="Aggressor",
    y="Mean Border Age",
    hue="Genotype",
    hue_order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'comboborderage',
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
    data=df_mean_external,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'externalborderage',
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
    data=df_mean_internal,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'internalborderage',
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
    data=df_mean,
    x="Genotype",
    y="Mean Border Age",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'nofilterborderage',
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
    data=df_res,
    x="Genotype",
    y="Channel Swap",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'channelswap',
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
    data=df_res,
    x="Genotype",
    y="Channel Change",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'channelchange',
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
    data=df_res,
    x="Genotype",
    y="External Channel Change",
    order=['Wild Type', 'Messaging Knockout'],
)

outfile = kn.pack({
    'title' : 'channelextchange',
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
