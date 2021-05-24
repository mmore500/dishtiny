#!/usr/bin/env python3
"""Combines all available collated data on s3 for a single stint.

Collated data is tabulated (reformatted and processed) and stitched into a single dataframe.
Uploads output to a programatically-generated s3 url.

Usage:
    ./tabulate_and_stitch_stint_series_profiles.py [bucket] [endeavor] [stint]
"""

import boto3
from functools import reduce
from iterdub import iterdub as ib
from iterpop import iterpop as ip
import itertools as it
from keyname import keyname as kn
import os
import pandas as pd
import sys
import tempfile

from dishpylib.pyassemblers import \
    assemble_config_records, \
    assemble_deletion_mutant_competitions, \
    assemble_deletion_mutant_phenotype_differentiation, \
    assemble_evolve_dpp_metrics_threadfirst, \
    assemble_evolve_dpp_metrics_threadmean, \
    assemble_evolve_kin_conflict_statistics, \
    assemble_insertion_mutant_competitions, \
    assemble_insertion_mutant_phenotype_differentiation, \
    assemble_lowestroot_immediatepredecessor_battles, \
    assemble_monoculture_birth_log_statistics, \
    assemble_monoculture_dpp_metrics_threadfirst, \
    assemble_monoculture_dpp_metrics_threadmean, \
    assemble_monoculture_kin_conflict_by_replev_statistics, \
    assemble_monoculture_kin_conflict_statistics, \
    assemble_mutant_competitions, \
    assemble_mutant_phenotype_differentiation, \
    assemble_mutating_competitions, \
    assemble_noncritical_nopout_fitness_competitions, \
    assemble_noncritical_phenotypeequivalent_nopinterpolation_competitions, \
    assemble_phenotype_neutral_nopout_fitness_competitions, \
    assemble_phenotype_neutral_nopout_phenotype_differentiation, \
    assemble_phenotype_neutral_nopouts, \
    assemble_point_mutant_competitions, \
    assemble_point_mutant_phenotype_differentiation, \
    assemble_predecessor_battles, \
    assemble_predecessor_competitions, \
    assemble_progenitor_competitions, \
    assemble_strain_competitions, \
    assemble_variant_competitions, \
    assemble_wildtype_doubling_time, \
    either_perturbation_competitions_assembler_factory, \
    messaging_selfsend_competitions_assembler_factory, \
    perturbation_competitions_assembler_factory

################################################################################
print(                                                                         )
print( 'running tabulate_and_stitch_stint_series_profiles.py'                  )
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
    assemble_config_records,
    assemble_strain_competitions,
    assemble_variant_competitions,
] + [
    perturbation_competitions_assembler_factory(
        target_state, perturbation_type
    )
    for target_state in ('Extrospective', 'Introspective', 'Writable')
    for perturbation_type in ('Rotate', 'Exchange')
] + [
    either_perturbation_competitions_assembler_factory( target_state )
    for target_state in ('Extrospective', 'Introspective', 'Writable')
] + [
    messaging_selfsend_competitions_assembler_factory( messaging_target )
    for messaging_target in ('Inter', 'Intra')
] + [
    assemble_mutant_competitions,
    assemble_noncritical_nopout_fitness_competitions,
    assemble_noncritical_phenotypeequivalent_nopinterpolation_competitions,
    assemble_phenotype_neutral_nopout_fitness_competitions,
    assemble_deletion_mutant_competitions,
    assemble_insertion_mutant_competitions,
    assemble_mutating_competitions,
    assemble_point_mutant_competitions,
    assemble_wildtype_doubling_time,
    assemble_phenotype_neutral_nopout_phenotype_differentiation,
    assemble_mutant_phenotype_differentiation,
    assemble_deletion_mutant_phenotype_differentiation,
    assemble_insertion_mutant_phenotype_differentiation,
    assemble_point_mutant_phenotype_differentiation,
    assemble_predecessor_competitions,
    assemble_predecessor_battles,
    assemble_progenitor_competitions,
    assemble_phenotype_neutral_nopouts,
    assemble_monoculture_kin_conflict_statistics,
    assemble_monoculture_kin_conflict_by_replev_statistics,
    assemble_monoculture_dpp_metrics_threadfirst,
    assemble_evolve_dpp_metrics_threadfirst,
    assemble_monoculture_dpp_metrics_threadmean,
    assemble_evolve_dpp_metrics_threadmean,
    assemble_evolve_kin_conflict_statistics,
    assemble_monoculture_birth_log_statistics,
    assemble_lowestroot_immediatepredecessor_battles,
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

def merge_pair(left, right):

    common_columns = list(left.columns & right.columns)

    # cross-fill nans if the other df has a value
    for col in common_columns:
        for cur, other in it.permutations([left, right]):
            for idx, row in cur[ cur[col].isnull() ].iterrows():
                other_indexer = (
                    (other['Stint'] == row['Stint'])
                    & (other['Series'] == row['Series'])
                )
                if other_indexer.any():
                    cur.at[idx,col] = ip.popsingleton(other[other_indexer][col])

    # verify data
    apply_comparable_indexing = lambda df: df.set_index(
        ['Series', 'Stint'],
        drop=False,
        verify_integrity=True,
    )
    reindexed_left = apply_comparable_indexing(left)
    reindexed_right = apply_comparable_indexing(right)
    common_indexes = reindexed_left.index.intersection(reindexed_right.index)
    comparable_left = reindexed_left.loc[ common_indexes ]
    comparable_right = reindexed_right.loc[ common_indexes ]
    for col in common_columns:
        # .equals() considers nans in same location equal
        assert comparable_left[col].equals( comparable_right[col] ), {
            'col' : col,
            'comparable_left' : comparable_left,
            'comparable_left[col]' : comparable_left[col],
            'comparable_right' : comparable_right,
            'comparable_right[col]' : comparable_right[col],
        }

    # do the merge
    res = pd.merge(
        left,
        right,
        on=common_columns,
        how='outer',
        validate='one_to_one',
    )

    # verify further
    assert len(res.groupby([
        'Series',
        'Stint',
    ])) == len(res), right

    return res

df_stitched = reduce(
    merge_pair,
    dataframes,
)

print(f'merged dataframe has {len(df_stitched.index)} rows')

# there should only be one entry for each series/stint
assert len(df_stitched.groupby([
    'Series',
    'Stint',
])) == len(df_stitched)
assert ip.pophomogeneous(df_stitched['Stint']) == stint
assert ip.pophomogeneous(df_stitched['Series'] // 1000) == endeavor


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
        'a' : 'series_profiles',
        'stint' : stint,
        'ext' : '.csv.xz',
    # },
})


out_prefix = f'endeavor={endeavor}/series-profiles/stage=6+what=tabulated_and_stitched/stint={stint}/'
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
