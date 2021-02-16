import pandas as pd
import boto3
from tqdm import tqdm
from io import StringIO
from keyname import keyname as kn
import itertools
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('Bucket',
                    metavar='bucket',
                    type=str,
                    help='bucket to run on')
parser.add_argument('Prefix',
                    metavar='prefix',
                    type=str,
                    help='prefix to concatenate')

if __name__ == '__main__':
    args = parser.parse_args()



bucket = args.Bucket
prefix = args.Prefix

#bucket = 'dnh2v'
#prefix = 'endeavor=10/evolve/metrics/stint=1/'

client = boto3.client('s3', region_name='us-west-2')

# Create a reusable Paginator
paginator = client.get_paginator('list_objects')

operation_parameters = {'Bucket': bucket,
                        'Prefix': prefix}


# Create a PageIterator from the Paginator
page_iterator = paginator.paginate(**operation_parameters)

matches = []

for page in page_iterator:
    for key in page['Contents']:
        if key['Key'].endswith(".csv"):
            matches.append(key['Key'])


filenames = [set(kn.unpack(match).items()) for match in matches]
intersect = set.intersection(*filenames)

common_keys = dict(intersect)
filename = kn.pack(common_keys)

df = pd.concat((tqdm((pd.read_csv(f's3://{bucket}/{dir}') for dir in matches), total=len(matches))))

buffer = StringIO()
df.to_csv(buffer, index=False, compression="xz")

s3_resource = boto3.resource('s3')
s3_resource.Object(bucket, f'/consolidated/{prefix}/{filename}.csv.xz').put(Body=buffer.getvalue())
