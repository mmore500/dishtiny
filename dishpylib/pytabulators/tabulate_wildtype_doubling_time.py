import numpy as np

def tabulate_wildtype_doubling_time( df ):

    df['Series'] = df['Competition Series']

    df['Doubling Time Growth Rate'] = np.power(
        df['Abundance'] / 0.25,
        1 / df['Update']
    )

    res = df.groupby([
        'Series',
    ])['Doubling Time Growth Rate'].mean().reset_index(
        name='Mean Doubling Time Growth Rate',
    )

    return res
