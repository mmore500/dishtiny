import sys
import os
import shutil
from keyname import keyname as kn

filenames = sys.argv[1:]

# check all data is from same software source
assert len({kn.unpack(filename)['_source_hash'] for filename in filenames}) == 1

todo = {kn.unpack(filename)['seed'] : filename for filename in filenames}

for idx, (update, filename) in enumerate(sorted(todo.items())):
    newname = 'frame-' + str(idx).zfill(4) + '.png'
    print(idx, filename, update, newname)
    os.system("cp " + filename + " " + newname)
