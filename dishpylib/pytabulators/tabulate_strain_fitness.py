from functools import reduce
import pandas as pd
from scipy import stats

def tabulate_strain_fitness(strain_df):

    strain_df['Series'] = strain_df['genome series']
    strain_df['Stint'] = strain_df['genome stint']

    mean_differential = strain_df.groupby([
        'Series',
        'Stint',
    ])['Fitness Differential'].mean().reset_index(
        name='Mean Inter-Strain Fitness Differential',
    )

    frac_won = strain_df.groupby([
        'Series',
        'Stint',
    ])['Fitness Differential'].apply(
        lambda just_one_series:
            (just_one_series > 0).sum() / len(just_one_series),
    ).reset_index(
        name='Fraction Inter-Strain Competitions Won',
    )

    null_p = strain_df.groupby([
        'Series',
        'Stint',
    ])['Fitness Differential'].apply(
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
