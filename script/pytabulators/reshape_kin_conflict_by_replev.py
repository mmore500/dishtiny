from keyname import keyname as kn

def reshape_kin_conflict_by_replev(df):

    # move Parent-Eliminated Kin ID Commonality and Replev
    # into a hierarchically-nested column name
    just_one_thread = df.groupby([
        'Stint',
        'Series',
        'Parent-Eliminated Kin ID Commonality',
        'Replev',
    ]).first().reset_index()

    res = just_one_thread.pivot(
        index=['Stint', 'Series',],
        columns=[
            'Parent-Eliminated Kin ID Commonality',
            'Replev',
        ],
    ).reset_index()

    # collapse hierarchically-nested columns into keyname-packed strings
    res.columns = res.columns.map(
        lambda x: kn.pack({
            'a' : x[0],
            'Parent-Eliminated Kin ID Commonality' : x[1],
            'Replev' : x[2],
        }) if x[1] != "" and x[2] != "" else x[0]
    )

    return res
