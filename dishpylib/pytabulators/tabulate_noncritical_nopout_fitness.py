from iterpop import iterpop as ip
import numpy as np
import pandas as pd
from scipy import stats

def tabulate_noncritical_nopout_fitness( df, control_fits_df ):

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
            'Noncritical Nopout Is More Fit' : is_more_fit,
            'Noncritical Nopout Is Less Fit' : is_less_fit,
            'Noncritical Nopout Is Neutral'
                : not (is_more_fit or is_less_fit),
            'Noncritical Nopout Fitness' : is_more_fit - is_less_fit,
            'Noncritical Nopout Fitness Differential'
                : ip.popsingleton( series_df['Fitness Differential'] ),
        })

    return pd.DataFrame(res)
