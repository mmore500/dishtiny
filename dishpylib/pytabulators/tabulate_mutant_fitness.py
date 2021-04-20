from iterpop import iterpop as ip
import numpy as np
import pandas as pd
from scipy import stats

def tabulate_mutant_fitness(mutant_df, control_fits_df, mutation_type=''):

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
