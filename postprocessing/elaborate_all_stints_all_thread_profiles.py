#!/usr/bin/env python3
"""Elaborates dataframe assembled from across all stints and all series.

Uploads output to a programatically-generated s3 url.

Usage:
    ./elaborate_all_stints_all_thread_profiles.py [bucket] [endeavor] [stint]
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
print( 'running elaborate_all_stints_all_thread_profiles.py'                   )
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
    Prefix=f'endeavor={endeavor}/thread-profiles/stage=7+what=collated/',
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
df['Endeavor'] = endeavor

################################################################################

df['Elapsed Generations'] = sum(
    df[col]
    for col in df.columns
    if 'Elapsed Generations Level ' in col
)

################################################################################

df['Elapsed Generations Delta'] = -df.sort_values('Stint').groupby([
    'Series',
    'proc',
    'thread',
])[
    'Elapsed Generations',
].diff(periods=-1)

df['Generations Per Update'] = df['Elapsed Generations Delta'] / df['Update']

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

out_prefix = f'endeavor={endeavor}/thread-profiles/stage=8+what=elaborated/'
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
