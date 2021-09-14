import functools
from iterpop import iterpop as ip
from scipy import stats

def preprocess_competition_fitnesses(competitions_df, control_fits_df):
    # preprocess data
    @functools.lru_cache
    def h0_fit(series):
        return ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

    competitions_df['p'] =  competitions_df.apply(
        lambda row: stats.t.cdf(
            row['Fitness Differential'],
            h0_fit(row['genome series'])['Fit Degrees of Freedom'],
            loc=h0_fit(row['genome series'])['Fit Loc'],
            scale=h0_fit(row['genome series'])['Fit Scale'],
        ),
        axis=1,
    )
    competitions_df['Is Less Fit'] = competitions_df['p'] < 1.0 / 500
    competitions_df['Is More Fit'] = competitions_df['p'] > (1.0 - 1.0 / 500)
    competitions_df['Is Neutral'] = ~(
        competitions_df['Is Less Fit'] | competitions_df['Is More Fit']
    )
    competitions_df['Relative Fitness'] = competitions_df.apply(
        lambda row: \
            'Significantly Advantageous' if row['Is More Fit'] \
            else 'Significantly Deleterious' if row['Is Less Fit'] \
            else 'Neutral',
        axis=1,
    )

    return competitions_df
