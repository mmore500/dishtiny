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
from iterdub import iterdub as ib

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
            attrs[column] = ib.dub(df[column])

    filename = kn.pack(attrs)

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video = cv2.VideoWriter(filename, fourcc, fps, (width, height))

    for frame in frames:
        video.write(frame)

    cv2.destroyAllWindows()
    video.release()

# this allows us to #include the script in C++
")python3"
