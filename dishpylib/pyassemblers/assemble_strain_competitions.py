import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pytabulators import tabulate_strain_fitness

@try_except_missing_data_decorator_factory('strain competitions')
@announce_job_decorator_factory('strain competitions')
@only_every_nth_stint_decorator_factory(10)
def assemble_strain_competitions( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    strain_competitions, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/strain-competitions/stage=2+what=collated/stint={stint}/',
    )

    strain_df = pd.read_csv(
        f's3://{bucket}/{strain_competitions.key}',
    )

    res_df = tabulate_strain_fitness( strain_df )
    res_sources = [
        strain_competitions.key,
    ]

    return res_df, res_sources
