#!/usr/bin/env python3
"""Collates data files from prefix_url on s3 matching regex.

Uploads collated output to a programatically-generated s3 url.

Usage:
    ./collate.py [prefix_url] [regex]
"""

import boto3
from functools import reduce
from io import StringIO
from iterdub import iterdub as ib
import itertools
from keyname import keyname as kn
import math
import multiprocessing as mp
import os
import pandas as pd
import re
import sys
import tempfile
from tqdm.contrib.concurrent import process_map

################################################################################
print(                      )
print( 'running collate.py' )
print( '------------------' )
################################################################################

try:
    prefix_url, regex = sys.argv[1:]
except:
    print(__doc__)
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
    res = pd.read_csv(f's3://{bucket}/{match}')

    num_nans = res.isna().sum().sum()
    num_cells = math.prod(map(len, res.axes))
    if num_nans * 2 > num_cells:
        print(f'WARNING: {match} is more than half nans')

    # print(match, len(res.index))

    return res

df = pd.concat(process_map(
        getter,
        matches,
        chunksize=10,
        max_workers=mp.cpu_count(),
))

print(f'concatenated dataframe has {len(df.index)} rows')
print(f'concatenated dataframe has {len(df.columns)} columns')

################################################################################
print(                                        )
print( 'deleting stale collated data, if any' )
print( '------------------------------------' )
################################################################################

pattern = re.compile(
    '.*/' # anything leading up to final /
    '[^\/]*' # anything EXCEPT /
    f'a={kn.unpack(out_path)["a"]}\+' # title of collation output
    '[^\/]*' # anything EXCEPT /
    '$' # end of line
)

matches = [
    key['Key']
    for page in client.get_paginator('list_objects').paginate(
        Bucket=bucket,
        Prefix=out_prefix,
    )
    for key in page.get('Contents', [])
    if pattern.match(key['Key'])
]

for match in matches:
    print(f'deleting stale collated data, {match}')
    client.delete_object(
        Bucket=bucket,
        Key=match,
    )


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
