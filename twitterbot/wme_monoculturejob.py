#!/usr/bin/env python3

import glob
import image_slicer
import os
import pandas as pd
import sys
import tempfile
import tweepy

import watch_me_evolve as wme

series, stint = map(int, sys.argv[1:])

if wme.make_id(series) not in (0, 1, 2):
    sys.exit(0)

metrics = pd.read_csv(
    glob.glob('outdata/a=demographic_phenotypic_phylogenetic_metrics*.csv')[0]
)

def query(metric):
    return metrics[ metrics['Metric'] == metric ]['Value'].iloc[0]


status = f'''{wme.make_catchphrase('pheno')}

strain: {wme.make_name(series)}
cell gen: {query("Mean Elapsed Generations Level 0"):.2E}
elapsed indel muts: {query("Mean Elapsed Indel Mutations"):.2E}
elapsed point muts: {query("Mean Elapsed Point Mutations"):.2E}
inner group size: {query("Mean Kin Group Size Level 0"):.2E}
outer group size: {query("Mean Kin Group Size Level 1"):.2E}
reproduction rate: {query("Num Births per Cell-update"):.2E}
'''

print("tweeting...")
print("length", len(status))
print(status)
print(wme.make_attribution())


with tempfile.TemporaryDirectory() as dirpath:

    original_image = glob.glob('outdrawings/a=montage*.jpg')[0]
    tiles = image_slicer.slice(original_image, 4, save=False)
    image_slicer.save_tiles(tiles, directory=dirpath)
    os.system(
        f'for f in {dirpath}/*; do convert "$f" -define jpeg:extent=3000KB "$f"; done'
    )
    os.system(f'du -h {dirpath}/*')


    tweepy_handle = wme.make_tweepy_handle()

    media_ids = []
    for filename in glob.glob(f'{dirpath}/*'):
        res = tweepy_handle.media_upload(filename)
        media_ids.append(res.media_id)

    print('made tweepy handle')
    tweet = tweepy_handle.update_status(
        status=status[:280],
        media_ids=media_ids,
        )
    print('sent main status')
    tweepy_handle.update_status(
        status=wme.make_attribution(),
        in_reply_to_status_id=tweet.id,
        auto_populate_reply_metadata=True,
    )
    print('added attribution')

print('phenotype tweet send success!')
