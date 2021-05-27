from functools import lru_cache
import pathlib
import subprocess

@lru_cache()
def get_git_revision_hash():
    return subprocess.check_output([
        'git',
        '-C',
        pathlib.Path(__file__).parent.absolute(),
        'rev-parse',
        '--short',
        'HEAD',
    ]).decode('ascii').strip()
