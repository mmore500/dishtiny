import pandas as pd
from scipy import stats

def fit_control_t_distns(control_df):

    na_rows = control_df['Fitness Differential'].isna()
    assert all( control_df[ na_rows ]['Population Extinct'] )
    control_df['Fitness Differential'].fillna(0, inplace=True,)

    res = []
    for series in control_df['Competition Series'].unique():

        series_df = control_df[ control_df['Competition Series'] == series ]

        # legacy data was mixed inside of the variant_df
        wt_vs_wt_df = series_df.groupby('Competition Repro').filter(
            lambda x: (x['genome variation'] == 'master').all()
        ).groupby('Competition Repro').first().reset_index()

        # fit a t distribution to the control data
        # df is degrees of freedom
        df, loc, scale = stats.t.fit( wt_vs_wt_df['Fitness Differential'] )


        res.append({
            'Series' : series,
            'Fit Degrees of Freedom' : df,
            'Fit Loc' : loc,
            'Fit Scale' : scale,
        })

    return pd.DataFrame(res)
