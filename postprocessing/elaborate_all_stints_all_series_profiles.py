#!/usr/bin/env python3
"""Elaborates dataframe assembled from across all stints and all series.

Uploads output to a programatically-generated s3 url.

Usage:
    ./elaborate_all_stints_all_series_profiles.py [bucket] [endeavor] [stint]
"""

import boto3
from functools import reduce
from iterdub import iterdub as ib
from iterpop import iterpop as ip
from keyname import keyname as kn
import os
import pandas as pd
import sys
import tempfile

################################################################################
print(                                                                         )
print( 'running elaborate_all_stints_all_series_profiles.py'                   )
print( '---------------------------------------------------------------------' )
################################################################################

try:
    bucket = sys.argv[1]
    endeavor, = map(int, sys.argv[2:])
except:
    print(__doc__)
    sys.exit(1)

print(f'bucket {bucket}')
print(f'endeavor {endeavor}')

################################################################################
print(                                                                         )
print( 'getting data'                                                          )
print( '---------------------------------------------------------------------' )
################################################################################

s3_handle = boto3.resource('s3')
bucket_handle = s3_handle.Bucket(bucket)

data_url, = bucket_handle.objects.filter(
    Prefix=f'endeavor={endeavor}/series-profiles/stage=7+what=collated/',
)

df = pd.read_csv(
    f's3://{bucket}/{data_url.key}',
)

################################################################################
print(                                                                         )
print( 'elaborating data'                                                      )
print( '---------------------------------------------------------------------' )
################################################################################

df['Bucket'] = bucket

################################################################################

df['Fitness Complexity'] = df['Estimated True Advantageous Sites']

for target in [
    'Number Unique Module Expression Profiles',
    'Number Unique Module Regulation Profiles',
    'Num Instructions Executed per Live Cardinal-update',
    'Mean Resource Received Per Cell',
    'Resource Receiving Cell Fraction',
    'Fraction Deaths apoptosis',
    'Fraction Deaths elimination',
    'Nulliparous Fraction',
    'Mean Kin Group Size Level 0',
    'Mean Kin Group Size Level 1',
]:
    df[target] = df[target + ' (monoculture mean)']

################################################################################

mutation_str = ip.pophomogeneous( df['MUTATION_RATE'].dropna() )
mutation_items = mutation_str.split()
assert mutation_items[0] == '[' and mutation_items[-1] == ']'

mutation_rates = list(map( float, mutation_items[1:-1] ))
nlev = len(mutation_rates) - 1

df['Expected Mutations'] = sum(
    df[f'Elapsed Generations Level {lev}'] * rate
    for lev, rate in enumerate(mutation_rates)
)
df['Mutation Accumulation'] = df['Elapsed Mutations'] / df['Expected Mutations']

################################################################################

df['Elapsed Generations'] = sum(
    df[f'Elapsed Generations Level {lev}']
    for lev in range(nlev + 1)
)
df['Selection Pressure'] = (
    df['Expected Mutations'] - df['Elapsed Mutations']
) / df['Expected Mutations']

################################################################################

for lev in range(nlev + 1):
    df[f'% Generations Level {lev}'] = (
        df[f'Elapsed Generations Level {lev}']
        / df['Elapsed Generations']
    )

################################################################################

df['Mean Mutations per Generation'] = (
    df['Expected Mutations'] / df['Elapsed Generations']
)

################################################################################

df['Fitness Complexity Delta'] = -df.sort_values('Stint').groupby('Series')[
    'Estimated True Advantageous Sites'
].diff(periods=-1)
df['Fitness Complexity Delta %'] = (
    df['Fitness Complexity Delta'] / df['Fitness Complexity']
)
df['Fitness Complexity Delta Sign'] = df['Fitness Complexity Delta'] > 0

################################################################################

df['Phenotype Complexity Delta'] = -df.sort_values('Stint').groupby('Series')[
    'Phenotype Complexity',
].diff(periods=-1)
df['Phenotype Complexity Delta %'] = (
    df['Phenotype Complexity Delta'] / df['Phenotype Complexity']
)
df['Phenotype Complexity Delta Sign'] = df['Phenotype Complexity Delta'] > 0

################################################################################

frac_won = lambda row: row['Fraction Predecessor Battles Won']
df['Predecessor Battle Outcome'] = df.apply(
    lambda row: (
        'less fit than predecessor' if frac_won(row) < 0.15
        else 'more fit than predecessor' if frac_won(row) > 0.85
        else 'indeterminate'
    ),
    axis=1,
)

################################################################################

df['Neutral Phenotypic Variation Under Mutation'] = (
    df['Fraction Phenotypically-Expressed Mutations']
    - df['Fraction Mutations that are Deleterious']
)
# disabled because data wasn't collected
# df['Neutral Phenotypic Variation Under Deletion Mutation'] = (
#     df['Fraction Phenotypically-Expressed Deletion Mutations']
#     - df['Fraction Deletion Mutations that are Deleterious']
# )
# df['Neutral Phenotypic Variation Under Insertion Mutation'] = (
#     df['Fraction Phenotypically-Expressed Insertion Mutations']
#     - df['Fraction Insertion Mutations that are Deleterious']
# )
# df['Neutral Phenotypic Variation Under Point Mutation'] = (
#     df['Fraction Phenotypically-Expressed Point Mutations']
#     - df['Fraction Point Mutations that are Deleterious']
# )

################################################################################

df['Fraction Phenotypic Variation That Is Neutral Under Mutation'] = (
    df['Neutral Phenotypic Variation Under Mutation']
    / df['Fraction Phenotypically-Expressed Mutations']
)
# disabled because data wasn't collected
# df['Fraction Phenotypic Variation That Is Neutral Under Deletion Mutation'] =(
#     df['Neutral Phenotypic Variation Under Deletion Mutation']
#     / df['Fraction Phenotypically-Expressed Deletion Mutations']
# )
# df['Fraction Phenotypic Variation That Is Neutral Under Insertion Mutation']=(
#     df['Neutral Phenotypic Variation Under Insertion Mutation']
#     / df['Fraction Phenotypically-Expressed Insertion Mutations']
# )
# df['Fraction Phenotypic Variation That Is Neutral Under Point Mutation'] = (
#     df['Neutral Phenotypic Variation Under Point Mutation']
#     / df['Fraction Phenotypically-Expressed Point Mutations']
# )

################################################################################

df['Num Less Fit Under State Exchanges'] = (
    df['Num Less Fit Under Extrospective State Exchanges']
    + df['Num Less Fit Under Introspective State Exchanges']
    + df['Num Less Fit Under Writable State Exchanges']
)
df['Num Less Fit Under State Rotates'] = (
    df['Num Less Fit Under Extrospective State Rotates']
    + df['Num Less Fit Under Introspective State Rotates']
    + df['Num Less Fit Under Writable State Rotates']
)

# either exchange or rotate
df['Num Less Fit Under State Perturbation'] = (
    df['Num Less Fit Under Extrospective State Perturbation']
    + df['Num Less Fit Under Introspective State Perturbation']
    + df['Num Less Fit Under Writable State Perturbation']
)

################################################################################

df['Cardinal Interface Complexity'] = (
    df['Num Less Fit Under State Perturbation']
    + df['Num Less Fit Under Inter Self-Send @ Filter Mod 20']
    + df['Num Less Fit Under Intra Self-Send @ Filter Mod 20']
)

df['Cell Interface Complexity'] = (
    df['Num Less Fit Under State Perturbation']
    + df['Num Less Fit Under Inter Self-Send @ Filter Mod 20']
)

################################################################################

valid_rows = (
    df['Parent-Eliminated Kin ID Commonality=0+Statistic=Frequency+What=birth (monoculture)'].notna()
    | df['Parent-Eliminated Kin ID Commonality=1+Statistic=Frequency+What=birth (monoculture)'].notna()
    | df['Parent-Eliminated Kin ID Commonality=2+Statistic=Frequency+What=birth (monoculture)'].notna()
)

df[
    'Parent-Eliminated Kin ID Commonality=0+Statistic=Frequency+What=birth (monoculture)'
][ valid_rows ].fillna(
    0,
    inplace=True,
)
df[
    'Parent-Eliminated Kin ID Commonality=1+Statistic=Frequency+What=birth (monoculture)'
][ valid_rows ].fillna(
    0,
    inplace=True,
)
df[
    'Parent-Eliminated Kin ID Commonality=2+Statistic=Frequency+What=birth (monoculture)'
][ valid_rows ].fillna(
    0,
    inplace=True,
)

################################################################################

df['Fraction Neighbors Exactly Kin Level 1 (monoculture mean)'] = (
    df['Fraction Neighbors Kin Level 1 (monoculture mean)']
    - df['Fraction Neighbors Kin Level 0 (monoculture mean)']
)

df['Parent-Eliminated Kin ID Commonality>=1+Statistic=Frequency+What=birth (monoculture)'] = (
    1
    - df['Parent-Eliminated Kin ID Commonality=0+Statistic=Frequency+What=birth (monoculture)']
)

df['Birth Conflict Ratio for Kin Commonality At Least 1'] = (
    df['Parent-Eliminated Kin ID Commonality>=1+Statistic=Frequency+What=birth (monoculture)']
    / df['Fraction Neighbors Kin Level 1 (monoculture mean)']
)

df['Birth Conflict Ratio for Kin Commonality Exactly 1'] = (
    df['Parent-Eliminated Kin ID Commonality=1+Statistic=Frequency+What=birth (monoculture)']
    / df['Fraction Neighbors Exactly Kin Level 1 (monoculture mean)']
)

df['Birth Conflict Ratio for Kin Commonality Exactly 2'] = (
    df['Parent-Eliminated Kin ID Commonality=2+Statistic=Frequency+What=birth (monoculture)']
    / df['Fraction Neighbors Kin Level 0 (monoculture mean)']
)

################################################################################

df['Stint Decade'] = df['Stint'] // 10

################################################################################

df['Critical Fitness Complexity'] = df['Fitness Complexity']
df['Interpolated Fitness Complexity'] = df['a=most_credible_idx+set_size=1']
df['Composite Fitness Complexity'] \
    = df['Critical Fitness Complexity'] + df['Interpolated Fitness Complexity']
df['Composite Fitness Complexity Lower Bound'] \
    = df['Critical Fitness Complexity'] \
    + df['a=credible_interval_lower_bound_idx+set_size=1']
df['Composite Fitness Complexity Upper Bound'] \
    = df['Critical Fitness Complexity'] \
    + df['a=credible_interval_upper_bound_idx+set_size=1']

################################################################################

df['More Fit than Predecessor'] \
    = df['Predecessor Battle Outcome'] == 'more fit than predecessor'
df['Less Fit than Predecessor'] \
    = df['Predecessor Battle Outcome'] == 'less fit than predecessor'
df['Predecessor Fitness Differentiation'] \
    = df['More Fit than Predecessor'] | df['More Fit than Predecessor']
df['Predecessor Fitness Neutrality'] \
    = ~df['Predecessor Fitness Differentiation']

################################################################################
print(                                                                         )
print( 'calculating upload path'                                               )
print( '---------------------------------------------------------------------' )
################################################################################

# common_keys = set.intersection(*[
#     set( kn.unpack(source).keys() )
#     for source in sources
# ])

out_filename = kn.pack(
    kn.unpack(data_url.key),
)

out_prefix = f'endeavor={endeavor}/series-profiles/stage=8+what=elaborated/'
out_path = out_prefix + out_filename

print(f'upload path will be s3://{bucket}/{out_path}')

################################################################################
print(                                                                         )
print( 'dumping and uploading'                                                 )
print( '---------------------------------------------------------------------' )
################################################################################

# have to work with filename or pandas compression doesn't work
with tempfile.TemporaryDirectory() as tmp:

    temp_path = os.path.join(tmp, 'data.xz')
    print(f'temp path is {temp_path}')


    df.to_csv(
        temp_path,
        index=False,
        compression='xz',
    )

    with open(temp_path, 'rb') as f:
        client = boto3.client('s3')
        client.upload_fileobj(
            f,
            bucket,
            out_path,
        )

################################################################################
print(                                                                         )
print( 'elaboration complete'                                                  )
print( '---------------------------------------------------------------------' )
################################################################################
