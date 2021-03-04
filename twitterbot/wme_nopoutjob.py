#!/usr/bin/env python3

import glob
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
    glob.glob('outdata/a=genome_statistics*morph=phenotype_equivalent_nopout*variation=master*.csv')[0]
)
print(metrics)

def query(metric):
    return metrics[ metric ].iloc[0]

with tempfile.TemporaryDirectory() as dirpath:

    genome_path = glob.glob('outartifacts/a=genome*morph=phenotype_equivalent_nopout*variation=master*.json.gz')[0]
    os.system( f'cp "{genome_path}" "{dirpath}/out.json.gz"' )
    os.system( f'gunzip -c "{dirpath}/out.json.gz" > "{dirpath}/out.txt"' )

    image_path = dirpath + '/out.png'
    os.system( f'convert -size 1000x2000 xc:white -font "Liberation-Mono" -pointsize 12 -fill black -annotate +15+15 "@{dirpath}/out.txt" -trim -bordercolor "#FFF" -border 10 +repage {image_path}' )
    os.system(f'convert "{image_path}" -define png:extent=3000KB "{image_path}"')


    status = f'''{wme.make_catchphrase('geno')}

    strain: {wme.make_name(series)}
    cell gens: {query("Elapsed Generations Level 0")}
    indel muts: {query("Elapsed Indel Mutations")}
    point muts: {query("Elapsed Point Mutations")}
    num modules: {query("Num Modules")}
    num sites: {query("Num Instructions")}
    num sites affecting phenotype: {query("Num Op Instructions")}
    '''

    print("tweeting...")
    print("length", len(status))
    print(status)

    tweepy_handle = wme.make_tweepy_handle()
    tweet = tweepy_handle.update_with_media(image_path, status[:280])
    tweepy_handle.update_status(
        status=wme.make_attribution(),
        in_reply_to_status_id=tweet.id,
        auto_populate_reply_metadata=True,
    )
