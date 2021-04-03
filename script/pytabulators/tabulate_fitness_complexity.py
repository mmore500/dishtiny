from iterpop import iterpop as ip
import pandas as pd
from scipy import stats

def tabulate_fitness_complexity(variant_df, control_fits_df):

    # count competions where both strains went extinct simultaneously
    # as 0 Fitness Differential
    na_rows = variant_df['Fitness Differential'].isna()
    assert all( variant_df[ na_rows ]['Population Extinct'] )
    variant_df['Fitness Differential'].fillna(0, inplace=True,)

    res = []
    for series in variant_df['Competition Series'].unique():

        series_df = variant_df[ variant_df['Competition Series'] == series ]

        wt_vs_variant_df = series_df[
            series_df['genome variation'] != 'master'
        ].reset_index()

        h0_fit = ip.popsingleton( control_fits_df[
            control_fits_df['Series'] == series
        ].to_dict(
            orient='records',
        ) )

        # calculate the probability of observing fitness differential result
        # under control data distribution
        if len(wt_vs_variant_df):
            wt_vs_variant_df['p'] =  wt_vs_variant_df.apply(
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
            wt_vs_variant_df['p'] = []


        p_thresh = 1.0 / 100
        num_more_fit_variants = (wt_vs_variant_df['p'] > 1 - p_thresh).sum()
        num_less_fit_variants = (wt_vs_variant_df['p'] < p_thresh).sum()

        expected_false_positives = len(wt_vs_variant_df) * p_thresh

        res.append({
            'Stint' : ip.pophomogeneous( variant_df['Competition Stint'] ),
            'Series' : series,
            'Flagged Advantageous Sites' : num_less_fit_variants,
            'Flagged Deleterious Sites' : num_more_fit_variants,
            'H_0 Advantageous Site Flags' : expected_false_positives,
            'H_0 Deleterious Site Flags' : expected_false_positives,
            'Estimated True Advantageous Sites'
                : num_less_fit_variants - expected_false_positives,
            'Estimated True Deleterious Sites'
                : num_more_fit_variants - expected_false_positives,
        })

    return pd.DataFrame(res)
