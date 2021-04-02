#!/usr/bin/env python3

import boto3
from collections import defaultdict
from functools import reduce
from iterdub import iterdub as ib
from iterpop import iterpop as ip
import itertools
from keyname import keyname as kn
import numpy as np
import os
import pandas as pd
import re
import seaborn as sns
import sys
import tempfile
from tqdm import tqdm
from scipy import stats

from pyhelpers import fit_control_t_distns
from pytabulators import reshape_dpp
from pytabulators import reshape_kin_conflict
from pytabulators import reshape_kin_conflict_by_replev
from pytabulators import tabulate_predecessor_fitness
from pytabulators import tabulate_strain_fitness
from pytabulators import tabulate_predecessor_battle_fitness
from pytabulators import filter_for_phenotype_neutral_nopout
from pytabulators import tabulate_fitness_complexity
from pytabulators import tabulate_mutant_fitness
from pytabulators import tabulate_either_perturbation_fitness
from pytabulators import tabulate_selfsend_fitness
from pytabulators import tabulate_phenotype_equivalent_nopout_fitness
from pytabulators import tabulate_perturbation_fitness
from pytabulators import tabulate_noncritical_nopout_fitness
from pytabulators import tabulate_progenitor_fitness
from pytabulators import tabulate_phenotype_neutral_nopout_phenotype_differentiation
from pytabulators import tabulate_wildtype_doubling_time
from pytabulators import tabulate_mutant_phenotype_differentiation

################################################################################
print(                                                                         )
print( 'running tabulate_stint.py'                                             )
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

s3 = boto3.resource('s3')
my_bucket = s3.Bucket(bucket)

dataframes = []
sources = []

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling config records'                                           )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        config_records, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/evolve/config-records/stage=1+what=collated/stint={stint}/'
        )

        config_records_df = pd.read_csv(f's3://{bucket}/{config_records.key}')
        config_records_df['Series'] = config_records_df['SERIES']
        config_records_df['Stint'] = config_records_df['STINT']

        dataframes.append( config_records_df )

        sources.append( config_records.key )
    except ValueError:
        print("missing config records, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling strain competitions'                                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        strain_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/strain-competitions/stage=2+what=collated/stint={stint}/'
        )

        strain_df = pd.read_csv(f's3://{bucket}/{strain_competitions.key}')

        dataframes.append(
            tabulate_strain_fitness( strain_df )
        )
        sources.append( strain_competitions.key )
    except ValueError:
        print("missing strain competitions, skipping")


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling variant competitions'                                     )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        variant_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/variant-competitions/stage=3+what=collated/stint={stint}/'
        )

        variant_df = pd.read_csv(f's3://{bucket}/{variant_competitions.key}')

        dataframes.append(
            tabulate_fitness_complexity(
                variant_df, fit_control_t_distns(control_df)
            )
        )
        sources.append( variant_competitions.key )
    except ValueError:
        print("missing variant competitions, skipping")

if (stint % 10 == 0):
    for target_state in ["Extrospective", "Introspective", "Writable"]:
        try:
            control_competitions, = my_bucket.objects.filter(
                Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
            )
        except ValueError:
            print("missing control data for perturbation competitions")
            print("skipping")
            break

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')
        control_distns = fit_control_t_distns(control_df)

        for perturbation_type in ["Rotate", "Exchange"]:
            ####################################################################
            print(                                                             )
            print(
                f'handling {target_state} state {perturbation_type}'
                f' perturbation competitions'
            )
            print( '---------------------------------------------------------' )
            ####################################################################

            try:
                perturbation_competitions, = my_bucket.objects.filter(
                    Prefix=f'endeavor={endeavor}/perturbation-{target_state.lower()}-state-{perturbation_type.lower()}-competitions/stage=2+what=collated/stint={stint}/'
                )

                perturbation_df = pd.read_csv(
                    f's3://{bucket}/{perturbation_competitions.key}'
                )

                dataframes.append(
                    tabulate_perturbation_fitness(
                        perturbation_df,
                        control_distns,
                        target_state,
                        perturbation_type,
                    )
                )
                sources.append( perturbation_competitions.key )

            except ValueError:
                print(
                    f'missing {target_state} state {perturbation_type}'
                    f' perturbation competitions'
                )
                print("skipping")

if (stint % 10 == 0):
    for target_state in ["Extrospective", "Introspective", "Writable"]:
        try:
            control_competitions, = my_bucket.objects.filter(
                Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/',
            )
            exchange_competitions, = my_bucket.objects.filter(
                Prefix=f'endeavor={endeavor}/perturbation-{target_state.lower()}-state-exchange-competitions/stage=2+what=collated/stint={stint}/',
            )
            rotate_competitions, = my_bucket.objects.filter(
                Prefix=f'endeavor={endeavor}/perturbation-{target_state.lower()}-state-rotate-competitions/stage=2+what=collated/stint={stint}/',
            )

        except ValueError:
            print(f"missing either {target_state} perturbation competitions")
            print("or control competitions")
            print("skipping")
            break

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')
        control_distns = fit_control_t_distns(control_df)

        exchange_df = pd.read_csv(
            f's3://{bucket}/{exchange_competitions.key}'
        )
        rotate_df = pd.read_csv(
            f's3://{bucket}/{rotate_competitions.key}'
        )


        ########################################################################
        print(                                                                 )
        print(f'handling {target_state} state either perturbation competitions')
        print( '--------------------------------------------------------------')
        ########################################################################

        dataframes.append(
            tabulate_either_perturbation_fitness(
                exchange_df,
                rotate_df,
                control_distns,
                target_state,
            )
        )
        sources.append( control_competitions.key )
        sources.append( exchange_competitions.key )
        sources.append( rotate_competitions.key )

if (stint % 10 == 0):
    for messaging_target in ["Inter", "Intra"]:
        try:
            control_competitions, = my_bucket.objects.filter(
                Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
            )
        except ValueError:
            print("missing control data for selfsend competitions")
            print("skipping")
            break

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')
        control_distns = fit_control_t_distns(control_df)

        ########################################################################
        print(                                                                 )
        print( f'handling {target_state} messaging self-send competitions'     )
        print( '-------------------------------------------------------------' )
        ####################################################################

        try:
            selfsend_competitions, = my_bucket.objects.filter(
                Prefix=f'endeavor={endeavor}/selfsend-{messaging_target.lower()}-competitions/stage=2+what=collated/stint={stint}/'
            )

            selfsend_df = pd.read_csv(
                f's3://{bucket}/{selfsend_competitions.key}'
            )

            dataframes.append(
                tabulate_selfsend_fitness(
                    selfsend_df,
                    control_distns,
                    messaging_target,
                )
            )
            sources.append( selfsend_competitions.key )

        except ValueError:
                print( f'missing {messaging_target} selfsend competitions' )
                print("skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling mutant competitions'                                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        mutant_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-competitions/stage=2+what=collated/stint={stint}/'
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_competitions.key}'
        )

        dataframes.append( tabulate_mutant_fitness(
            mutant_df, fit_control_t_distns(control_df)
        ) )
        sources.append( mutant_competitions.key )
    except ValueError:
        print("missing mutant competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling noncritical nopout fitness competitions'                  )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        noncritical_nopout_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/noncritical-nopout-competitions/stage=5+what=collated/stint={stint}/'
        )

        noncritical_nopout_competition_df = pd.read_csv(
            f's3://{bucket}/{noncritical_nopout_competitions.key}'
        )

        dataframes.append( tabulate_phenotype_equivalent_nopout_fitness(
            noncritical_nopout_competition_df,
            fit_control_t_distns(control_df),
        ) )
        sources.append( noncritical_nopout_competitions.key )
    except ValueError:
        print("missing noncritical nopout competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling phenotype neutral nopout fitness competitions'            )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        phenotype_neutral_nopout_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/phenotype-neutral-nopout-competitions/stage=3+what=collated/stint={stint}/'
        )

        phenotype_neutral_nopout_competition_df = pd.read_csv(
            f's3://{bucket}/{phenotype_neutral_nopout_competitions.key}'
        )

        dataframes.append( tabulate_phenotype_equivalent_nopout_fitness(
            phenotype_neutral_nopout_competition_df,
            fit_control_t_distns(control_df),
        ) )
        sources.append( phenotype_neutral_nopout_competitions.key )
    except ValueError:
        print("missing phenotype neutral nopout competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling deletion mutant competitions'                             )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        mutant_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-deletion-competitions/stage=2+what=collated/stint={stint}/'
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_competitions.key}'
        )

        dataframes.append(
            tabulate_mutant(
                mutant_df, fit_control_t_distns(control_df), 'Deletion ',
            )
        )
        sources.append( mutant_competitions.key )
    except ValueError:
        print("missing deletion mutant competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling insertion mutant competitions'                            )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        mutant_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-insertion-competitions/stage=2+what=collated/stint={stint}/'
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_competitions.key}'
        )

        dataframes.append(
            tabulate_mutant(
                mutant_df, fit_control_t_distns(control_df), 'Insertion ',
            )
        )
        sources.append( mutant_competitions.key )
    except ValueError:
        print("missing insertion mutant competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling mutating competitions'                                    )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        mutant_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutating-competitions/stage=2+what=collated/stint={stint}/'
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_competitions.key}'
        )

        dataframes.append(
            tabulate_mutant(
                mutant_df, fit_control_t_distns(control_df), 'Mutating ',
            )
        )
        sources.append( mutant_competitions.key )
    except ValueError:
        print("missing mutating competitions, skipping")


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling point mutant competitions'                                )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        control_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/control-competitions/stage=2+what=collated/stint={stint}/'
        )

        control_df = pd.read_csv(f's3://{bucket}/{control_competitions.key}')

        mutant_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-point-competitions/stage=2+what=collated/stint={stint}/'
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_competitions.key}'
        )

        dataframes.append(
            tabulate_mutant(
                mutant_df, fit_control_t_distns(control_df), 'Point ',
            )
        )
        sources.append( mutant_competitions.key )
    except ValueError:
        print("missing point mutant competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling wildtype doubling-time'       )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        wildtype_doubling_time, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/wildtype-doubling-time/stage=2+what=collated/stint={stint}/',
        )

        wildtype_doubling_time_df = pd.read_csv(
            f's3://{bucket}/{wildtype_doubling_time.key}',
        )

        dataframes.append(
            tabulate_wildtype_doubling_time(
                wildtype_doubling_time_df,
            )
        )
        sources.append( wildtype_doubling_time.key )
    except ValueError:
        print("missing wildtype doubling-time")
        print("skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling phenotype neutral nopout phenotype-differentiation'       )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        phenotype_neutral_nopout_phenotype_differentiation, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/phenotype-neutral-nopout-phenotype-differentiation/stage=3+what=collated/stint={stint}/',
        )

        phenotype_neutral_nopout_phenotype_differentiation_df = pd.read_csv(
            f's3://{bucket}/{phenotype_neutral_nopout_phenotype_differentiation.key}',
        )

        dataframes.append(
            tabulate_phenotype_neutral_nopout_phenotype_differentiation(
                phenotype_neutral_nopout_phenotype_differentiation_df,
            )
        )
        sources.append( phenotype_neutral_nopout_phenotype_differentiation.key )
    except ValueError:
        print("missing phenotype neutral nopout phenotype-differentiation")
        print("skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling mutant phenotype-differentiation'                         )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        mutant_phenotype_differentiation, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-phenotype-differentiation/stage=2+what=collated/stint={stint}/',
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_phenotype_differentiation.key}',
        )

        dataframes.append( tabulate_mutant_phenotype_differentiation(
            mutant_df,
        ) )
        sources.append( mutant_phenotype_differentiation.key )
    except ValueError:
        print("missing mutant phenotype-differentiation, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling deletion mutant phenotype-differentiation'                )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        mutant_phenotype_differentiation, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-deletion-phenotype-differentiation/stage=2+what=collated/stint={stint}/',
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_phenotype_differentiation.key}',
        )

        dataframes.append( tabulate_mutant_phenotype_differentiation(
            mutant_df, 'Deletion ',
        ) )
        sources.append( mutant_phenotype_differentiation.key )
    except ValueError:
        print("missing deletion mutant phenotype-differentiation, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling insertion mutant phenotype-differentiation'               )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        mutant_phenotype_differentiation, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-insertion-phenotype-differentiation/stage=2+what=collated/stint={stint}/',
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_phenotype_differentiation.key}',
        )

        dataframes.append( tabulate_mutant_phenotype_differentiation(
            mutant_df, 'Insertion ',
        ) )
        sources.append( mutant_phenotype_differentiation.key )
    except ValueError:
        print("missing insertion mutant phenotype-differentiation, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling point mutant phenotype-differentiation'                   )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:

        mutant_phenotype_differentiation, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/mutant-point-phenotype-differentiation/stage=2+what=collated/stint={stint}/',
        )

        mutant_df = pd.read_csv(
            f's3://{bucket}/{mutant_phenotype_differentiation.key}',
        )

        dataframes.append( tabulate_mutant_phenotype_differentiation(
            mutant_df, 'Point ',
        ) )
        sources.append( mutant_phenotype_differentiation.key )
    except ValueError:
        print("missing point mutant phenotype-differentiation, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling predecessor competitions'                                 )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        predecessor_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/predecessor-competitions/stage=2+what=collated/stint={stint}/',
        )

        predecessor_df = pd.read_csv(
            f's3://{bucket}/{predecessor_competitions.key}',
        )

        dataframes.append(
            tabulate_predecessor_fitness( predecessor_df )
        )
        sources.append( predecessor_competitions.key )

    except ValueError:
        print("missing predecessor competitions, skipping")


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling predecessor battles'                                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        predecessor_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/predecessor-battles/stage=2+what=collated/stint={stint}/',
        )

        predecessor_df = pd.read_csv(
            f's3://{bucket}/{predecessor_competitions.key}',
        )

        dataframes.append(
            tabulate_predecessor_battle_fitness( predecessor_df )
        )
        sources.append( predecessor_competitions.key )

    except ValueError:
        print("missing predecessor battles, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling progenitor competitions'                                  )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        progenitor_competitions, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/progenitor-competitions/stage=2+what=collated/stint={stint}/'
        )

        progenitor_df = pd.read_csv(f's3://{bucket}/{progenitor_competitions.key}')

        dataframes.append(
            tabulate_progenitor_fitness( progenitor_df )
        )
        sources.append( progenitor_competitions.key )
    except ValueError:
        print("missing progenitor competitions, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling phenotype neutral nopouts'                                )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        genome_statistics, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/genomes/stage={2 if stint % 10 == 0 else 1}+what=collated/stint={stint}/'
        )

        genome_df = pd.read_csv(f's3://{bucket}/{genome_statistics.key}')

        dataframes.append(
            filter_for_phenotype_neutral_nopout( genome_df )
        )
        sources.append( genome_statistics.key )
    except ValueError:
        print("missing phenotype neutral nopout genotype statistics, skipping")


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling monoculture kin conflict statistics'                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        monoculture_kin_conflict_statistics, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/monocultures/metrics/stage=2+what=collated/stint={stint}/a=kin_conflict_statistics+'
        )

        monoculture_kin_conflict_df = pd.read_csv(
            f's3://{bucket}/{monoculture_kin_conflict_statistics.key}'
        )

        dataframes.append(
            reshape_kin_conflict( monoculture_kin_conflict_df )
        )
        sources.append( monoculture_kin_conflict_statistics.key )
    except ValueError:
        print("missing monoculture kin conflict statistics, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling monoculture kin conflict by replev statistics'            )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        monoculture_kin_conflict_by_replev_statistics, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/monocultures/metrics/stage=2+what=collated/stint={stint}/a=kin_conflict_by_replev_statistics+'
        )

        monoculture_kin_conflict_by_replev_df = pd.read_csv(
            f's3://{bucket}/{monoculture_kin_conflict_by_replev_statistics.key}'
        )

        dataframes.append(
            reshape_kin_conflict_by_replev( monoculture_kin_conflict_by_replev_df )
        )
        sources.append( monoculture_kin_conflict_by_replev_statistics.key )
    except ValueError:
        print("missing monoculture kin conflict by replev statistics, skipping")

if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling monoculture dpp metrics'                                  )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    try:
        monoculture_dpp_metrics, = my_bucket.objects.filter(
            Prefix=f'endeavor={endeavor}/monocultures/metrics/stage=2+what=collated/stint={stint}/a=demographic_phenotypic_phylogenetic_metrics+'
        )

        monoculture_dpp_df = pd.read_csv(
            f's3://{bucket}/{monoculture_dpp_metrics.key}'
        )

        dataframes.append(
            reshape_dpp( monoculture_dpp_df )
        )
        sources.append( monoculture_dpp_metrics.key )
    except ValueError:
        print("missing monoculture dpp metrics, skipping")

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
