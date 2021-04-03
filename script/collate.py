#!/usr/bin/env python3

import boto3
from io import StringIO
from iterdub import iterdub as ib
import itertools
from keyname import keyname as kn
import multiprocessing as mp
import os
import pandas as pd
import re
import sys
import tempfile
from tqdm.contrib.concurrent import process_map
from math import gcd
from functools import reduce

################################################################################
print(                      )
print( 'running collate.py' )
print( '------------------' )
################################################################################

try:
    prefix_url, regex = sys.argv[1:]
except:
    print('bad arguments')
    print('USAGE: [prefix_url] [regex]')
    sys.exit(1)

bucket = re.search('s3://(.+?)/', prefix_url).group(1)
prefix = re.search(f's3://{bucket}/(.+)', prefix_url).group(1)

print(f'prefix_url {prefix_url}')
print(f'regex {regex}')
print(f'bucket {bucket}')
print(f'prefix {prefix}')

assert prefix.count('stage=') == 1 and prefix.count('what=') == 1
assert any(
    'stage' in kn.unpack(segment) and 'what' in kn.unpack(segment)
    for segment in prefix.split('/')
)

stages, = [
    list(kn.unpack(segment)['stage'].split('~'))
    for segment in prefix.split('/')
    if 'stage' in kn.unpack(segment) and 'what' in kn.unpack(segment)
]

print(f'stages {stages}')

prefixes = [ '/'.join(
    kn.pack({
        **kn.unpack(segment),
        **{ 'stage' : stage, },
    })
    if 'stage' in kn.unpack(segment) and 'what' in kn.unpack(segment)
    else segment
    for segment in prefix.split('/')
) for stage in stages ]

print(f'prefixes {prefixes}')

################################################################################
print(                             )
print( 'grepping for source files' )
print( '-------------------------' )
################################################################################

client = boto3.client('s3', region_name='us-west-2',)

pattern = re.compile('.*' + regex + '$')

matches = [
    key['Key']
    for prefix in prefixes
    for page in client.get_paginator('list_objects').paginate(
        Bucket=bucket,
        Prefix=prefix,
    )
    for key in page['Contents']
    if pattern.match(key['Key'])
]

print(f'{len(matches)} matching source files')
print(f'for example, s3://{bucket}/{matches[0]}')

if not matches:
    print('no matches detected')
    exit(1)

################################################################################
print(                         )
print( 'computing output path' )
print( '---------------- ----' )
################################################################################

out_prefix = '/'.join(
    kn.pack({
        **kn.unpack(segment),
        **{
            'stage' : max(
                map(int, stages),
            ) + 1,
            'what' : 'collated',
        },
    })
    if 'stage' in kn.unpack(segment) and 'what' in kn.unpack(segment)
    else segment
    for segment in prefix.split('/')
)

common_keys = set.intersection(*[
    set( kn.unpack(match).keys() )
    for match in matches
])

out_filename = kn.pack({
    **{
        key : ib.dub(
            kn.unpack(match)[key]
            for match in matches
        )
        for key in common_keys
    },
    **{ 'ext' : '.csv.xz' },
})


out_path = (out_prefix + '/' + out_filename).replace('//', '/')

print(f'upload path will be s3://{bucket}/{out_path}')

################################################################################
print(                                 )
print( 'downloading and concatenating' )
print( '-----------------------------' )
################################################################################

def getter(match):
    return pd.read_csv(f's3://{bucket}/{match}')

df = pd.concat(process_map(
        getter,
        matches,
        chunksize=10,
        max_workers=mp.cpu_count(),
))

################################################################################
print(                         )
print( 'dumping and uploading' )
print( '---------------------' )
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
        client.upload_fileobj(
            f,
            bucket,
            out_path,
        )

################################################################################
print(                          )
print( 'consolidation complete' )
print( '----------------------' )
################################################################################
