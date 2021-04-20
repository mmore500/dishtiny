import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pytabulators import tabulate_mutant_fitness

@try_except_missing_data_decorator_factory('point mutant competitions')
@announce_job_decorator_factory('point mutant competitions')
@only_every_nth_stint_decorator_factory(10)
def assemble_point_mutant_competitions( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    mutant_competitions, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/mutant-point-competitions/stage=2+what=collated/stint={stint}/',
    )

    mutant_df = pd.read_csv(
        f's3://{bucket}/{mutant_competitions.key}',
    )

    res_df = tabulate_mutant_fitness(
        mutant_df,
        get_control_t_distns( bucket, endeavor, stint ),
        'Point ',
    )
    sources.append( mutant_competitions.key )

    return res_df, res_sources
