import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pytabulators import reshape_dpp_threadfirst

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('monoculture dpp metrics threadfirst')
@announce_job_decorator_factory('monoculture dpp metrics threadfirst')
@only_every_nth_stint_decorator_factory(10)
def assemble_monoculture_dpp_metrics_threadfirst( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    monoculture_dpp_metrics, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/monocultures/metrics/stage=2+what=collated/stint={stint}/a=demographic_phenotypic_phylogenetic_metrics+',
    )

    monoculture_dpp_df = pd.read_csv(
        f's3://{bucket}/{monoculture_dpp_metrics.key}',
    )

    res_df = reshape_dpp_threadfirst( monoculture_dpp_df )

    # add suffix to prevent clash with monoculture dpp metrics
    res_df.rename(
        lambda col: \
            col if col in {
                'Series',
                'Stint',
            } \
            else f'{col} (monoculture first)' \
        ,
        inplace=True,
        axis='columns',
    )

    res_sources = [
        monoculture_dpp_metrics.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
