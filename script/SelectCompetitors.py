import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt
from tqdm import tqdm

# LOAD MEAN GENOTYPE DATA

res = pd.concat(
        [pd.read_csv(f,comment='#') for f in glob.glob("Genotypes_*.csv")],
        ignore_index=True
    )


# FILTER TO LAST UPDATE

fil_upd = res[res['update'] == res['update'].max()].reset_index(drop=True)


# PRINT SEED VALUE WITH GREATEST MEAN USE OF EACH POOL

print('largest mean_res_pool0')
print(fil_upd.nlargest(1, 'mean_res_pool0')['seed'])

print('largest mean_res_pool1')
print(fil_upd.nlargest(1, 'mean_res_pool1')['seed'])

print('largest mean_res_pool2')
print(fil_upd.nlargest(1, 'mean_res_pool2')['seed'])
