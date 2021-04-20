#!/usr/bin/env python3

import copy
import gzip
import json
import sys

from keyname import keyname as kn

from dishpylib.pyloaders import genome_local_autoload

try:
    __, applyto, basedon = sys.argv
except:
    print('bad arguments')
    print('USAGE: [applyto] [basedon]')
    sys.exit(1)

assert 'ext' in kn.unpack( applyto )
assert 'ext' in kn.unpack( basedon )

applytodata = genome_local_autoload( applyto )
basedondata = genome_local_autoload( basedon )

assert (
    len( applytodata['value0']['program'] )
    == len( basedondata['value0']['program'] )
)


ops = [
    idx
    for idx, inst in enumerate(basedondata['value0']['program'])
    if 'Nop-' not in inst['operation']
]

print(f'{basedon} has {len(basedondata["value0"]["program"])} instructions, {len(ops)} of which are ops')
print(f'nopping out corresponding {len(ops)} sites on {applyto}...')

for idx in ops:

    variant = copy.deepcopy(applytodata)

    variant['value0']['program'][ idx ]['operation'] = 'Nop-0'

    attrs = kn.unpack(applyto)
    attrs['variation'] = (
        f'{attrs["variation"]}~i{idx}%Nop-0'
        if 'variation' in attrs
        and attrs['variation'] != 'master' else
        f'i{idx}%Nop-0'
    )

    with (
            open(kn.pack( attrs ), 'w',  encoding='ascii')
            if attrs['ext'] == '.json' else
            gzip.open(kn.pack( attrs ), 'wt', encoding='ascii')
        ) as f:

        json.dump(variant, f)
