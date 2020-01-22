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
    'resource-even__channelsense-yes__nlev-onebig' : 'Flat-Even',
    'resource-even__channelsense-yes__nlev-two' : 'Nested-Even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'Flat-Wave',
    'resource-wave__channelsense-yes__nlev-two' : 'Nested-Wave',
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

    plt.clf()

    ax = sns.barplot(
        data=df.pivot_table(
            index=[
                'First Update', 'Last Update', 'Relationship',
                'Relationship Category', 'Seed', 'Treatment'
            ],
            columns='Measure',
            values='Value',
            aggfunc='first'
        ).reset_index(),
        x="Relationship",
        y=measure,
        hue="Treatment",
        order=[
            'Neighbor',
            'Related Neighbor',
            'Unrelated Neighbor',
            ] + sorted([
                r for r in df['Relationship'].unique() if 'Channelmate' in r
            ]) + [
            'Nonchannelmate',
            'Cell Child',
            'Cell Parent',
            'Propagule Child',
            'Propagule Parent',
        ],
    )

    for label in ax.get_xticklabels():
        label.set_rotation(-90)

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

    plt.gcf().set_size_inches(12, 5)

    plt.gcf().savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0
    )

    print("Output saved to", outfile)
