from functools import reduce
from keyname import keyname as kn
import pandas as pd

def _extract_occurence_column(df, target, statistic):
    res = df.value_counts(
        [
            'Series',
            'Stint',
            target,
        ],
        normalize=(statistic == 'Frequency'),
    ).reset_index(
        name=statistic,
    ).pivot(
        index=['Stint', 'Series'],
        columns=target,
    ).reset_index()
    # collapse hierarchically-nested columns into keyname-packed strings
    res.columns = res.columns.map(
        lambda x: kn.pack({
            target : x[1],
            'Statistic' : statistic,
            'What' : 'birth',
        }) if x[1] != "" else x[0]
    )
    return res


def reshape_birth_log(df):

    extracted_columns = [
        _extract_occurence_column(df, target, statistic)
        for target in [
            'Daughter-Eliminated Kin ID Commonality',
            'Parent-Daughter Kin ID Commonality',
            'Parent-Eliminated Kin ID Commonality',
            'Eliminated Cell Level 0 Peripherality',
            'Eliminated Cell Level 1 Peripherality',
        ]
        for statistic in ['Count', 'Frequency']
    ]

    return reduce(
        lambda left, right: pd.merge(
            left,
            right,
            on=[
                'Series',
                'Stint',
            ],
        ),
        extracted_columns,
    )
