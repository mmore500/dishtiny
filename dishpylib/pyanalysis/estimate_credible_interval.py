import math
import operator

def estimate_credible_interval(likelihoods):

    # drop na
    valid_likelihoods = [
        (idx, likelihood)
        for idx, likelihood in enumerate(likelihoods)
        if not math.isnan(likelihood)
    ]

    # enumerate and sort by descending likelihood
    descending_likelihoods = sorted(
        valid_likelihoods,
        key=operator.itemgetter(1),
        reverse=True,
    )

    total_likelihood = sum(
        likelihood
        for idx, likelihood in valid_likelihoods
    )

    # accept idx's until 95% of total likelihood is accounted for
    ci_indexes = []
    running_likelihood_sum = 0.0
    for idx, likelihood in descending_likelihoods:
        running_likelihood_sum += likelihood
        ci_indexes.append(idx)

        if running_likelihood_sum > 0.95 * total_likelihood:
            break

    return {
        'credible_interval_lower_bound_idx' : min(ci_indexes or [math.nan]),
        'most_credible_idx' : (
            descending_likelihoods[0][0]
            if descending_likelihoods
            else math.nan
        ),
        'most_credible_likelihood' : (
            descending_likelihoods[0][1]
            if descending_likelihoods
            else math.nan
        ),
        'credible_interval_upper_bound_idx' : max(ci_indexes or [math.nan]),
    }
