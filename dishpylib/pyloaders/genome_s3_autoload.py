import boto3
import gzip
import json
from keyname import keyname as kn
import re
import tempfile

def genome_s3_autoload( s3_url ):

    s3 = boto3.client('s3')

    bucket = re.search('s3://(.+?)/', s3_url).group(1)
    object_name = re.search(f's3://{bucket}/(.+)', s3_url).group(1)

    with tempfile.NamedTemporaryFile(mode='w+b') as temp:
        print('temporary file', temp.name)

        s3.download_fileobj(bucket, object_name, temp)

        if kn.unpack( s3_url )['ext'] == '.json':
            with open( temp.name, 'r') as f:
                return json.load( f )

        elif kn.unpack( s3_url )['ext'] == '.json.gz':
            try:
                with gzip.open( temp.name, 'rb') as f:
                    return json.loads( f.read().decode('ascii') )
            except Exception:
                pass

            try:
                with gzip.open( temp.name, 'rb') as f:
                    return json.loads( f.read().decode('utf-8') )
            except Exception:
                pass

        raise ValueError
