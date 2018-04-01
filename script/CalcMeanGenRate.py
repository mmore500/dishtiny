import pandas as pd
import numpy as np
import glob

# calculate the number of comulative generations that have passed
# at a particular update for a particular seed

# open phenotype, population dataframes

dfph = pd.concat([pd.read_csv(f) for f in glob.glob("Phenotypes_*.csv")], ignore_index=True)

dfpo = pd.concat([pd.read_csv(f) for f in glob.glob("Population_*.csv")], ignore_index=True)

# merge phenotype, population dataframes

res = dfph.merge(dfpo, how='left', on=['seed', 'update'])

# add column calculating number of elapsed generations...
res['num_gens'] = res['total_reproduce'] / res['num_orgs']

# how many updates between recordings?
phstep = np.sort(res['update'].unique())[1] - np.sort(res['update'].unique())[0]

# add column calculating number updates per cell-level generation
res['cell_generation'] = res.apply(
        lambda row: row['num_orgs']*phstep/row['total_reproduce'],
        axis=1
        )

# print diagnostic information

print("Mean number generations per %d updates:" % phstep)
print(res['num_gens'].mean())
print("Standard deviation number generations per %d updates:" % phstep)
print(res['num_gens'].std())

print("Mean number cell generation length in updates:")
print(res['cell_generation'].mean())
print("Standard deviation cell generation length in updates:")
print(res['cell_generation'].std())
print("Maximum cell generation length in updates:")
print(res['cell_generation'].max())
