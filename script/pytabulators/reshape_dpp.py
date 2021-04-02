def reshape_dpp(df):

    just_one_thread = df.groupby([
        'Stint',
        'Series',
        'Metric',
        'Update',
    ]).first().reset_index()

    last_update_idxs = just_one_thread.groupby([
        'Stint',
        'Series',
        'Metric',
    ])['Update'].transform(max) == just_one_thread['Update']

    # move "Metric" values into columns
    return just_one_thread[ last_update_idxs ].pivot(
        index=['Stint', 'Series',],
        columns='Metric',
        values='Value',
    ).reset_index()
