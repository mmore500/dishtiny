R"python3(" # this allows us to #include the script in C++

__doc__ = """Stitches frame images into a video file.

Frames with distinct 'a=', 'thread=', and 'proc=' keyname attributes are stitched into distinct videos.
Video output is saved into the current working directory with a programatically-generated filename.

Usage:
    ./animate_frames.py [frames_per_second] [max_frames or 0]

    Frame filenames are read from stdin.
"""

import cv2
from functools import reduce
from iterdub import iterdub as ib
from iterpop import iterpop as ip
from keyname import keyname as kn
import os
import pandas as pd
import sys
from tqdm import tqdm

def make_video( df, max_frames, frames_per_second, ):

    if max_frames and max_frames is not None:
        df = df[-max_frames:]

    sample_filename = df['_'].iloc[0]
    sample_frame = cv2.imread( sample_filename )
    height, width, layers = sample_frame.shape

    out_attrs = {
        column : ib.dub( df[column] )
        for column in df.columns
        if column not in { '_', 'ext', 'index', }
    }

    for k, v in out_attrs.items():
        if any( c in k or c in v for c in {'=', '+'} ):
            print('illegal character in out attr')
            print('key', k)
            print('value', v)
            print('source files')
            for filename in df['_']:
                print( filename )

    out_filename = kn.pack({
        **out_attrs, **{ 'ext' : '.mp4', }
    })
    out_path = (
        f'outvideos/{out_filename}'
        if os.path.isdir('./outvideos/')
        else out_filename
    )


    video = cv2.VideoWriter(
        out_path,
        cv2.VideoWriter_fourcc(*'mp4v'),
        frames_per_second,
        (width, height),
    )

    frames = ( cv2.imread( file ) for file in df['_'] )
    for frame in frames:
        video.write(frame)

    cv2.destroyAllWindows()
    video.release()

    print( ip.pophomogeneous(df['a']), 'video written to', out_path )


################################################################################
print(                                                                         )
print( 'running animate_frames.py'                                             )
print( '---------------------------------------------------------------------' )
################################################################################

try:
    frames_per_second, max_frames = map(int, sys.argv[1:])
except:
    print( __doc__ )
    sys.exit(1)

print( 'frames_per_second', frames_per_second )
print( 'max_frames', max_frames )

df = pd.DataFrame.from_records(
    [
        kn.unpack(image_filename)
        for line in sys.stdin
        for image_filename in line.split()
    ],
)

print( 'num image files', len(df) )

if len(df) == 0:
    print( 'no image files to process, exiting' )
    sys.exit(0)

df['update'] = pd.to_numeric( df['update'], errors='coerce', )
df.sort_values( by=['update'], inplace=True, )

for (a, thread, proc), subset_df in df.groupby(['a', 'thread', 'proc',]):
    print( 'a', a, )
    print( 'thread', thread, )
    print( 'proc', proc, )
    if len( subset_df ) > 2:
        print('stitching', len( subset_df ), 'frames for', a, )
        make_video( subset_df.reset_index(), max_frames, frames_per_second, )
    else:
        print('passing on', a, ', only', len( subset_df ), 'frames', )

# this allows us to #include the script in C++
")python3"
