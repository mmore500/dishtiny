#!/usr/bin/env python3
"""Generate num_interpolation_steps genome variants, progressively nopping more_ops_genome until it matches less_ops_genome.

The instructions nopped are independently drawn each interpolation step.
Output genomes are written to the current working directory with programatically-generated filenames.

Usage:
    ./make_nop_interpolated_variants.py [less_ops_genome] [more_ops_genome] [num_interpolation_steps]
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
    __, less_ops, more_ops, num_interpolation_steps = sys.argv
    num_interpolation_steps = int( num_interpolation_steps )
except:
    print( __doc__ )
    sys.exit(1)

assert 'ext' in kn.unpack( less_ops )
assert 'ext' in kn.unpack( more_ops )

less_ops_data = genome_local_autoload( less_ops )
more_ops_data = genome_local_autoload( more_ops )

less_ops_num_insts = len( less_ops_data['value0']['program'] )
more_ops_num_insts = len( more_ops_data['value0']['program'] )

assert less_ops_num_insts == more_ops_num_insts

less_ops_num_ops = sum(
    'Nop-' not in inst['operation']
    for inst in less_ops_data["value0"]["program"]
)
more_ops_num_ops = sum(
    'Nop-' not in inst['operation']
    for inst in more_ops_data["value0"]["program"]
)

assert less_ops_num_ops <= more_ops_num_ops

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

sampled_nop_counts = [
    round(count)
    for count in np.linspace(
        0, len(target_op_idxs), num_interpolation_steps
    )
]

for interpolation_idx, num_to_nop in enumerate( sampled_nop_counts ):

    print(f'interpolation step {interpolation_idx}')
    print(f'  nopping {num_to_nop} ops on {more_ops}...')

    variant = copy.deepcopy( more_ops_data )

    for target_op_idx in random.sample( target_op_idxs, num_to_nop ):
        variant['value0']['program'][ target_op_idx ]['operation'] = 'Nop-0'

    attrs = kn.unpack( more_ops )
    attrs['nop_interpolation_step'] = interpolation_idx
    attrs['nop_interpolation_num_nopped'] = num_to_nop
    attrs['variation'] = (
        f'{attrs["variation"]}~{num_to_nop}x%Nop-0'
        if 'variation' in attrs
        and attrs['variation'] != 'master' else
        f'{num_to_nop}x%Nop-0'
    )

    with (
            open(kn.pack( attrs ), 'w',  encoding='ascii')
            if attrs['ext'] == '.json' else
            gzip.open(kn.pack( attrs ), 'wt', encoding='ascii')
        ) as f:

        json.dump(variant, f)
