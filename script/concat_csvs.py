R"python3(" # this allows us to #include the script in C++

__doc__ = """Combines data from several csv files.

All input files must be from the same directory.
Output data is written to a csv in the directory input files were read from.
The output filename is programatically generated.

Usage:
    ./concat_csvs.py [target_files]...
"""

from iterdub import iterdub as ib
from iterpop import iterpop as ip
from keyname import keyname as kn
import os
import pandas as pd
import pathlib
import sys

def make_output_filename():

    df = pd.DataFrame.from_records(
        [
            kn.unpack(source_filename)
            for source_filename in sys.argv[1:]
        ],
    )

    out_attrs = {
        column : ib.dub( df[column] )
        for column in df.columns
        if column not in { '_', 'ext', }
    }

    out_filename = kn.pack({
        **out_attrs,
        **{
            'concat' : str( len( sys.argv ) - 1 ),
            'ext' : '.csv',
        },
    })

    return out_filename

def make_output_dir():

    # directory all sources were located in
    # must be identical across sources
    source_dir = ip.pophomogeneous(
        map(
            lambda x: os.path.dirname( os.path.abspath( x ) ),
            sys.argv[1:],
        )
    )

    # put output files into concat subdirectory
    return  os.path.join( source_dir, 'concat', )

def make_output_path():

    return os.path.join(
        make_output_dir(),
        make_output_filename(),
    )

if len( sys.argv ) > 1:

    # ensure output directory exists
    pathlib.Path(
        make_output_dir(),
    ).mkdir(
        parents=True, exist_ok=True,
    )

    df = pd.concat(
        map( pd.read_csv, sys.argv[1:] )
    )

    df.to_csv(
        make_output_path(),
        index=False,
    )

    print(
        sys.argv[1:],
        'concatenated to',
        make_output_path(),
    )

else:

    print('no files to concatenate')

# this allows us to #include the script in C++
")python3"
