import pandas as pd
import sys
from tqdm import tqdm
import os

assert len(sys.argv) >= 3, \
    "First argument is maximum update (inclusive), last arguments are filenames."


for f in sys.argv[2:]:

    df = pd.read_csv(f,comment='#')

    if df['update'].max() < int(sys.argv[1]):
        os.remove(f)
        continue

    open(f + "_tr", 'w').close()

    print("start " + f)

    last = None
    iter = pd.read_csv(f, chunksize=14400)
    header = True
    for chunk in tqdm(iter):
        last = chunk

        if any(chunk['update'] > int(sys.argv[1])):
            break

        chunk.to_csv(f  + "_tr", header=header, mode='a', index=False)
        header = False

    last = last[last['update'] <= int(sys.argv[1])]
    last.to_csv(f + "_tr", header=header, mode='a', index=False)

    print("done " + f)
