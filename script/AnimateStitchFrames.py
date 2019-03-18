import sys
import os
import shutil
from keyname import keyname as kn
from collections import defaultdict
import pandas as pd
from tqdm import tqdm

filenames = sys.argv[1:]

df = pd.DataFrame.from_dict([kn.unpack(filename) for filename in filenames])

for treat in df['treat'].unique():
    updates = [int(upd) for upd in df['update'].unique()]
    for idx, update in tqdm(enumerate(sorted(updates))):
        fil = df[(df['treat'] == treat) & (df['update'] == str(update))]
        if len(fil['title'].unique()) == len(df['title'].unique()):
            os.system(' '.join(
                ['convert', '+append']
                + fil['_'].tolist()
                + [kn.pack({
                    'treat' : treat,
                    'frame' : str(idx).zfill(4),
                    'ext' : '.png'
                    })]
                ))

    os.system(' '.join(
        ['ffmpeg','-framerate','10','-i']
        + [kn.pack({'treat' : treat, 'frame' : '%04d', 'ext' : '.png'})]
        + ['-c:v','libx264','-r','30','-pix_fmt','yuv420p']
        + [kn.pack({'treat' : treat, 'ext' : '.mp4'})]
        ))
