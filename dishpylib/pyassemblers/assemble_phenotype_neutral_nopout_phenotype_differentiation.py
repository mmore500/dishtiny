import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pytabulators import \
    tabulate_phenotype_neutral_nopout_phenotype_differentiation

@try_except_missing_data_decorator_factory(
    'phenotype neutral nopout phenotype-differentiation'
)
@announce_job_decorator_factory(
    'phenotype neutral nopout phenotype-differentiation'
)
@only_every_nth_stint_decorator_factory(10)
def assemble_phenotype_neutral_nopout_phenotype_differentiation(
    *, bucket, endeavor, stint
):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    phenotype_neutral_nopout_phenotype_differentiation, = \
        bucket_handle.objects.filter(
            Prefix=f'endeavor={endeavor}/phenotype-neutral-nopout-phenotype-differentiation/stage=3+what=collated/stint={stint}/',
        )

    phenotype_neutral_nopout_phenotype_differentiation_df = pd.read_csv(
        f's3://{bucket}/{phenotype_neutral_nopout_phenotype_differentiation.key}',
    )

    res_df = tabulate_phenotype_neutral_nopout_phenotype_differentiation(
        phenotype_neutral_nopout_phenotype_differentiation_df,
    )
    res_sources = [
        phenotype_neutral_nopout_phenotype_differentiation.key,
    ]

    return res_df, res_sources
