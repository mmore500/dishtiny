import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('config records')
@announce_job_decorator_factory('config records')
@only_every_nth_stint_decorator_factory(1)
def assemble_config_records( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    config_records, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/evolve/config-records/stage=1+what=collated/stint={stint}/',
    )

    config_records_df = pd.read_csv(
        f's3://{bucket}/{config_records.key}',
    )

    config_records_df['Series'] = config_records_df['SERIES']
    config_records_df['Stint'] = config_records_df['STINT']

    res_df = config_records_df
    res_sources = [
        config_records.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
