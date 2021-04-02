def tabulate_phenotype_neutral_nopout_phenotype_differentiation( df ):

    assert all( df['first genome series'] == df['second genome series'] )
    df['Series'] = df['first genome series']

    assert all( df['First Root ID'] == 0 )
    assert all( df['Second Root ID'] == 1 )

    assert( len( df['Series'].unique() ) == len( df ) )

    df['Phenotype Neutral Nopout is Phenotypically Divergent'] = df[
        'Phenotype Divergence Detected'
    ] == 1
    df['Phenotype Neutral Nopout is Phenotypically Identical'] = df[
        'Phenotype Divergence Detected'
    ] == 0

    return df
