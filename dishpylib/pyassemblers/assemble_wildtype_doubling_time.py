import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pytabulators import tabulate_wildtype_doubling_time

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('wildtype doubling time')
@announce_job_decorator_factory('wildtype doubling time')
@only_every_nth_stint_decorator_factory(10)
def assemble_wildtype_doubling_time( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    wildtype_doubling_time, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/wildtype-doubling-time/stage=2+what=collated/stint={stint}/',
    )

    wildtype_doubling_time_df = pd.read_csv(
        f's3://{bucket}/{wildtype_doubling_time.key}',
    )

    res_df = tabulate_wildtype_doubling_time( wildtype_doubling_time_df )
    if 'Stint' not in res_df.columns: res_df['Stint'] = stint
    res_sources = [
        wildtype_doubling_time.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
