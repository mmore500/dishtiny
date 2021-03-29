#!/usr/bin/env python3

import boto3
from collections import defaultdict
from functools import reduce
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

def fit_control_t_distns(control_df):

    na_rows = control_df['Fitness Differential'].isna()
    assert all( control_df[ na_rows ]['Population Extinct'] )
    control_df['Fitness Differential'].fillna(0, inplace=True,)

    res = []
    for series in control_df['Competition Series'].unique():

        series_df = control_df[ control_df['Competition Series'] == series ]

        # legacy data was mixed inside of the variant_df
        wt_vs_wt_df = series_df.groupby('Competition Repro').filter(
            lambda x: (x['genome variation'] == 'master').all()
        ).groupby('Competition Repro').first().reset_index()

        # fit a t distribution to the control data
        # df is degrees of freedom
        df, loc, scale = stats.t.fit( wt_vs_wt_df['Fitness Differential'] )


        res.append({
            'Series' : series,
            'Fit Degrees of Freedom' : df,
            'Fit Loc' : loc,
            'Fit Scale' : scale,
        })

    return pd.DataFrame(res)

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

def tabulate_predecessor_battle_fitness(predecessor_df):

    # root id 0 is the control competitors (i.e., the predecessors)
    predecessor_df = predecessor_df[
        predecessor_df['Root ID'] == 1
    ].reset_index()
    predecessor_df['Series'] = predecessor_df['genome series']

    mean_differential = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].mean().reset_index(
        name='Mean Fitness Differential Against Predecessor Population',
    )

    frac_won = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series:
            (just_one_series > 0).sum() / len(just_one_series),
    ).reset_index(
        name='Fraction Predecessor Battles Won',
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
        name='Predecessor Battle Null p-value',
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


def tabulate_fitness_complexity(variant_df, control_fits_df):

    # count competions where both strains went extinct simultaneously
    # as 0 Fitness Differential
    na_rows = variant_df['Fitness Differential'].isna()
    assert all( variant_df[ na_rows ]['Population Extinct'] )
    variant_df['Fitness Differential'].fillna(0, inplace=True,)

    res = []
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
        num_more_fit_variants = (wt_vs_variant_df['p'] > 1 - p_thresh).sum()
        num_less_fit_variants = (wt_vs_variant_df['p'] < p_thresh).sum()

        expected_false_positives = len(wt_vs_variant_df) * p_thresh

        res.append({
            'Stint' : ip.pophomogeneous( variant_df['Competition Stint'] ),
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

def tabulate_mutant(mutant_df, control_fits_df, mutation_type=''):

    res = []
    for series in mutant_df['Competition Series'].unique():

        mutant_series_df = mutant_df[
            mutant_df['Competition Series'] == series
        ]

        wt_vs_mutant_df = mutant_series_df[
            mutant_series_df['genome root_id'] == 1
        ].reset_index()

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(wt_vs_mutant_df):
            wt_vs_mutant_df['p'] =  wt_vs_mutant_df.apply(
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
            wt_vs_mutant_df['p'] = []


        p_thresh = 1.0 / 100
        num_more_fit_mutants = (wt_vs_mutant_df['p'] > 1 - p_thresh).sum()
        num_less_fit_mutants = (wt_vs_mutant_df['p'] < p_thresh).sum()

        res.append({
            'Series' : series,
            f'Num More Fit {mutation_type}Mutants' : num_more_fit_mutants,
            f'Num Less Fit {mutation_type}Mutants' : num_less_fit_mutants,
            f'Fraction {mutation_type}Mutations that are Advantageous'
                : num_more_fit_mutants / len(wt_vs_mutant_df),
            f'Fraction {mutation_type}Mutations that are Deleterious'
                : num_less_fit_mutants  / len(wt_vs_mutant_df),
            f'Mean {mutation_type}Mutant Fitness Differential'
                : np.mean( wt_vs_mutant_df['Fitness Differential'] ),
            f'Median {mutation_type}Mutant Fitness Differential'
                : np.median( wt_vs_mutant_df['Fitness Differential'] ),
        })

    return pd.DataFrame(res)

def tabulate_perturbation(
    perturbation_df, control_fits_df, target_state, perturbation_type,
):

    intermittent_p_col = f'Intermittent {target_state} State {perturbation_type} Probability'

    if intermittent_p_col not in perturbation_df.columns:
        perturbation_df[intermittent_p_col] = 1

    if f'{target_state} State Target Idx' not in perturbation_df.columns:
        perturbation_df[f'{target_state} State Target Idx'] = range(
            len(perturbation_df)
        )

    # count competions where both strains went extinct simultaneously
    # as 0 Fitness Differential
    # filter prevents inclusion of intermittent_p_col nan or zero
    # (from root id 0)
    res_by_series = defaultdict(dict)
    for (series, intermittent_p), subset_df in perturbation_df[
        perturbation_df['Root ID'] == 1
    ].groupby([
        'Competition Series',
        intermittent_p_col,
    ]):

        idx_perturbed_df = subset_df[
            subset_df[f'{target_state} State Target Idx'].notnull()
        ].reset_index()

        # not doing anything with this data for now
        entire_perturbed_df = subset_df[
            subset_df[f'{target_state} State Target Idx'].isnull()
        ].reset_index()

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        wt_vs_perturbed_df = idx_perturbed_df

        # ensure that perturbation idxs are unique
        assert len(idx_perturbed_df) == len(
            idx_perturbed_df[f'{target_state} State Target Idx'].unique()
        )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(wt_vs_perturbed_df):
            wt_vs_perturbed_df['p'] =  wt_vs_perturbed_df.apply(
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
            wt_vs_perturbed_df['p'] = []


        p_thresh = 1.0 / 100
        num_more_fit_perturbations = (
            wt_vs_perturbed_df['p'] > 1 - p_thresh
        ).sum()
        num_less_fit_perturbations = (wt_vs_perturbed_df['p'] < p_thresh).sum()


        suffixes = [f' @ Intermittent P {intermittent_p}']
        if intermittent_p == 1: suffixes.append('')
        for suffix in suffixes:
            res_by_series[series].update({
                'Series' : series,
                f'Num More Fit Under {target_state} State {perturbation_type}s{suffix}' : num_more_fit_perturbations,
                f'Num Less Fit Under {target_state} State {perturbation_type}s{suffix}' : num_less_fit_perturbations,
                f'Fraction {target_state} State {perturbation_type}s that are Advantageous{suffix}'
                    : num_more_fit_perturbations / len(wt_vs_perturbed_df),
                f'Fraction {target_state} State {perturbation_type}s that are Deleterious{suffix}'
                    : num_less_fit_perturbations  / len(wt_vs_perturbed_df),
                f'Mean {target_state} State {perturbation_type} Fitness Differential{suffix}'
                    : np.mean( wt_vs_perturbed_df['Fitness Differential'] ),
                f'Median {target_state} State {perturbation_type} Fitness Differential{suffix}'
                    : np.median( wt_vs_perturbed_df['Fitness Differential'] ),
            })

    return pd.concat([
        pd.DataFrame.from_records( [just_one_series] )
        for just_one_series in res_by_series.values()
    ])

def tabulate_selfsend(
    selfsend_df, control_fits_df, messaging_target,
):

    # fixes typo in older data
    if f'{messaging_target} Message Self-Send Fitler Mod' in selfsend_df.columns:
        selfsend_df[f'{messaging_target} Message Self-Send Filter Mod'] = selfsend_df[f'{messaging_target} Message Self-Send Fitler Mod']

    # fixes typo in older data
    if f'{messaging_target} Message Self-Send Fitler Target' in selfsend_df.columns:
        selfsend_df[f'{messaging_target} Message Self-Send Filter Target'] = selfsend_df[f'{messaging_target} Message Self-Send Fitler Target']

    # count competions where both strains went extinct simultaneously
    # as 0 Fitness Differential
    res_by_series = defaultdict(dict)
    for (series, mod), wt_vs_selfsend_df in selfsend_df[
        selfsend_df['genome root_id'] == 1
    ].groupby([
        'Competition Series',
        f'{messaging_target} Message Self-Send Filter Mod',
    ]):

        assert all(wt_vs_selfsend_df[
            f'{messaging_target} Message Self-Send Filter Target'
        ].notnull())

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        # ensure that perturbation idxs are unique
        assert len(wt_vs_selfsend_df) == len(
            wt_vs_selfsend_df[f'{messaging_target} Message Self-Send Filter Target'].unique()
        )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(wt_vs_selfsend_df):
            wt_vs_selfsend_df['p'] =  wt_vs_selfsend_df.apply(
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
            wt_vs_selfsend_df['p'] = []


        p_thresh = 1.0 / 100
        num_more_fit_selfsends = (
            wt_vs_selfsend_df['p'] > 1 - p_thresh
        ).sum()
        num_less_fit_selfsends = (wt_vs_selfsend_df['p'] < p_thresh).sum()


        suffixes = [f' @ Fitler Mod {mod}']
        if len( selfsend_df[
            selfsend_df['genome root_id'] == 1
        ][
            f'{messaging_target} Message Self-Send Fitler Mod'
        ].unique() ) == 1:
            suffixes.append('')
        for suffix in suffixes:
            res_by_series[series].update({
                'Series' : series,
                f'Num More Fit Under {messaging_target} Self-Send{suffix}'
                    : num_more_fit_selfsends,
                f'Num Less Fit Under {messaging_target} Self-Send{suffix}'
                    : num_less_fit_selfsends,
                f'Fraction {messaging_target} Self-Sends that are Advantageous{suffix}'
                    : num_more_fit_selfsends / len(wt_vs_selfsend_df),
                f'Fraction {messaging_target} Self-Sends that are Deleterious{suffix}'
                    : num_less_fit_selfsends  / len(wt_vs_selfsend_df),
                f'Mean {messaging_target} Self-Send Fitness Differential{suffix}'
                    : np.mean( wt_vs_selfsend_df['Fitness Differential'] ),
                f'Median {messaging_target} Self-Send Fitness Differential{suffix}'
                    : np.median( wt_vs_selfsend_df['Fitness Differential'] ),
            })

    return pd.concat([
        pd.DataFrame.from_records( [just_one_series] )
        for just_one_series in res_by_series.values()
    ])

def tabulate_phenotype_equivalent_nopout_fitness( df, control_fits_df ):

    res = []
    for (series), series_df in df[
        df['genome root_id'] == 1
    ].groupby([
        'Competition Series',
    ]):

        assert len(series_df) == 1

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(series_df):
            series_df['p'] =  series_df.apply(
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
            series_df['p'] = []


        p_thresh = 1.0 / 100
        is_more_fit = (
            ip.popsingleton(series_df['p']) > 1 - p_thresh
        )
        is_less_fit = (
            ip.popsingleton(series_df['p']) < p_thresh
        )

        assert not ( is_more_fit and is_less_fit )

        res.append({
            'Series' : series,
            'Phenotype-Neutral Nopout Is More Fit' : is_more_fit,
            'Phenotype-Neutral Nopout Is Less Fit' : is_less_fit,
            'Phenotype-Neutral Nopout Is Neutral'
                : not (is_more_fit or is_less_fit),
            'Phenotype-Neutral Nopout Fitness' : is_more_fit - is_less_fit,
            'Phenotype-Neutral Nopout Fitness Differential'
                : ip.popsingleton( series_df['Fitness Differential'] ),
        })

    return pd.DataFrame(res)

def tabulate_phenotype_neutral_nopout_phenotype_differentiation( df ):

    assert all( df['first genome series'] == df['second genome series'] )
    df['Series'] = df['first genome series']

    assert all( df['First Root ID'] == 0 )
    assert all( df['Second Root ID'] == 1 )

    assert( len( df['Series'].unique() ) == len( df ) )

    df['Phenotype Neutral Nopout is Phenotypically Divergent'] = df[
        'Phenotype Divergence Detected'
    ] == 1
    df['Phenotype Neutral Nopout is Phenotypically Identical'] = df[
        'Phenotype Divergence Detected'
    ] == 0

    return df

def tabulate_wildtype_doubling_time( df ):

    df['Series'] = df['Competition Series']

    df['Doubling Time Growth Rate'] = np.power(
        df['Abundance'] / 0.25,
        1 / df['Update']
    )

    res = df.groupby([
        'Series',
    ])['Doubling Time Growth Rate'].mean().reset_index(
        name='Mean Doubling Time Growth Rate',
    )

    return res

def tabulate_mutant_phenotype_differentiation(mutant_df, mutation_type=''):

    assert all(
        mutant_df['first genome series'] == mutant_df['second genome series']
    )
    mutant_df['Series'] = mutant_df['first genome series']

    assert all( mutant_df['First Root ID'] == 0 )
    assert all( mutant_df['Second Root ID'] == 1 )

    phenotypically_expressed_col = (
        f'Fraction Phenotypically-Expressed {mutation_type}Mutations'
    )
    phenotypically_neutral_col = (
        f'Fraction Phenotypically-Neutral {mutation_type}Mutations'
    )

    res = mutant_df.groupby(
        ['Series'],
    )['Phenotype Divergence Detected'].mean().reset_index(
        name=phenotypically_expressed_col,
    )

    res[phenotypically_neutral_col] = 1.0 - res[phenotypically_expressed_col]

    return res


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
                    tabulate_perturbation(
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
                tabulate_selfsend(
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

        dataframes.append(
            tabulate_mutant( mutant_df, fit_control_t_distns(control_df) )
        )
        sources.append( mutant_competitions.key )
    except ValueError:
        print("missing mutant competitions, skipping")

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
