import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
import scipy.stats as stats

from corboot import corboot

# open-type fonts
matplotlib.rcParams['pdf.fonttype'] = 42

# LOAD MEAN GENOTYPE, PHENOTYPE DATA
# and filter to most recent update in each dataframe

dfp = pd.concat([pd.read_csv(f) for f in glob.glob("Phenotypes_*.csv")], ignore_index=True)

fil_dfp = dfp[dfp['update'] == dfp['update'].max()].reset_index(drop=True)

dfg = pd.concat([pd.read_csv(f) for f in glob.glob("../collected/Genotypes_*.csv")], ignore_index=True)

fil_dfg = dfg[dfg['update'] == dfg['update'].max()].reset_index(drop=True)

# stitch together dataframes using 'seed'

dfc = fil_dfp.join(fil_dfg.set_index('seed').drop(columns=['update']), on='seed').reset_index(drop=True)

###############################################################################

ax = sns.regplot(dfc['mean_res_pool1'],dfc['total_reproduce'])
lab = "Zeroth-level Resource Sharing and Net Reproduction Rate"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_0$")
ax.set_ylabel("Population Net Offspring per 10,000 Updates")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool1_vs_net_reproduction.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool1','total_reproduce']])

################################################################################

ax = sns.regplot(dfc['mean_res_pool2'],dfc['total_reproduce'])
lab = "First-level Resource Sharing and Net Reproduction Rate"
ax.set_title(lab)
ax.set_xlabel("Population Mean $P_1$")
ax.set_ylabel("Population Net Offspring per 10,000 Updates")

plt.gcf().set_size_inches(7, 5)
plt.savefig("mean_res_pool2_vs_net_reproduction.pdf",transparent=True)
plt.clf()

print("Correlation Bootstrap for mean %s:" % lab)
corboot(dfc[['mean_res_pool2','total_reproduce']])
