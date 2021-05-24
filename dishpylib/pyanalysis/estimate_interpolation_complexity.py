from iterpop import iterpop as ip
import itertools as it
from keyname import keyname as kn
import math
import numpy as np

from .calc_loglikelihoods_by_num_sets import calc_loglikelihoods_by_num_sets
from .estimate_credible_interval import estimate_credible_interval

def estimate_interpolation_complexity(
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

    res = {
        'none_nopped_is_less_fit' : (
            ip.popsingleton(fil[
                fil['genome nop_interpolation_num_nopped'] == 0
            ]['Is Less Fit'])
        ),
        'all_nopped_is_less_fit' : (
            ip.popsingleton(fil[
                fil['genome nop_interpolation_num_nopped']
                == max(fil['genome nop_interpolation_num_nopped'])
            ]['Is Less Fit'])
        ),
        'credible_interval_lower_bound_idx' : math.nan,
        'most_credible_idx' : math.nan,
        'most_credible_likelihood' : math.nan,
        'most_credible_set_size' : math.nan,
        'interpolation_complexity' : math.nan,
        'credible_interval_upper_bound_idx' : math.nan,
        'Series' : series,
    }

    if res['none_nopped_is_less_fit'] or not res['all_nopped_is_less_fit']:
        return res

    for set_size in it.count(start=1):
        loglikelihoods = calc_loglikelihoods_by_num_sets(
            interpolation_competitions_df=interpolation_competitions_df,
            series=series,
            set_size=set_size,
        )
        likelihoods = [np.exp(loglike) for loglike in loglikelihoods]

        cur = estimate_credible_interval(likelihoods)

        for k, v in cur.items():
            res[ kn.pack({
                'a': k,
                'set_size' : str(set_size),
            }) ] = v

        if math.isnan(res['most_credible_likelihood']) \
            or cur['most_credible_likelihood'] \
                > res['most_credible_likelihood']:

            res['most_credible_set_size'] = set_size
            # might consider multiplying this by set_size
            res['interpolation_complexity'] = cur['most_credible_idx']

            if math.isnan(cur['most_credible_likelihood']):
                return res

            # should update 'most_credible_likelihood' and other fields
            res.update(cur)

        else:
            # likelihood was worse for this set_size
            # don't investigate further set_sizes
            break

    return res
