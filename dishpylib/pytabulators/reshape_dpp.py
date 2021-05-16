def reshape_dpp(df):

    last_update_idxs = df.groupby([
        'Stint',
        'Series',
        'Metric',
        'proc',
        'thread',
    ])['Update'].transform(max) == df['Update']

    # move "Metric" values into columns
    return df[ last_update_idxs ].pivot(
        index=[
            'Stint',
            'Series',
            'proc',
            'thread',
            'Update',
        ],
        columns='Metric',
        values='Value',
    ).reset_index()
