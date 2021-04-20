from collections import defaultdict
from iterpop import iterpop as ip
import numpy as np
import pandas as pd
from scipy import stats

def tabulate_selfsend_fitness(
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


        suffixes = [f' @ Filter Mod {mod}']
        if len( selfsend_df[
            selfsend_df['genome root_id'] == 1
        ][
            f'{messaging_target} Message Self-Send Filter Mod'
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
