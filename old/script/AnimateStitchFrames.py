import sys
import os
import shutil
from keyname import keyname as kn
from collections import defaultdict
import pandas as pd
from tqdm import tqdm
from joblib import delayed, Parallel

filenames = sys.argv[1:]

assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1
assert len({kn.unpack(filename)['seed'] for filename in filenames}) == 1

df = pd.DataFrame.from_dict([kn.unpack(filename) for filename in filenames])

updates = [int(upd) for upd in df['update'].unique()]

for treat in df['treat'].unique():

    commands = [
        ' '.join(
            ['convert', '+append']
            + fil['_'].tolist()
            + [kn.pack({
                'treat' : treat,
                'frame' : str(idx).zfill(4),
                'seed' : kn.unpack(filenames[0])['seed'],
                '_source_hash' : kn.unpack(filenames[0])['_source_hash'],
                'ext' : '.png'
                })]
        ) for idx, update in enumerate(sorted(updates))
        for fil in (df[(df['treat'] == treat) & (df['update'] == str(update))],)
        if len(fil['title'].unique()) == len(df['title'].unique())
    ]

    Parallel(n_jobs=-1)(
        delayed(os.system)(command) for command in tqdm(commands)
    )

    os.system(' '.join(
        ['ffmpeg','-framerate','10','-i']
        + [kn.pack({
            'treat' : treat,
            'frame' : '%04d',
            'seed' : kn.unpack(filenames[0])['seed'],
            '_source_hash' : kn.unpack(filenames[0])['_source_hash'],
            'ext' : '.png'
        })] + ['-c:v','libx264','-r','30','-pix_fmt','yuv420p']
        + [kn.pack({
            'treat' : treat,
            'title' : '_'.join(df['title'].unique()),
            'seed' : kn.unpack(filenames[0])['seed'],
            '_source_hash' : kn.unpack(filenames[0])['_source_hash'],
            'ext' : '.mp4'
        })]
    ))
