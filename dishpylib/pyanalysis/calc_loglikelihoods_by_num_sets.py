import math
import numpy as np

from .count_hands_with_k_or_more_sets import count_hands_with_k_or_more_sets
from .count_hands_without_k_or_more_sets import count_hands_without_k_or_more_sets

def _count_nop_targetings_consistent_with_observation(
        *,
        num_nopped,
        num_sets,
        num_sites,
        set_size,
        was_significantly_divergent,
    ):

    res = (
        count_hands_with_k_or_more_sets
        if was_significantly_divergent
        else count_hands_without_k_or_more_sets
    )(
        deck_size=num_sites,
        hand_size=num_nopped,
        num_sets=num_sets,
        set_size=set_size,
        k=1,
    )

    assert res >= 0
    assert res <= math.comb(num_sites, num_nopped)

    return res


def _calc_loglikelihood_of_observations(
        *,
        interpolation_competitions_df,
        num_sets,
        series,
        set_size,
    ):

    # filter for just this series,
    # from the point of view of the nop-interpolated variants
    # not the controls which have nans for num nopped
    fil = interpolation_competitions_df[
        interpolation_competitions_df['genome series'] == series
    ].dropna(
        subset=['genome nop_interpolation_num_nopped']
    )
    num_sites = int(fil['genome nop_interpolation_num_nopped'].max())

    # sum logp of observations assuming num_sets, set_size
    total_logp = 0.0
    for idx, row in fil.iterrows():

        num_nopped = int(row['genome nop_interpolation_num_nopped'])

        res = _count_nop_targetings_consistent_with_observation(
            num_nopped=num_nopped,
            num_sets=num_sets,
            num_sites=num_sites,
            set_size=set_size,
            was_significantly_divergent=not row['Is Neutral'],
        )

        if res:
            # log-land equivalent of
            # *= num_nop_targetings_for_obvs / total_num_nop_targetings
            total_logp += (
                math.log(res)
                - math.log(math.comb(num_sites, num_nopped))
            )
        else:
            # if we have a zero-probabilty observation, bail and return nan
            return math.nan

    return total_logp


def calc_loglikelihoods_by_num_sets(
        *,
        interpolation_competitions_df,
        series,
        set_size,
    ):

    num_sites = int(max(filter(
        np.isfinite,
        interpolation_competitions_df[
            interpolation_competitions_df['genome series'] == series
        ]['genome nop_interpolation_num_nopped'],
    )))
    max_num_sets = num_sites // set_size

    loglikelihoods = []
    for num_sets in range(max_num_sets):

        cur = _calc_loglikelihood_of_observations(
            interpolation_competitions_df=interpolation_competitions_df,
            num_sets=num_sets,
            series=series,
            set_size=set_size,
        )

        loglikelihoods.append( cur )

        # if 1000x less likely than best num_sets seen so far,
        # larger num_sets won't be worth investigating so bail early
        # (assumes that distribution of loglikelihoods is unimodal)
        if any(map(np.isfinite, loglikelihoods)) \
            and cur < max(filter(np.isfinite, loglikelihoods)) - np.log(1000):
            break

    return loglikelihoods
