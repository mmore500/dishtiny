# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

non_url_safe = ['"', '#', '$', '%', '&', '+',
                ',', '/', ':', ';', '=', '?',
                '@', '[', '\\', ']', '^', '`',
                '{', '|', '}', '~', "'"]

def slugify(text):
    """
    Turn the text content of a header into a slug for use in an ID
    """
    non_safe = [c for c in text if c in non_url_safe]
    if non_safe:
        for c in non_safe:
            text = text.replace(c, '')
    # Strip leading, trailing and multiple whitespace, convert remaining whitespace to _
    text = u'_'.join(text.split())
    return text

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

print("Data loaded!")

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'resource-even__channelsense-no__nlev-two__mute' : 'Blind',
    'resource-even__channelsense-no__nlev-two__mute__mixed' : 'Mixed',
    'resource-even__channelsense-yes__nlev-two' : 'Even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'Flat',
    'resource-wave__channelsense-yes__nlev-two' : 'Nested'
    }[raw]
)

df['Relationship Category'] = df.apply(
    lambda x: (
        'Neighbor' if 'Neighbor' in x['Relationship']
        else 'Channel' if 'mate' in x['Relationship']
        else 'Cell' if 'Cell' in x['Relationship']
        else 'Propagule' if 'Propagule' in x['Relationship']
        else 'Unknown'
    ),
    axis=1
)

print("Data crunched!")
for measure in df['Measure'].unique():

    manip = df.pivot_table(
        index=[
            'First Update', 'Last Update', 'Relationship',
            'Relationship Category', 'Seed', 'Treatment'
        ],
        columns='Measure',
        values='Value',
        aggfunc='first'
    ).reset_index()

    manip = manip[manip["Relationship"] != "Neighbor"]

    l0 = manip[
        manip["Relationship"].map(lambda x: " 1" not in x)
    ].copy()
    l0["Level"] = 0

    l1 = manip[
        manip["Relationship"].map(lambda x: " 0" not in x)
    ].copy()
    l1["Level"] = 1

    res = pd.concat([l0, l1])
    res["Relationship"] = res["Relationship"].map(
        lambda x: x.replace(
            " 0", ""
        ).replace(
            " 1", ""
        )
    )
    res = res[res["Relationship"] != "Nuclear Channelmate"]

    plt.clf()

    sns.catplot(
        data=res,
        x="Treatment",
        y=measure,
        hue="Relationship",
        order=[
            "Even",
            "Nested",
            "Flat",
        ],
        hue_order=[
            "Child Channelmate",
            "Parent Channelmate",
            "Non-nuclear Channelmate",
            "Unrelated Neighbor",
        ],
        col="Level",
        kind="bar",
        dodge=True,
    )

    outfile = kn.pack({
        'title' : slugify(measure),
        '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
        '_script_fullcat_hash' : fsh.FilesHash(
                                        file_parcel="full_parcel",
                                        files_join="cat_join"
                                    ).hash_files([sys.argv[0]]),
        '_source_hash' :kn.unpack(dataframe_filename)['_source_hash'],
        'ext' : ".pdf"
    })

    plt.gcf().savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0
    )

    print("Output saved to", outfile)
