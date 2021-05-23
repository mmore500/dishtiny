from iterpop import iterpop as ip

def validate_assembled_dataframe( df, endeavor, stint ):

    # there should only be one entry for each series/stint
    assert len(df.groupby([
        'Series',
        'Stint',
    ])) == len(df)
    assert ip.pophomogeneous(df['Series'] // 1000) == endeavor
    assert ip.pophomogeneous(df['Stint']) == stint
