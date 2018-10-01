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
        [pd.read_csv(f,comment='#') for f in glob.glob("Genotypes_*.csv")],
        ignore_index=True
    )

# filter for comparison update
df = df_in[df_in['update'] == 200000].reset_index(drop=True)

def r(f):
    return "{0:.2f}".format(round(f,2))

print()

print('means', len(df))
for column in df:
    print(column,r(df[column].mean()),r(df[column].std()))
