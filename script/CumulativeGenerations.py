import pandas as pd
import glob
from tqdm import tqdm
from sys import argv

# calculate the number of comulative generations that have passed
# at a particular update for a particular seed

# setup arguments
assert len(argv) == 3, "Two arguments expected: target seed and target update."
target_seed = int(argv[1])
target_update = int(argv[2])

# setup progress bar for pandas
tqdm.pandas(tqdm())

# open phenotypes, populations dtaframes and filter for target seed

dfph = pd.concat([pd.read_csv(f) for f in glob.glob("Phenotypes_*.csv")], ignore_index=True)

dfph = dfph[dfph['seed'] == target_seed]

dfpo = pd.concat([pd.read_csv(f) for f in glob.glob("Population_*.csv")], ignore_index=True)

dfpo = dfpo[dfpo['seed'] == target_seed]

# merge phenotype and population dataframes

res = dfph.merge(dfpo, how='left', on=['seed', 'update'])

# new column:
# calculate number generations...

res['num_gens'] = res['total_reproduce'] / res['num_orgs']

# new column:
# calculate cumulative generations as cumulative sum of `num_gens` values

res['cum_gen'] = res.progress_apply(
        lambda x: res[res['update'] <= x['update']]['num_gens'].sum(),
        axis=1
    )

# print result

print(res[['update','cum_gen']][res['update'] == target_update])
