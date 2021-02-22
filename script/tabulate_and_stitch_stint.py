#!/usr/bin/env python3

import boto3
from functools import reduce
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

# adapted from https://stackoverflow.com/a/43091576
def to_ranges(integers):
    integers = sorted(set(integers))
    for key, group in itertools.groupby(
        enumerate(integers),
        lambda t: t[1] - t[0],
    ):
        group = list(group)
        yield group[0][1], group[-1][1]


def summarize_ranges(integers):
    return '_'.join(
        f'{fromm}-{to}'
        if fromm != to else
        str(fromm)
        for fromm, to in to_ranges(integers)
    )


def summarize(values):
    vals=list(values)
    if all(value.isdigit() for value in vals):
        return summarize_ranges(map(int, vals))
    elif len(set(vals)) < 3:
        return '~'.join( sorted(set(vals)) )
    else:
        return f'num_unique%{len(set(vals))}'


def reshape_dpp(df):

    just_one_thread = df.groupby([
        'Stint',
        'Series',
        'Metric',
        'Update',
    ]).first().reset_index()

    last_update_idxs = just_one_thread.groupby([
        'Stint',
        'Series',
        'Metric',
    ])['Update'].transform(max) == just_one_thread['Update']

    # move "Metric" values into columns
    return just_one_thread[ last_update_idxs ].pivot(
        index=['Stint', 'Series',],
        columns='Metric',
        values='Value',
    ).reset_index()


def reshape_kin_conflict(df):

    just_one_thread = df.groupby([
        'Stint',
        'Series',
        'Parent-Eliminated Kin ID Commonality',
    ]).first().reset_index()

    # move Parent-Eliminated Kin ID Commonality
    # into a hierarchically-nested column name
    res = just_one_thread.pivot(
        index=['Stint', 'Series',],
        columns='Parent-Eliminated Kin ID Commonality',
    ).reset_index()

    # collapse hierarchically-nested columns into keyname-packed strings
    res.columns = res.columns.map(
        lambda x: kn.pack({
            'a' : x[0],
            'Parent-Eliminated Kin ID Commonality' : x[1],
        }) if x[1] != "" else x[0]
    )

    return res


def reshape_kin_conflict_by_replev(df):

    # move Parent-Eliminated Kin ID Commonality and Replev
    # into a hierarchically-nested column name
    just_one_thread = df.groupby([
        'Stint',
        'Series',
        'Parent-Eliminated Kin ID Commonality',
        'Replev',
    ]).first().reset_index()

    res = just_one_thread.pivot(
        index=['Stint', 'Series',],
        columns=[
            'Parent-Eliminated Kin ID Commonality',
            'Replev',
        ],
    ).reset_index()

    # collapse hierarchically-nested columns into keyname-packed strings
    res.columns = res.columns.map(
        lambda x: kn.pack({
            'a' : x[0],
            'Parent-Eliminated Kin ID Commonality' : x[1],
            'Replev' : x[2],
        }) if x[1] != "" and x[2] != "" else x[0]
    )

    return res


def tabulate_strain_fitness(strain_df):

    strain_df['Series'] = strain_df['genome series']

    mean_differential = strain_df.groupby(
        ['Series'],
    )['Fitness Differential'].mean().reset_index(
        name='Mean Inter-Strain Fitness Differential',
    )

    frac_won = strain_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series:
            (just_one_series > 0).sum() / len(just_one_series),
    ).reset_index(
        name='Fraction Inter-Strain Competitions Won',
    )

    null_p = strain_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series: stats.binom_test(
            x=(just_one_series > 0).sum(), # number of successes
            n=len(just_one_series), # number of trials
            p=0.5,
            alternative='two-sided',
        ),
    ).reset_index(
        name='Inter-Strain Competition Null p-value',
    )

    return reduce(
        lambda left, right: pd.merge(
            left,
            right,
            on='Series',
        ),
        [
            mean_differential,
            frac_won,
            null_p,
        ],
    )

def tabulate_predecessor_fitness(predecessor_df):

    # root id 0 is the control competitors (i.e., the predecessors)
    predecessor_df = predecessor_df[
        predecessor_df['Root ID'] == 1
    ].reset_index()
    predecessor_df['Series'] = predecessor_df['genome series']

    mean_differential = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].mean().reset_index(
        name='Mean Fitness Differential Against Predecessor',
    )

    frac_won = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series:
            (just_one_series > 0).sum() / len(just_one_series),
    ).reset_index(
        name='Fraction Predecessor Competitions Won',
    )

    null_p = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series: stats.binom_test(
            x=(just_one_series > 0).sum(), # number of successes
            n=len(just_one_series), # number of trials
            p=0.5,
            alternative='two-sided',
        ),
    ).reset_index(
        name='Predecessor Competition Null p-value',
    )

    return reduce(
        lambda left, right: pd.merge(
            left,
            right,
            on='Series',
        ),
        [
            mean_differential,
            frac_won,
            null_p,
        ],
    )

def tabulate_progenitor_fitness(progenitor_df):

    # root id 0 is the control competitors (i.e., the progenitors)
    progenitor_df = progenitor_df[
        progenitor_df['Root ID'] == 1
    ].reset_index()
    progenitor_df['Series'] = progenitor_df['genome series']

    mean_differential = progenitor_df.groupby(
        ['Series'],
    )['Fitness Differential'].mean().reset_index(
        name='Mean Fitness Differential Against Progenitor',
    )

    frac_won = progenitor_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series:
            (just_one_series > 0).sum() / len(just_one_series),
    ).reset_index(
        name='Fraction Progenitor Competitions Won',
    )

    null_p = progenitor_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series: stats.binom_test(
            x=(just_one_series > 0).sum(), # number of successes
            n=len(just_one_series), # number of trials
            p=0.5,
            alternative='two-sided',
        ),
    ).reset_index(
        name='Progenitor Competition Null p-value',
    )

    return reduce(
        lambda left, right: pd.merge(
            left,
            right,
            on='Series',
        ),
        [
            mean_differential,
            frac_won,
            null_p,
        ],
    )


def filter_for_phenotype_neutral_nopout(genome_df):
    res = genome_df[
        genome_df['genome morph'] == 'phenotype_equivalent_nopout'
    ].reset_index()
    res['Phenotype Complexity'] = res['Num Op Instructions']
    return res


def tabulate_fitness_complexity(variant_df):

    res = []
    for series in variant_df['Competition Series'].unique():

        series_df = variant_df[ variant_df['Competition Series'] == series ]

        wt_vs_wt_df = series_df.groupby('Competition Repro').filter(
            lambda x: (x['genome variation'] == 'master').all()
        ).groupby('Competition Repro').first().reset_index()

        wt_vs_variant_df = series_df[
            series_df['genome variation'] != 'master'
        ].reset_index()

        # fit a t distribution to the control data
        df, loc, scale = stats.t.fit( wt_vs_wt_df['Fitness Differential'] )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(wt_vs_variant_df):
            wt_vs_variant_df['p'] =  wt_vs_variant_df.apply(
                lambda row: stats.t.cdf(
                    row['Fitness Differential'],
                    df,
                    loc=loc,
                    scale=scale,
                ),
                axis=1,
            )
        else:
            # special case for an empty dataframe
            # to prevent an exception
            wt_vs_variant_df['p'] = []


        p_thresh = 1.0 / 100
        num_more_fit_variants = (wt_vs_variant_df['p'] > 1 - p_thresh).sum()
        num_less_fit_variants = (wt_vs_variant_df['p'] < p_thresh).sum()

        expected_false_positives = len(wt_vs_variant_df) * p_thresh

        res.append({
            'Series' : series,
            'Flagged Advantageous Sites' : num_less_fit_variants,
            'Flagged Deleterious Sites' : num_more_fit_variants,
            'H_0 Advantageous Site Flags' : expected_false_positives,
            'H_0 Deleterious Site Flags' : expected_false_positives,
            'Estimated True Advantageous Sites'
                : num_less_fit_variants - expected_false_positives,
            'Estimated True Deleterious Sites'
                : num_more_fit_variants - expected_false_positives,
        })

    return pd.DataFrame(res)


################################################################################
print(                                                                         )
print( 'running tabulate_stint.py'                                             )
print( '---------------------------------------------------------------------' )
################################################################################

bucket = 'dnh2v'

endeavor, stint = map(int, sys.argv[1:])

s3 = boto3.resource('s3')
my_bucket = s3.Bucket(bucket)

dataframes = []
sources = []

if (stint % 20 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling strain competitions'                                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    strain_competitions, = my_bucket.objects.filter(
        Prefix=f'endeavor={endeavor}/strain-competitions/stage=2+what=collated/stint={stint}/'
    )

    strain_df = pd.read_csv(f's3://{bucket}/{strain_competitions.key}')

    dataframes.append(
        tabulate_strain_fitness( strain_df )
    )
    sources.append( strain_competitions.key )


if (stint % 20 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling variant competitions'                                     )
    print( '-----------------------------------------------------------------' )
    ############################################################################


    variant_competitions, = my_bucket.objects.filter(
        Prefix=f'endeavor={endeavor}/variant-competitions/stage=3+what=collated/stint={stint}/'
    )

    variant_df = pd.read_csv(f's3://{bucket}/{variant_competitions.key}')

    dataframes.append(
        tabulate_fitness_complexity( variant_df )
    )
    sources.append( variant_competitions.key )


if (stint % 20 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling predecessor competitions'                                 )
    print( '-----------------------------------------------------------------' )
    ############################################################################


    predecessor_competitions, = my_bucket.objects.filter(
        Prefix=f'endeavor={endeavor}/predecessor-competitions/stage=2+what=collated/stint={stint}/'
    )

    predecessor_df = pd.read_csv(
        f's3://{bucket}/{predecessor_competitions.key}'
    )

    dataframes.append(
        tabulate_predecessor_fitness( predecessor_df )
    )
    sources.append( predecessor_competitions.key )


if (stint % 20 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling progenitor competitions'                                  )
    print( '-----------------------------------------------------------------' )
    ############################################################################


    progenitor_competitions, = my_bucket.objects.filter(
        Prefix=f'endeavor={endeavor}/progenitor-competitions/stage=2+what=collated/stint={stint}/'
    )

    progenitor_df = pd.read_csv(f's3://{bucket}/{progenitor_competitions.key}')

    dataframes.append(
        tabulate_progenitor_fitness( progenitor_df )
    )
    sources.append( progenitor_competitions.key )


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling phenotype neutral nopouts'                                )
    print( '-----------------------------------------------------------------' )
    ############################################################################

    genome_statistics, = my_bucket.objects.filter(
        Prefix=f'endeavor={endeavor}/genomes/stage={2 if stint % 20 == 0 else 1}+what=collated/stint={stint}/'
    )

    genome_df = pd.read_csv(f's3://{bucket}/{genome_statistics.key}')

    dataframes.append(
        filter_for_phenotype_neutral_nopout( genome_df )
    )
    sources.append( genome_statistics.key )


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling monoculture kin conflict statistics'                      )
    print( '-----------------------------------------------------------------' )
    ############################################################################

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


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling monoculture kin conflict by replev statistics'            )
    print( '-----------------------------------------------------------------' )
    ############################################################################

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


if (stint % 10 == 0):
    ############################################################################
    print(                                                                     )
    print( 'handling monoculture dpp metrics'                                  )
    print( '-----------------------------------------------------------------' )
    ############################################################################

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
    #     key : summarize(
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


out_prefix = f'endeavor={endeavor}/stage=4+what=tabulated_and_stitched/stint={stint}/'
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
