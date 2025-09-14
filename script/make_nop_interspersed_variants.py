#!/usr/bin/env python3
"""Generate genome variants of less_ops_genome, one containing each extra nop in more_ops_genome.

The instructions nopped are independently drawn each interspersion step.
Output genomes are written to the current working directory with programatically-generated filenames.

Usage:
    ./make_nop_interspersed_variants.py [less_ops_genome] [more_ops_genome]
"""

import copy
import gzip
import itertools as it
import json
from keyname import keyname as kn
import numpy as np
import random
import sys

from dishpylib.pyloaders import genome_local_autoload

random.seed(1)

try:
    __, less_ops, more_ops = sys.argv
except:
    print( __doc__ )
    sys.exit(1)

assert 'ext' in kn.unpack( less_ops )
assert 'ext' in kn.unpack( more_ops )

less_ops_data = genome_local_autoload( less_ops )
more_ops_data = genome_local_autoload( more_ops )

less_ops_num_insts = len( less_ops_data['value0']['program'] )
more_ops_num_insts = len( more_ops_data['value0']['program'] )

assert less_ops_num_insts == more_ops_num_insts, (
    less_ops_num_insts,
    more_ops_num_insts,
)

less_ops_num_ops = sum(
    'Nop-' not in inst['operation']
    for inst in less_ops_data["value0"]["program"]
)
more_ops_num_ops = sum(
    'Nop-' not in inst['operation']
    for inst in more_ops_data["value0"]["program"]
)

assert less_ops_num_ops <= more_ops_num_ops, (
    less_ops_num_ops,
    more_ops_num_ops,
)

target_op_idxs = [
    idx
    for idx, less_ops_inst, more_ops_inst in zip(
        it.count(),
        less_ops_data['value0']['program'],
        more_ops_data['value0']['program'],
    )
    if 'Nop-' not in more_ops_inst['operation']
    and 'Nop-' in less_ops_inst['operation']
]

print(
    f'{less_ops} has {less_ops_num_insts} instructions, '
    f'{less_ops_num_ops} of which are ops'
)
print(
    f'{more_ops} has {more_ops_num_insts} instructions, '
    f'{more_ops_num_ops} of which are ops'
)
print( f'{len(target_op_idxs)} target op indexes' )


for interspersion_idx, target_op_idx in enumerate( target_op_idxs ):

    print(f'interspersion step {interspersion_idx}')
    print(f'  nopping op {target_op_idx} on {more_ops}...')

    variant = copy.deepcopy( more_ops_data )

    variant['value0']['program'][ target_op_idx ]['operation'] = 'Nop-0'

    attrs = kn.unpack( more_ops )
    attrs['nop_interspersion_step'] = interspersion_idx
    attrs['nop_interspersion_target'] = target_op_idx
    attrs['variation'] = (
        f'{attrs["variation"]}~i{target_op_idx}%Nop-0'
        if 'variation' in attrs
        and attrs['variation'] != 'master' else
        f'i{target_op_idx}%Nop-0'
    )

    with (
            open(kn.pack( attrs ), 'w',  encoding='ascii')
            if attrs['ext'] == '.json' else
            gzip.open(kn.pack( attrs ), 'wt', encoding='ascii')
        ) as f:

        json.dump(variant, f)
