from functools import reduce
import pandas as pd
from scipy import stats

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
