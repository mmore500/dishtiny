import matplotlib
matplotlib.use('Agg')
import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
import scipy.stats as stats
from tqdm import tqdm

from corboot import corboot

# open-type fonts
matplotlib.rcParams['pdf.fonttype'] = 42

# LOAD CHAMPION GENOTYPE DATA

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

# LOAD MEAN GENOTYPE DATA

# read in all mean genotype data
df_in = pd.concat(
        [pd.read_csv(f) for f in glob.glob("Genotypes_*.csv")], ignore_index=True
    )

# filter for last update
means_df = df_in[
        df_in['update'] == df_in['update'].max()
    ].reset_index(drop=True)

# ANALYZE RES_POOL, DAMAGE_SUICIDE CORRELATIONS

# using champion genotype data

################################################################################

ax = sns.regplot(champions_df['res_pool1'], champions_df['damage_suicide0'])
lab = "Zeroth-level Resource Sharing and Cell-level Altruistic Suicide"
ax.set_title(lab)
ax.set_xlabel("Dominant Genotype $P_0$")
ax.set_ylabel("Dominant Genotype $M_c$")

plt.savefig("champion_res_pool1_vs_champion_damage_suicide0.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for champion %s:" % lab)
corboot(champions_df[['res_pool1','damage_suicide0']])

################################################################################

ax = sns.regplot(champions_df['res_pool2'], champions_df['damage_suicide0'])
lab = "First-level Resource Sharing and Cell-level Altruistic Suicide"
ax.set_title(lab)
ax.set_xlabel("Dominant Genotype $P_1$")
ax.set_ylabel("Dominant Genotype $M_c$")

plt.savefig("champion_res_pool2_vs_champion_damage_suicide0.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for champion %s:" % lab)
corboot(champions_df[['res_pool2','damage_suicide0']])

################################################################################

# using mean genotype data

################################################################################

ax = sns.regplot(means_df['mean_res_pool1'], means_df['mean_damage_suicide0'])
lab = "Zeroth-level Resource Sharing and Cell-level Altruistic Suicide"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Population Mean $M_c$")

plt.savefig("mean_res_pool1_vs_mean_damage_suicide0.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(means_df[['mean_res_pool1','mean_damage_suicide0']])

################################################################################

ax = sns.regplot(means_df['mean_res_pool2'], means_df['mean_damage_suicide0'])
lab = "First-level Resource Sharing and Cell-level Altruistic Suicide"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Population Mean $M_c$")

plt.savefig("mean_res_pool2_vs_mean_damage_suicide0.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(means_df[['mean_res_pool2','mean_damage_suicide0']])

################################################################################

# ANALYZE RES_POOL, ENDOWMENT CORRELATIONS

################################################################################

# using champion genotype data

################################################################################

ax = sns.regplot(champions_df['res_pool1'], champions_df['endowment1'])
lab = "Zeroth-level Resource Sharing and Zeroth-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Dominant Genotype $P_0$")
ax.set_ylabel("Dominant Genotype $E_0")

plt.savefig("champion_res_pool1_vs_champion_endowment1.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for champion %s:" % lab)
corboot(champions_df[['res_pool1','endowment1']])

################################################################################

ax = sns.regplot(champions_df['res_pool2'], champions_df['endowment1'])
lab = "First-level Resource Sharing and Zeroth-level Offspring Endowment"
ax.set_title(lab)
ax.set_ylabel("Dominant Genotype $P_1$")
ax.set_xlabel("Dominant Genotype $E_0$")

plt.savefig("champion_res_pool2_vs_champion_endowment1.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for champion %s:" % lab)
corboot(champions_df[['res_pool2','endowment1']])

################################################################################

ax = sns.regplot(champions_df['res_pool1'], champions_df['endowment2'])
lab = "Zeroth-level Resource Sharing and First-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Dominant Genotype $P_0$")
ax.set_ylabel("Dominant Genotype $E_1$")

plt.savefig("champion_res_pool1_vs_champion_endowment2.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for champion %s:" % lab)
corboot(champions_df[['res_pool1','endowment2']])

################################################################################

ax = sns.regplot(champions_df['res_pool2'], champions_df['endowment2'])
lab = "First-level Resource Sharing and First-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Dominant Genotype $P_1$")
ax.set_ylabel("Dominant Genotype $E_1$")

plt.savefig("champion_res_pool2_vs_champion_endowment2.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for champion %s:" % lab)
corboot(champions_df[['res_pool2','endowment2']])

################################################################################

# using mean genotype data

################################################################################

ax = sns.regplot(means_df['mean_res_pool1'], means_df['mean_endowment1'])
lab = "Zeroth-level Resource Sharing and Zeroth-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Population Mean $E_0$")

plt.savefig("mean_res_pool1_vs_mean_endowment1.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(means_df[['mean_res_pool1','mean_endowment1']])

################################################################################

ax = sns.regplot(means_df['mean_res_pool2'], means_df['mean_endowment1'])
lab = "First-level Resource Sharing and Zeroth-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Population Mean $E_0$")

plt.savefig("mean_res_pool2_vs_mean_endowment1.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(means_df[['mean_res_pool2','mean_endowment1']])

################################################################################

ax = sns.regplot(means_df['mean_res_pool1'], means_df['mean_endowment2'])
lab = "Zeroth-level Resource Sharing and First-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Population Mean $E_1$")

plt.savefig("mean_res_pool1_vs_mean_endowment2.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(means_df[['mean_res_pool1','mean_endowment2']])

################################################################################

ax = sns.regplot(means_df['mean_res_pool2'], means_df['mean_endowment2'])
lab = "First-level Resource Sharing and First-level Offspring Endowment"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Population Mean $E_1$")

plt.savefig("mean_res_pool2_vs_mean_endowment2.pdf", transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(means_df[['mean_res_pool2','mean_endowment2']])
