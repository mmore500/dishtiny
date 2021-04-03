import gzip
import json
from keyname import keyname as kn

def genome_local_autoload(target):
    if kn.unpack( target )['ext'] == '.json':
        with open( target, 'r') as f:
            return json.load( f )

    elif kn.unpack( target )['ext'] == '.json.gz':
        try:
            with gzip.open( target, 'rb') as f:
                return json.loads( f.read().decode('ascii') )
        except Exception:
            pass

        try:
            with gzip.open( target, 'rb') as f:
                return json.loads( f.read().decode('utf-8') )
        except Exception:
            pass

    raise ValueError
