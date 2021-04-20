import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pytabulators import filter_for_phenotype_neutral_nopout

@try_except_missing_data_decorator_factory('phenotype neutral nopouts')
@announce_job_decorator_factory('phenotype neutral nopouts')
@only_every_nth_stint_decorator_factory(10)
def assemble_phenotype_neutral_nopouts( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    genome_statistics, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/genomes/stage={2 if stint % 10 == 0 else 1}+what=collated/stint={stint}/',
    )

    genome_df = pd.read_csv(
        f's3://{bucket}/{genome_statistics.key}',
    )

    res_df = filter_for_phenotype_neutral_nopout( genome_df )
    res_sources = [
        genome_statistics.key,
    ]

    return res_df, res_sources
