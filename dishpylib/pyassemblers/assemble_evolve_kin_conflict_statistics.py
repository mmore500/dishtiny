import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pytabulators import reshape_kin_conflict

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory(
    'evolve kin conflict statistics'
)
@announce_job_decorator_factory('evolve kin conflict statistics')
@only_every_nth_stint_decorator_factory(10)
def assemble_evolve_kin_conflict_statistics( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    evolve_kin_conflict_statistics, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/evolve/metrics/stage=1+what=collated/stint={stint}/a=kin_conflict_statistics+'
    )

    evolve_kin_conflict_df = pd.read_csv(
        f's3://{bucket}/{evolve_kin_conflict_statistics.key}'
    )

    res_df = reshape_kin_conflict( evolve_kin_conflict_df )
    # add suffix to prevent clash with monoculture dpp metrics
    res_df.rename(
        lambda col: \
            col if col in {
                'Series',
                'Stint',
            } \
            else f'{col} (evolve)' \
        ,
        inplace=True,
    )

    res_sources = [
        evolve_kin_conflict_statistics.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
