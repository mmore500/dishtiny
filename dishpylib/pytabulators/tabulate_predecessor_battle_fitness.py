from functools import reduce
import pandas as pd
from scipy import stats

def tabulate_predecessor_battle_fitness(predecessor_df, specifier=''):

    # root id 0 is the control competitors (i.e., the predecessors)
    predecessor_df = predecessor_df[
        predecessor_df['Root ID'] == 1
    ].reset_index()
    predecessor_df['Series'] = predecessor_df['genome series']

    mean_differential = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].mean().reset_index(
        name=f'Mean Fitness Differential Against {specifier}Predecessor Population',
    )

    median_differential = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].median().reset_index(
        name=f'Median Fitness Differential Against {specifier}Predecessor Population',
    )

    frac_won = predecessor_df.groupby(
        ['Series'],
    )['Fitness Differential'].apply(
        lambda just_one_series:
            (just_one_series > 0).sum() / len(just_one_series),
    ).reset_index(
        name=f'Fraction {specifier}Predecessor Battles Won',
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
        name=f'{specifier}Predecessor Battle Null p-value',
    )

    return reduce(
        lambda left, right: pd.merge(
            left,
            right,
            on='Series',
        ),
        [
            mean_differential,
            median_differential,
            frac_won,
            null_p,
        ],
    )
