import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pytabulators import reshape_dpp_threadfirst

@try_except_missing_data_decorator_factory('evolve dpp metrics threadfirst')
@announce_job_decorator_factory('evolve dpp metrics threadfirst')
@only_every_nth_stint_decorator_factory(1)
def assemble_evolve_dpp_metrics_threadfirst( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    evolve_dpp_metrics, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/evolve/metrics/stage=1+what=collated/stint={stint}/a=demographic_phenotypic_phylogenetic_metrics+',
    )

    evolve_dpp_df = pd.read_csv(
        f's3://{bucket}/{evolve_dpp_metrics.key}',
    )

    res_df = reshape_dpp_threadfirst( evolve_dpp_df )

    # add suffix to prevent clash with monoculture dpp metrics
    res_df.rename(
        lambda col: \
            col if col in {
                'Series',
                'Stint',
            } \
            else f'{col} (evolve first)' \
        ,
        inplace=True,
        axis='columns',
    )

    res_sources = [
        evolve_dpp_metrics.key,
    ]

    return res_df, res_sources
