import sys
import os
import shutil

def ExtractSeed(filename):
    return int(next(
        ''.join(filter(lambda x: x.isdigit(), str)) for str in os.path.basename(filename).replace('.','+').split('+') if "update=" in str
    ))

filenames = sys.argv[1:]

todo = {ExtractSeed(filename) : filename for filename in filenames}

for idx, (update, filename) in enumerate(sorted(todo.items())):
    newname = 'frame-' + str(idx).zfill(4) + '.png'
    print(idx, filename, update, newname)
    os.system("cp " + filename + " " + newname)
