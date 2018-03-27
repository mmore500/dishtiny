import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
import scipy.stats as stats
from tqdm import tqdm

# CATEGORIZE OUTCOMES USING CHAMPION GENOTYPES

# read in all census data
df_in = pd.concat(
        [pd.read_csv(f) for f in glob.glob("Census_*.csv")],
        ignore_index=True
    )

# filter for last update
fil_upd = df_in[df_in['update'] == df_in['update'].max()].reset_index(drop=True)

# loop through seeds to genotype for eadh with largest count

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

# iterate through rows of the champion genotype dataframe
# and count how many are zero, first, second level genotypes

count = [0,0,0]

for __, row in champions_df.iterrows():
    # zero level
    if (
            row['res_pool0'] > row['res_pool1'] and
            row['res_pool0'] > row['res_pool2']
        ):
        count[0] += 1
    # first level
    elif (
            row['res_pool1'] > row['res_pool0'] and
            row['res_pool1'] > row['res_pool2']
        ):
        count[1] += 1
    # second level
    elif (
            row['res_pool2'] > row['res_pool0'] and
            row['res_pool2'] > row['res_pool1']
        ):
        count[2] += 1
    else:
        assert True, "tie in genotype catergorization!"

print("Count of zero, first, second level genotypes among champions:")
print(count)

# CATEGORIZE OUTCOMES USING MEAN GENOTYPE

# read in all mean genotype data
df_in = pd.concat(
        [pd.read_csv(f) for f in glob.glob("Genotypes_*.csv")], ignore_index=True
    )

# filter for last update
means_df = df_in[
        df_in['update'] == df_in['update'].max()
    ].reset_index(drop=True)

# iterate through rows of the mean genotype dataframe
# and count how many averages are zero, first, second level

count = [0,0,0]

for __, row in means_df.iterrows():
    if (
            row['mean_res_pool0'] > row['mean_res_pool1'] and
            row['mean_res_pool0'] > row['mean_res_pool2']
        ):
        count[0] += 1
    elif (
            row['mean_res_pool1'] > row['mean_res_pool0'] and
            row['mean_res_pool1'] > row['mean_res_pool2']
        ):
        count[1] += 1
    elif (
            row['mean_res_pool2'] > row['mean_res_pool0'] and
            row['mean_res_pool2'] > row['mean_res_pool1']
        ):
        count[2] += 1
    else:
        assert True, "tie in genotype catergorization!"

print("Count of zero, first, second level outcomes by mean genotype:")
print(count)
