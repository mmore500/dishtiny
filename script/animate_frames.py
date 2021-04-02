R"python3(" # this allows us to #include the script in C++

import sys
import os
import shutil
from keyname import keyname as kn
from collections import defaultdict
import pandas as pd
from tqdm import tqdm
from joblib import delayed, Parallel
import cv2
from math import gcd, isnan
from functools import reduce
import itertools

# adapted from https://stackoverflow.com/a/50830937
def find_gcd(list_):
    x = reduce(gcd, list_)
    return x

# adapted from https://stackoverflow.com/a/43091576
def to_ranges(integers):
    integers = sorted(set(integers))
    by = find_gcd( integers ) or 1
    for key, group in itertools.groupby(
        enumerate(integers),
        lambda t: (t[1] / by - t[0]),
    ):
        group = list(group)
        yield group[0][1], group[-1][1], by

def summarize_ranges(integers):
    return '_'.join(
        f'{from_}-{to}{f"%{by}" if by != 1 else ""}'
        if from_ != to else
        str(from_)
        for from_, to, by in to_ranges(integers)
    )

def is_numeric(val):
    return isinstance(val, int) or (isinstance(val, float) and not isnan(val)) or (isinstance(val, str) and val.isdigit())

def summarize(values):
    vals=list(values)
    if all(is_numeric(value) for value in vals):
        return summarize_ranges(map(int, vals))
    elif len(set(vals)) < 3:
        return '~'.join( str(x) for x in sorted(set(vals)) )
    else:
        return f'num_unique%{len(set(vals))}'

os.chdir('outdrawings/')
filenames = [f for f in os.listdir('.') if os.path.isfile(f) and f.endswith('.png')]

try:
    fps = int(sys.argv[1])
except:
    fps = 15

df = pd.DataFrame.from_dict([kn.unpack(filename) for filename in filenames])

df['update'] = pd.to_numeric(df['update'], errors='coerce')
df.sort_values(by=['update'], inplace=True)

updates = [int(upd) for upd in df['update'].unique()]

for treat in df['a'].unique():
    images = df[df['a'] == treat]

    frames = []

    start_frame = cv2.imread(images.iloc[0].iloc[8])
    height, width, layers = start_frame.shape

    for file in images['_']:
        frame = cv2.imread(file)
        height, width, layers = frame.shape
        size = (width,height)
        frames.append(frame)

    attrs = {'ext': '.mp4'}

    for column in images.columns:
        if column not in {'_', 'ext'}:
            attrs[column] = summarize(df[column])

    filename = kn.pack(attrs)

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video = cv2.VideoWriter(filename, fourcc, fps, (width, height))

    for frame in frames:
        video.write(frame)

    cv2.destroyAllWindows()
    video.release()

# this allows us to #include the script in C++
")python3"
