import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pytabulators import reshape_dpp_threadmean

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('evolve dpp metrics threadmean')
@announce_job_decorator_factory('evolve dpp metrics threadmean')
@only_every_nth_stint_decorator_factory(1)
def assemble_evolve_dpp_metrics_threadmean( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    evolve_dpp_metrics, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/evolve/metrics/stage=1+what=collated/stint={stint}/a=demographic_phenotypic_phylogenetic_metrics+',
    )

    evolve_dpp_df = pd.read_csv(
        f's3://{bucket}/{evolve_dpp_metrics.key}',
    )

    res_df = reshape_dpp_threadmean( evolve_dpp_df )

    # add suffix to prevent clash with monoculture dpp metrics
    res_df.rename(
        lambda col: \
            col if col in {
                'Series',
                'Stint',
            } \
            else f'{col} (evolve mean)' \
        ,
        inplace=True,
        axis='columns',
    )

    res_sources = [
        evolve_dpp_metrics.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
