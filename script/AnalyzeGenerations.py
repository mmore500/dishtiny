import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
import scipy.stats as stats
import numpy as np

# LOAD MEAN GENOTYPE, PHENOTYPE, POPULATION DATA
# and filter to most recent update in each dataframe

dfp = pd.concat([pd.read_csv(f) for f in glob.glob("Phenotypes_*.csv")], ignore_index=True)

fil_dfp = dfp[dfp['update'] == dfp['update'].max()].reset_index(drop=True)

dfg = pd.concat([pd.read_csv(f) for f in glob.glob("../collected/Genotypes_*.csv")], ignore_index=True)

fil_dfg = dfg[dfg['update'] == dfg['update'].max()].reset_index(drop=True)

dfo = pd.concat([pd.read_csv(f) for f in glob.glob("Population_*.csv")], ignore_index=True)

fil_dfo = dfo[dfo['update'] == dfo['update'].max()].reset_index(drop=True)

# stitch together dataframes using 'seed'

dfc = fil_dfp.join(
        fil_dfg.set_index('seed').drop(columns=['update']), on='seed'
        ).reset_index(drop=True
        ).join(fil_dfo.set_index('seed').drop(columns=['update']), on='seed'
        ).reset_index(drop=True)


# CREATE NEW ROWS WITH cell, chan0, chan1 GENERATION LENGTH CALCULATED
pstep = np.sort(dfp['update'].unique())[1] - np.sort(dfp['update'].unique())[0]

dfc['cell_generation'] = dfc.apply(
        lambda row: row['num_orgs']*pstep/row['total_reproduce'],
        axis=1
        )

dfc['chan0_generation'] = dfc.apply(
        lambda row: row['num_channels0']*pstep/row['total_channelrep0'],
        axis=1
        )

dfc['chan1_generation'] = dfc.apply(
        lambda row: row['num_channels1']*pstep/row['total_channelrep1'],
        axis=1
        )

###############################################################################

# INVESTIGATE GENERATION-GENOTYPE CORRELATIONS

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['cell_generation'])
lab = "First-level Resource Sharing and Cell Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Cell Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_cell_generation.pdf")
plt.clf()

print("Pearson Correlation, p value for mean %s:" % lab)
print(stats.pearsonr(
        dfc['mean_res_pool1'],
        dfc['cell_generation']
    ))

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['chan0_generation'])
lab = "First-level Resource Sharing and Channel 0 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Channel 0 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_chan0_generation.pdf")
plt.clf()

print("Pearson Correlation, p value for mean %s:" % lab)
print(stats.pearsonr(
        dfc['mean_res_pool1'],
        dfc['chan0_generation']
    ))

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['chan1_generation'])
lab = "First-level Resource Sharing and Channel 1 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Channel 1 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_chan1_generation.pdf")
plt.clf()

print("Pearson Correlation, p value for mean %s:" % lab)
print(stats.pearsonr(
        dfc['mean_res_pool1'],
        dfc['chan1_generation']
    ))


###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['cell_generation'])
lab = "Second-level Resource Sharing and Cell Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_2$")
ax.set_ylabel("Updates Per Cell Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_cell_generation.pdf")
plt.clf()

print("Pearson Correlation, p value for mean %s:" % lab)
print(stats.pearsonr(
        dfc['mean_res_pool2'],
        dfc['cell_generation']
    ))

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['chan0_generation'])
lab = "Second-level Resource Sharing and Channel 0 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_2$")
ax.set_ylabel("Updates Per Channel 0 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_chan0_generation.pdf")
plt.clf()

print("Pearson Correlation, p value for mean %s:" % lab)
print(stats.pearsonr(
        dfc['mean_res_pool2'],
        dfc['chan0_generation']
    ))

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['chan1_generation'])
lab = "Second-level Resource Sharing and Channel 1 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_2$")
ax.set_ylabel("Updates Per Channel 1 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_chan1_generation.pdf")
plt.clf()

print("Pearson Correlation, p value for mean %s:" % lab)
print(stats.pearsonr(
        dfc['mean_res_pool2'],
        dfc['chan1_generation']
    ))

###############################################################################

# CALCULATE MEAN GENEARTION TIME FOR DIFFERENT OUTCOMES

###############################################################################

print("Mean Cell Generation Length for Zero-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['cell_generation']
    ))

print("Mean Channel 0 Generation Length for Zero-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['chan0_generation']
    ))

print("Mean Channel 1 Generation Length for Zero-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['chan1_generation']
    ))

###############################################################################

print("Mean Cell Generation Length for First-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['cell_generation']
    ))

print("Mean Channel 0 Generation Length for First-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['chan0_generation']
    ))

print("Mean Channel 1 Generation Length for First-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['chan1_generation']
    ))

###############################################################################

print("Mean Cell Generation Length for Second-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['cell_generation']
    ))

print("Mean Channel 0 Generation Length for Second-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['chan0_generation']
    ))

print("Mean Channel 1 Generation Length for Second-Level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['chan1_generation']
    ))

###############################################################################
