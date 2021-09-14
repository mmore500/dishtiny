import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pytabulators import tabulate_fitness_complexity

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('variant competitions')
@announce_job_decorator_factory('variant competitions')
@only_every_nth_stint_decorator_factory(10)
def assemble_variant_competitions( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    variant_competitions, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/variant-competitions/stage=3+what=collated/stint={stint}/',
    )

    variant_df = pd.read_csv(
        f's3://{bucket}/{variant_competitions.key}',
    )

    res_df = tabulate_fitness_complexity(
        variant_df,
        get_control_t_distns( bucket, endeavor, stint ),
    )
    res_sources = [
        variant_competitions.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
