import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
import scipy.stats as stats
import numpy as np

from corboot import corboot

# LOAD MEAN GENOTYPE, PHENOTYPE, POPULATION DATA
# and filter to most recent update in each dataframe

dfp = pd.concat([pd.read_csv(f) for f in glob.glob("Phenotypes_*.csv")], ignore_index=True)

fil_dfp = dfp[dfp['update'] == dfp['update'].max()].reset_index(drop=True)

dfg = pd.concat([pd.read_csv(f) for f in glob.glob("Genotypes_*.csv")], ignore_index=True)

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
# how many updates between dfp recordings?
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

dfc['num_gens'] = dfc['total_reproduce'] / dfc['num_orgs']

## PRINT DIAGNOSTIC INFORMATION
print("Mean number cell generations per %d updates at update %d:"
        % (pstep, dfp['update'].max())
    )
print(dfc['num_gens'].mean())
print("Standard deviation number cell generations per %d updates at update %d:"
        % (pstep, dfp['update'].max())
    )
print(dfc['num_gens'].std())

print("Mean number cell generation length at update %d:"
        % dfp['update'].max()
    )
print(dfc['cell_generation'].mean())
print("Standard deviation cell generation length at update %d:"
        % dfp['update'].max()
    )
print(dfc['cell_generation'].std())


###############################################################################

# INVESTIGATE GENERATION-GENOTYPE CORRELATIONS

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['cell_generation'])
lab = "Zeroth-level Resource Sharing and Cell Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Updates Per Cell Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_cell_generation.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','cell_generation']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['chan0_generation'])
lab = "Zeroth-level Resource Sharing and Channel 0 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Updates Per Channel 0 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_chan0_generation.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','chan0_generation']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['chan1_generation'])
lab = "Zeroth-level Resource Sharing and Channel 1 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Updates Per Channel 1 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_chan1_generation.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','chan1_generation']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['cell_generation'])
lab = "First-level Resource Sharing and Cell Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Cell Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_cell_generation.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','cell_generation']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['chan0_generation'])
lab = "First-level Resource Sharing and Channel 0 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Channel 0 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_chan0_generation.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','chan0_generation']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['chan1_generation'])
lab = "First-level Resource Sharing and Channel 1 Generation Length"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Channel 1 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_chan1_generation.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','chan1_generation']])

###############################################################################

# CALCULATE MEAN GENERATION TIME FOR DIFFERENT OUTCOMES

###############################################################################

print("Mean Cell Generation Length for Cell-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['cell_generation']
    ))

print("Mean Channel 0 Generation Length for Cell-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['chan0_generation']
    ))

print("Mean Channel 1 Generation Length for Cell-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['chan1_generation']
    ))

###############################################################################

print("Mean Cell Generation Length for Zeroth-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['cell_generation']
    ))

print("Mean Channel 0 Generation Length for Zeroth-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['chan0_generation']
    ))

print("Mean Channel 1 Generation Length for Zeroth-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['chan1_generation']
    ))

###############################################################################

print("Mean Cell Generation Length for First-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['cell_generation']
    ))

print("Mean Channel 0 Generation Length for First-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['chan0_generation']
    ))

print("Mean Channel 1 Generation Length for First-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['chan1_generation']
    ))

###############################################################################

# INVESTIGATE POPULATION-GENOTYPE CORRELATIONS

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['num_orgs'])
lab = "Zeroth-level Resource Sharing and Cell Population Size"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Updates Per Cell Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_num_orgs.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','num_orgs']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['num_channels0'])
lab = "Zeroth-level Resource Sharing and Channel 0 Population Size"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Updates Per Channel 0 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_num_channels0.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','num_channels0']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['num_channels1'])
lab = "Zeroth-level Resource Sharing and Channel 1 Population Size"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Updates Per Channel 1 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_num_channels1.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','num_channels1']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['num_orgs'])
lab = "First-level Resource Sharing and Cell Population Size"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Cell Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_num_orgs.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','num_orgs']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['num_channels0'])
lab = "First-level Resource Sharing and Channel 0 Population Size"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Channel 0 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_num_channels0.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','num_channels0']])

###############################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['num_channels1'])
lab = "First-level Resource Sharing and Channel 1 Population Size"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Updates Per Channel 1 Generation")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_num_channels1.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','num_channels1']])

###############################################################################

# CALCULATE MEAN POPULATION SIZE FOR DIFFERENT OUTCOMES

###############################################################################

print("Mean Cell Population Size for Cell-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['num_orgs']
    ))

print("Mean Channel 0 Population Size for Cell-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['num_channels0']
    ))

print("Mean Channel 1 Population Size for Cell-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool0'] > dfc['mean_res_pool1']) &
                (dfc['mean_res_pool0'] > dfc['mean_res_pool2'])
            ]['num_channels1']
    ))

###############################################################################

print("Mean Cell Population Size for Zeroth-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['num_orgs']
    ))

print("Mean Channel 0 Population Size for Zeroth-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['num_channels0']
    ))

print("Mean Channel 1 Population Size for Zeroth-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool1'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool1'] > dfc['mean_res_pool2'])
            ]['num_channels1']
    ))

###############################################################################

print("Mean Cell Population Size for First-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['num_orgs']
    ))

print("Mean Channel 0 Population Size for First-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['num_channels0']
    ))

print("Mean Channel 1 Population Size for First-level Outcomes:")
print(np.mean(
        dfc[
                (dfc['mean_res_pool2'] > dfc['mean_res_pool0']) &
                (dfc['mean_res_pool2'] > dfc['mean_res_pool1'])
            ]['num_channels1']
    ))

###############################################################################
