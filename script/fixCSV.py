import subprocess
import sys

for fname in sys.argv[1:]:

    with open(fname) as f:
        first_line = f.readline()

        comma_count = first_line.count(',')

        comma_match = '^[^,]+' + ',[^,]+'*comma_count+'$'

        subprocess.call(['sed','-r','-i','/'+comma_match+'/!s/^/#/',fname])
