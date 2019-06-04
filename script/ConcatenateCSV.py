# usage:
# filenames

import sys
import os
import seaborn as sns
import pandas as pd
from tqdm import tqdm

filenames = sys.argv[1:]

res = pd.concat(pd.read_csv(filename) for filename in tqdm(filenames))

res.to_csv('out.csv', index=False)
