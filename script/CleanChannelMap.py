import pandas as pd
from tqdm import tqdm
import numpy as np
import sys
from itertools import count
from collections import defaultdict

assert len(sys.argv) == 4, \
    "First argument is filename, second argument is start update, third argument is finish update."

iter = pd.read_csv(sys.argv[1], chunksize=14400)
last = None
for chunk in iter:
    last = chunk

    if any(chunk['update'] >= int(sys.argv[2])):
        break

# setup unique channel ID assignment system
it0 = count()
next(it0)
maps0 = defaultdict(lambda: next(it0))
maps_prev0 = {}

it1 = count()
next(it1)
maps1 = defaultdict(lambda: next(it1))
maps_prev1 = {}

def processChunk(chunk, header):
    global it0, maps0, maps_prev0, it1, maps1, maps_prev1

    # filter out non-uniformly spaced updates
    if chunk.iloc[0]['update'] % 5552 != 0:
        return

    # filter out non-uniformly spaced updates
    if chunk.iloc[0]['update'] > int(sys.argv[2]):
        return

    # add for convenience
    chunk['count'] = 1

    # filter out dead cells
    chunk = chunk[chunk['channel0'] != 0]

    # set unique channel values
    for __, row in chunk.iterrows():
        if row['channel1'] in maps1:
            pass
        elif row['channel1'] in maps_prev1:
            maps1[row['channel1']] = maps_prev1[row['channel1']]

        row['channel1'] = maps1[row['channel1']]

        if row['channel0'] in maps0:
            pass
        elif row['channel0'] in maps_prev0:
            maps0[row['channel0']] = maps_prev0[row['channel0']]

        row['channel0'] = maps0[row['channel0']]

    # update maps
    maps_prev0 = maps0
    maps0 = defaultdict(lambda: next(it0))
    maps_prev1 = maps1
    maps1 = defaultdict(lambda: next(it1))

    chunk.to_csv(sys.argv[1]  + "_cl", header=header, mode='a', index=False)


processChunk(last,True)
for chunk in tqdm(iter):
    processChunk(chunk,False)

print(sys.argv[1] + " done!")
