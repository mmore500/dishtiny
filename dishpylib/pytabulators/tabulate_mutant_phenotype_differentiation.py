def tabulate_mutant_phenotype_differentiation(mutant_df, mutation_type=''):

    assert all(
        mutant_df['first genome series'] == mutant_df['second genome series']
    )
    mutant_df['Series'] = mutant_df['first genome series']

    assert all( mutant_df['First Root ID'] == 0 )
    assert all( mutant_df['Second Root ID'] == 1 )

    phenotypically_expressed_col = (
        f'Fraction Phenotypically-Expressed {mutation_type}Mutations'
    )
    phenotypically_neutral_col = (
        f'Fraction Phenotypically-Neutral {mutation_type}Mutations'
    )

    res = mutant_df.groupby(
        ['Series'],
    )['Phenotype Divergence Detected'].mean().reset_index(
        name=phenotypically_expressed_col,
    )

    res[phenotypically_neutral_col] = 1.0 - res[phenotypically_expressed_col]

    return res
