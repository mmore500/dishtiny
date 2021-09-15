from collections import defaultdict
from iterpop import iterpop as ip
import numpy as np
import pandas as pd
from scipy import stats

def tabulate_either_perturbation_fitness(
    exchange_df, rotate_df, control_fits_df, target_state
):

    exchange_df[ 'Perturbation' ] = 'Exchange'
    exchange_df[ 'Perturbation P' ] = exchange_df[
        f'Intermittent {target_state} State Exchange Probability'
    ]

    rotate_df[ 'Perturbation' ] = 'Rotate'
    rotate_df[ 'Perturbation P' ] = rotate_df[
        f'Intermittent {target_state} State Rotate Probability'
    ]

    perturbation_df = pd.concat([ exchange_df, rotate_df ])

    # count competions where both strains went extinct simultaneously
    # as 0 Fitness Differential
    # filter prevents inclusion of intermittent_p_col nan or zero
    # (from root id 0)
    res_by_series = defaultdict(dict)
    for (series, intermittent_p), subset_df in perturbation_df[
        perturbation_df['Root ID'] == 1
    ].groupby([
        'Competition Series',
        'Perturbation P',
    ]):

        idx_perturbed_df = subset_df[
            subset_df[f'{target_state} State Target Idx'].notnull()
        ].reset_index()

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        wt_vs_perturbed_df = idx_perturbed_df

        # ensure that perturbation idxs are unique
        # one each for exchange and rotate
        num_rows = len(idx_perturbed_df)
        num_idxs = len(
            idx_perturbed_df[f'{target_state} State Target Idx'].unique()
        )
        assert num_rows == 2 * num_idxs, f'{num_rows} {num_idxs}'

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

        wt_vs_perturbed_df[
            'Is Significantly More Fit'
        ] = wt_vs_perturbed_df['p'] > 1 - p_thresh
        wt_vs_perturbed_df[
            'Is Significantly Less Fit'
        ] = wt_vs_perturbed_df['p'] < p_thresh

        num_more_fit_perturbations = (
            wt_vs_perturbed_df.groupby(
                f'{target_state} State Target Idx'
            )['Is Significantly More Fit'].any()
        ).sum()
        num_less_fit_perturbations = (
            wt_vs_perturbed_df.groupby(
                f'{target_state} State Target Idx'
            )['Is Significantly Less Fit'].any()
        ).sum()

        suffixes = [f' @ Intermittent P {intermittent_p}']
        if intermittent_p == 1: suffixes.append('')
        for suffix in suffixes:
            res_by_series[series].update({
                'Series' : series,
                f'Num More Fit Under {target_state} State Perturbation{suffix}' : num_more_fit_perturbations,
                f'Num Less Fit Under {target_state} State Perturbation{suffix}' : num_less_fit_perturbations,
                f'Fraction {target_state} State Perturbations that are Advantageous{suffix}'
                    : num_more_fit_perturbations / len(wt_vs_perturbed_df),
                f'Fraction {target_state} State Perturbations that are Deleterious{suffix}'
                    : num_less_fit_perturbations  / len(wt_vs_perturbed_df),
                f'Mean {target_state} State Perturbation Fitness Differential{suffix}'
                    : np.mean( wt_vs_perturbed_df['Fitness Differential'] ),
                f'Median {target_state} State Perturbation Fitness Differential{suffix}'
                    : np.median( wt_vs_perturbed_df['Fitness Differential'] ),
            })

            for target_idx, group in wt_vs_perturbed_df.groupby(
                f'{target_state} State Target Idx'
            ):
                res_by_series[series].update({
                    f'Is More Fit Under {target_state} State {int(target_idx)} Perturbation{suffix}'
                        : group['Is Significantly More Fit'].any(),
                    f'Is Less Fit Under {target_state} State {int(target_idx)} Perturbation{suffix}'
                        : group['Is Significantly Less Fit'].any(),
                })

    return pd.concat([
        pd.DataFrame.from_records( [just_one_series] )
        for just_one_series in res_by_series.values()
    ])
