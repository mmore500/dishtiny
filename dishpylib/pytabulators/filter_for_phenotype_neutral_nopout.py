def filter_for_phenotype_neutral_nopout(genome_df):
    res = genome_df[
        genome_df['genome morph'] == 'phenotype_equivalent_nopout'
    ].reset_index()
    res['Phenotype Complexity'] = res['Num Op Instructions']
    return res
