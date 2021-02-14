#!/usr/bin/env python3

import copy
import gzip
import json
import sys

from keyname import keyname as kn

__, target = sys.argv

assert 'ext' in kn.unpack( target )

def multiloader(target):
    if kn.unpack( target )['ext'] == '.json':
        with open( target, 'r') as f:
            return json.load( f )

    elif kn.unpack( target )['ext'] == '.json.gz':
        try:
            with gzip.open( target, 'rb') as f:
                return json.loads( f.read().decode('ascii') )
        except Exception:
            pass

        try:
            with gzip.open( target, 'rb') as f:
                return json.loads( f.read().decode('utf-8') )
        except Exception:
            pass

    raise ValueError


data = multiloader( target )

ops = [
    idx
    for idx, inst in enumerate(data['value0']['program'])
    if 'Nop-' not in inst['operation']
]

print(f'{target} has {len(data["value0"]["program"])} instructions')
print(f'nopping out {len(ops)} op sites...')

for idx in ops:

    variant = copy.deepcopy(data)

    variant['value0']['program'][ idx ]['operation'] = 'Nop-0'

    attrs = kn.unpack(target)
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
