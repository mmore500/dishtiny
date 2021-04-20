from keyname import keyname as kn

def reshape_kin_conflict(df):

    just_one_thread = df.groupby([
        'Stint',
        'Series',
        'Parent-Eliminated Kin ID Commonality',
    ]).first().reset_index()

    # move Parent-Eliminated Kin ID Commonality
    # into a hierarchically-nested column name
    res = just_one_thread.pivot(
        index=['Stint', 'Series',],
        columns='Parent-Eliminated Kin ID Commonality',
    ).reset_index()

    # collapse hierarchically-nested columns into keyname-packed strings
    res.columns = res.columns.map(
        lambda x: kn.pack({
            'a' : x[0],
            'Parent-Eliminated Kin ID Commonality' : x[1],
        }) if x[1] != "" else x[0]
    )

    return res
