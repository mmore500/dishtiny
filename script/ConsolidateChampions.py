import pandas as pd
import glob
import seaborn as sns
from matplotlib import pyplot as plt

res = []
for f in glob.glob("Census_*.csv"):
    dfin = pd.read_csv(f,comment='#')
    fil_upd = dfin[dfin['update'] == dfin['update'].max()].reset_index(drop=True)
    fil_count = fil_upd.iloc[[fil_upd['count'].idxmax()]]
    print(fil_count)
    res.append(fil_count)

champs = pd.concat(res, ignore_index=True)

print(champs)

champs.to_csv("ConsolidatedChampions.csv", index=False)
