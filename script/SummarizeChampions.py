import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
import scipy.stats as stats
from tqdm import tqdm

pd.set_option('display.max_columns',None)

# CATEGORIZE OUTCOMES USING CHAMPION GENOTYPES

# read in all census data
df_in = pd.concat(
        [pd.read_csv(f,comment='#') for f in glob.glob("Census_*.csv")],
        ignore_index=True
    )

# filter for last update
fil_upd = df_in[df_in['update'] == df_in['update'].max()].reset_index(drop=True)

# loop through seeds to genotype for each with largest count

champions_list = []
dominance_list = []

for s in tqdm(fil_upd['seed'].unique()):
    # grab entries for particular seed
    s_df = fil_upd[fil_upd['seed'] == s].reset_index(drop=True)
    # filter for entry with larget count (dominant genotype)
    fil_count = s_df.iloc[[s_df['count'].idxmax()]].reset_index(drop=True)
    champions_list.append(fil_count)
    # what proportion of the population has the dominant genotye?
    dominance_list.append(s_df['count'].max()/s_df['count'].sum())

print("Dominance of champion for each trial:")
print(dominance_list)

champions_df = pd.concat(champions_list, ignore_index=True)
print(champions_df)

pure_l1 = champions_df[(champions_df['res_pool1'] == 1.0)];
l1 = champions_df[(champions_df['res_pool1'] != 1.0) & (champions_df['res_pool1'] > champions_df['res_pool2'])];
l2 = champions_df[(champions_df['res_pool1'] <= champions_df['res_pool2'])];

def r(f):
    return "{0:.2f}".format(round(f,2))

print('pure_l1', len(pure_l1))
for column in pure_l1:
    print(column,r(pure_l1[column].mean()),r(pure_l1[column].std()))

print()

print('l1', len(l1))
for column in l1:
    print(column,r(l1[column].mean()),r(l1[column].std()))

print()

print('l2', len(l2))
for column in l2:
    print(column,r(l2[column].mean()),r(l2[column].std()))

print()

print('champions_df', len(champions_df))
for column in champions_df:
    print(column,r(champions_df[column].mean()),r(champions_df[column].std()))
