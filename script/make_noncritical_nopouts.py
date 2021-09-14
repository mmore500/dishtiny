#!/usr/bin/env python3
"""Generate a genome morph where all noncritical sites are nopped out.

Requires control and morph competition experiment data.
Output genome is uploaded to s3 at a programatically-generated url.

Usage:
    ./make_noncritical_nopouts.py [bucket] [endeavor] [stint]
"""

import boto3
from collections import defaultdict
from functools import reduce
import gzip
from iterpop import iterpop as ip
import itertools
import json
from keyname import keyname as kn
import numpy as np
import os
import pandas as pd
import re
import re
from scipy import stats
import seaborn as sns
import sys
import tempfile
from tqdm import tqdm

from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pyloaders import genome_s3_autoload

def get_critical_sites(variant_df, control_fits_df):

    # count competions where both strains went extinct simultaneously
    # as 0 Fitness Differential
    na_rows = variant_df['Fitness Differential'].isna()
    assert all( variant_df[ na_rows ]['Population Extinct'] )
    variant_df['Fitness Differential'].fillna(0, inplace=True,)

    res = {}
    for series in variant_df['Competition Series'].unique():

        series_df = variant_df[ variant_df['Competition Series'] == series ]

        wt_vs_variant_df = series_df[
            series_df['genome variation'] != 'master'
        ].reset_index()

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(wt_vs_variant_df):
            wt_vs_variant_df['p'] =  wt_vs_variant_df.apply(
                lambda row: stats.t.cdf(
                    row['Fitness Differential'],
                    h0_fit['Fit Degrees of Freedom'],
                    loc=h0_fit['Fit Loc'],
                    scale=h0_fit['Fit Scale'],
                ),
                axis=1,
            )
        else:
            # special case for an empty dataframe
            # to prevent an exception
            wt_vs_variant_df['p'] = []


        p_thresh = 1.0 / 100
        less_fit_variants = wt_vs_variant_df[
            wt_vs_variant_df['p'] < p_thresh
        ]

        variation_attrs = [
            kn.unpack( kn.promote( variation ), source_attr=False )
            for variation in less_fit_variants['genome variation']
        ]

        assert all(
            'Nop-' in ip.popsingleton( attrs.values() )
            for attrs in variation_attrs
        )

        critical_idxs = [
            int( re.search(
                '^i(\d+)$', ip.popsingleton( attrs.keys() )
            ).group(1) )
            for attrs in variation_attrs
        ]

        res[ series ] = set(critical_idxs)

    return res

################################################################################
print(                                                                         )
print( 'running make_noncritical_nopout.py'                                    )
print( '---------------------------------------------------------------------' )
################################################################################

try:
    bucket = sys.argv[1]
    endeavor, stint = map(int, sys.argv[2:])
except:
    print( __doc__ )
    sys.exit(1)

print(f'bucket {bucket}')
print(f'endeavor {endeavor}')
print(f'stint {stint}')

s3 = boto3.resource('s3')
bucket_handle = s3.Bucket(bucket)

################################################################################
print(                                                                         )
print( 'getting assets'                                                        )
print( '---------------------------------------------------------------------' )
################################################################################

variant_competitions, = bucket_handle.objects.filter(
    Prefix=f'endeavor={endeavor}/variant-competitions/stage=3+what=collated/stint={stint}/'
)

variant_df = pd.read_csv(f's3://{bucket}/{variant_competitions.key}')

################################################################################
print(                                                                         )
print( 'calculating critical sites'                                            )
print( '---------------------------------------------------------------------' )
################################################################################

critical_sites_by_series = get_critical_sites(
    variant_df, get_control_t_distns( bucket, endeavor, stint )
)

for series, critical_sites in critical_sites_by_series.items():

    ############################################################################
    print(                                                                     )
    print( f'series {series} downloading wildtype genome'                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    wt_genome_s3_url = f's3://{bucket}/endeavor={endeavor}/genomes/stage=0+what=generated/stint={stint}/series={series}/a=genome+criteria=abundance+morph=wildtype+proc=0+series={series}+stint={stint}+thread=0+variation=master+ext=.json.gz'

    genome_data = genome_s3_autoload( wt_genome_s3_url )

    ############################################################################
    print(                                                                     )
    print( f'series {series} nopping noncritical sites'                        )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    program_length = len(genome_data["value0"]["program"])
    print( f' {program_length} insts' )

    print( f' {len(critical_sites)} critical sites' )

    num_ops_before = sum(
        'Nop-' not in inst['operation']
        for inst in genome_data["value0"]["program"]
    )
    print( f' {num_ops_before} ops before' )

    for idx, inst in enumerate( genome_data["value0"]["program"] ):
        if idx not in critical_sites:
            inst['operation'] = 'Nop-0'

    num_ops_after = sum(
        'Nop-' not in inst['operation']
        for inst in genome_data["value0"]["program"]
    )
    print( f' {num_ops_after} ops after' )


    ############################################################################
    print(                                                                     )
    print( f'series {series} uploading noncritical nopout'                     )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    out_path = f'endeavor={endeavor}/genomes/stage=3+what=tabulated/stint={stint}/series={series}/a=genome+criteria=abundance+morph=noncritical_nopout+proc=0+series={series}+stint={stint}+thread=0+variation=master+ext=.json.gz'

    # have to work with filename or pandas compression doesn't work
    with tempfile.NamedTemporaryFile() as temp:
        print('temporary file', temp.name)

        with gzip.open(temp.name, 'wt', encoding='ascii') as f:
            json.dump(genome_data, f)

        with open(temp.name, 'rb') as f:
            client = boto3.client('s3', region_name='us-west-2')
            client.upload_fileobj( f, bucket, out_path )


################################################################################
print(                                                                         )
print( 'noncritical nopouts complete'                                          )
print( '---------------------------------------------------------------------' )
################################################################################
