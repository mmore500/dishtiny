#!/usr/bin/env python3
"""Combines all available collated data on s3 for a single stint.

Collated data is tabulated (reformatted and processed) and stitched into a single dataframe.
Uploads output to a programatically-generated s3 url.

Usage:
    ./tabulate_and_stitch_stint_thread_profiles.py [bucket] [endeavor] [stint]
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

from dishpylib.pyassemblers import \
    assemble_config_records, \
    assemble_evolve_dpp_metrics

################################################################################
print(                                                                         )
print( 'running tabulate_and_stitch_stint_thread_profiles.py'                  )
print( '---------------------------------------------------------------------' )
################################################################################

try:
    bucket = sys.argv[1]
    endeavor, stint = map(int, sys.argv[2:])
except:
    print(__doc__)
    sys.exit(1)

print(f'bucket {bucket}')
print(f'endeavor {endeavor}')
print(f'stint {stint}')

################################################################################
print(                                                                         )
print( 'running data assemblers'                                               )
print( '---------------------------------------------------------------------' )
################################################################################

assemblers = [
    assemble_evolve_dpp_metrics,
]


dataframes = []
sources = []
for assembler in assemblers:

    res = assembler(
        bucket=bucket,
        endeavor=endeavor,
        stint=stint,
    )

    if res is not None:
        res_df, res_sources = res
        dataframes.append( res_df )
        sources += res_sources


################################################################################
print(                                                                         )
print( 'stitching data'                                                        )
print( '---------------------------------------------------------------------' )
################################################################################

print(f'{len(dataframes)} dataframes to merge')
print(f'dataframes have {[len(df.index) for df in dataframes]} rows')

df_stitched = reduce(
    lambda left, right: pd.merge(
        left,
        right,
        on=list(left.columns & right.columns),
        how='outer',
    ),
    dataframes,
)

print(f'merged dataframe has {len(df_stitched.index)} rows')

# there should only be one entry for each series/stint/thread/proc
assert len(res_df.groupby([
    'Series',
    'Stint',
    'thread',
    'proc',
])) == len(res_df)

################################################################################
print(                                                                         )
print( 'calculating upload path'                                               )
print( '---------------------------------------------------------------------' )
################################################################################

# common_keys = set.intersection(*[
#     set( kn.unpack(source).keys() )
#     for source in sources
# ])

out_filename = kn.pack({
    # **{
    #     key : ib.dub(
    #         kn.unpack(source)[key]
    #         for source in sources
    #     )
    #     for key in common_keys
    # },
    # **{
        'a' : 'thread_profiles',
        'stint' : stint,
        'ext' : '.csv.xz',
    # },
})


out_prefix = f'endeavor={endeavor}/thread-profiles/stage=6+what=tabulated_and_stitched/stint={stint}/'
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


    df_stitched.to_csv(
        temp_path,
        index=False,
        compression='xz',
    )

    with open(temp_path, 'rb') as f:
        client = boto3.client('s3', region_name='us-west-2',)
        client.upload_fileobj(
            f,
            bucket,
            out_path,
        )

################################################################################
print(                                                                         )
print( 'tabulation and stitching complete'                                     )
print( '---------------------------------------------------------------------' )
################################################################################
