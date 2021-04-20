#!/usr/bin/env python3

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
    assemble_deletion_mutant_competitions, \
    assemble_deletion_mutant_phenotype_differentiation, \
    assemble_insertion_mutant_competitions, \
    assemble_insertion_mutant_phenotype_differentiation, \
    assemble_monoculture_dpp_metrics, \
    assemble_monoculture_kin_conflict_by_replev_statistics, \
    assemble_monoculture_kin_conflict_statistics, \
    assemble_mutant_competitions, \
    assemble_mutant_phenotype_differentiation, \
    assemble_mutating_competitions, \
    assemble_noncritical_nopout_fitness_competitions, \
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
print( 'running tabulate_and_stitch_stint.py'                                  )
print( '---------------------------------------------------------------------' )
################################################################################

try:
    bucket = sys.argv[1]
    endeavor, stint = map(int, sys.argv[2:])
except:
    print('bad arguments')
    print('USAGE: [bucket] [endeavor] [stint]')
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
    assemble_phenotype_neutral_nopout_fitness_competitions,
    assemble_deletion_mutant_competitions,
    assemble_insertion_mutant_competitions,
    assemble_mutating_competitions,
    assemble_mutant_competitions,
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
    assemble_monoculture_dpp_metrics,
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

df_stitched = reduce(
    lambda left, right: pd.merge(
        left,
        right,
        on=list(left.columns & right.columns),
        how='outer',
    ),
    dataframes,
)

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


out_prefix = f'endeavor={endeavor}/stage=6+what=tabulated_and_stitched/stint={stint}/'
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
