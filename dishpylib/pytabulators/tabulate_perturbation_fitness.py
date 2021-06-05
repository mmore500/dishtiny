from collections import defaultdict
from iterpop import iterpop as ip
import numpy as np
import pandas as pd
from scipy import stats

def tabulate_perturbation_fitness(
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

            for target_idx, group in idx_perturbed_df.groupby(
                f'{target_state} State Target Idx'
            ):
                res_by_series[series].update({
                    f'Is More Fit Under {target_state} State {int(target_idx)} {perturbation_type}s{suffix}'
                        : ip.popsingleton(group['p']) > 1 - p_thresh,
                    f'Is Less Fit Under {target_state} State {int(target_idx)} {perturbation_type}s{suffix}'
                        : ip.popsingleton(group['p']) < p_thresh,
                    f'{target_state} State {int(target_idx)} {perturbation_type} Fitness Differential{suffix}'
                        : ip.popsingleton(group['Fitness Differential']) < p_thresh,
                })

    return pd.concat([
        pd.DataFrame.from_records( [just_one_series] )
        for just_one_series in res_by_series.values()
    ])
