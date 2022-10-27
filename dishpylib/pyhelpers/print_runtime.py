import inspect
import socket
import sys
import types

from . import get_env_context
from . import get_git_revision_hash
from . import make_timestamp

def print_runtime():

    print( 'context:', get_env_context() )
    print( 'hostname:', socket.gethostname() )
    print( 'interpreter:', sys.version.replace('\n', ' ').strip() )
    print( 'revision:', get_git_revision_hash() )
    print( 'timestamp:', make_timestamp() )

    print()

    caller_globals = inspect.stack()[1][0].f_globals

    caller_base_module_names = [
        val.__name__.split('.')[0]
        for val in caller_globals.values()
        if isinstance(val, types.ModuleType)
    ] + [
        'IPython',
    ]

    for base_module in sorted(caller_base_module_names):
        exec(f'import {base_module}', globals())
        exec(
            f'version = getattr({base_module}, "__version__", None)',
            globals(),
        )
        if version:
            print(f'{base_module}=={version}')
