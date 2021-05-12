from iterpop import iterpop as ip
import itertools as it
import math
import numpy as np
import pandas as pd

from .calc_loglikelihoods_by_num_sets import calc_loglikelihoods_by_num_sets

def calc_loglikelihoods_over_set_sizes(
        *,
        series,
        interpolation_competitions_df,
    ):

    # filter for just this series,
    # from the point of view of the nop-interpolated variants
    # not the controls which have nans for num nopped
    fil = interpolation_competitions_df[
        interpolation_competitions_df['genome series'] == series
    ].dropna(
        subset=['genome nop_interpolation_num_nopped'],
    )

    none_nopped_is_less_fit = ip.popsingleton(fil[
        fil['genome nop_interpolation_num_nopped'] == 0
    ]['Is Less Fit'])

    all_nopped_is_less_fit = ip.popsingleton(fil[
        fil['genome nop_interpolation_num_nopped']
        == max(fil['genome nop_interpolation_num_nopped'])
    ]['Is Less Fit'])

    if none_nopped_is_less_fit or not all_nopped_is_less_fit:
        return pd.DataFrame(
            columns=[
                'set_size',
                'num_sets',
                'loglikelihood',
                'likelihood',
            ],
        )

    res = []
    best_loglikelihood = -math.inf
    for set_size in it.count(start=1):
        loglikelihoods = calc_loglikelihoods_by_num_sets(
            interpolation_competitions_df=interpolation_competitions_df,
            series=series,
            set_size=set_size,
        )

        for num_sets, loglikelihood in enumerate(loglikelihoods):
            res.append({
                'set_size' : set_size,
                'num_sets' : num_sets,
                'loglikelihood' : loglikelihood,
                'likelihood' : np.exp(loglikelihood),
            })

        if not any(map(np.isfinite, loglikelihoods)):
            break

        cur_best_loglikelihood = max(filter(np.isfinite, loglikelihoods))
        if cur_best_loglikelihood < best_loglikelihood:
            break
        else:
            best_loglikelihood = cur_best_loglikelihood

    return pd.DataFrame(res)
