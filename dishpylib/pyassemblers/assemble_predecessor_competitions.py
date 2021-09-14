import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pytabulators import tabulate_predecessor_fitness

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('predecessor competitions')
@announce_job_decorator_factory('predecessor competitions')
@only_every_nth_stint_decorator_factory(10)
def assemble_predecessor_competitions( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    predecessor_competitions, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/predecessor-competitions/stage=2+what=collated/stint={stint}/',
    )

    predecessor_df = pd.read_csv(
        f's3://{bucket}/{predecessor_competitions.key}',
    )

    res_df =  tabulate_predecessor_fitness( predecessor_df )
    res_sources = [
        predecessor_competitions.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
