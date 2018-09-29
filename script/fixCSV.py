import subprocess
import sys
from tqdm import tqdm

for fname in tqdm(sys.argv[1:]):

    with open(fname) as f:
        first_line = f.readline()

        comma_count = first_line.count(',')

        comma_match = '^[^,]+' + ',[^,]+'*comma_count+'$'

        subprocess.call(['sed','-r','-i','/'+comma_match+'/!s/^/#/',fname])

        # clear out null characters
        subprocess.call(['sed', '-r', '-i', r's/\x0//g',fname])
