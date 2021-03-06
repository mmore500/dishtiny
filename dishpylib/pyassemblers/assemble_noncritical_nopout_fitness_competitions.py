import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pytabulators import tabulate_phenotype_equivalent_nopout_fitness

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory(
    'noncritical nopout fitness competitions'
)
@announce_job_decorator_factory(
    'noncritical nopout fitness competitions'
)
@only_every_nth_stint_decorator_factory(10)
def assemble_noncritical_nopout_fitness_competitions(
    *, bucket, endeavor, stint
):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    noncritical_nopout_competitions, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/noncritical-nopout-competitions/stage=5+what=collated/stint={stint}/',
    )

    noncritical_nopout_competition_df = pd.read_csv(
        f's3://{bucket}/{noncritical_nopout_competitions.key}',
    )

    res_df = tabulate_phenotype_equivalent_nopout_fitness(
        noncritical_nopout_competition_df,
        get_control_t_distns( bucket, endeavor, stint ),
    )
    res_sources = [
        noncritical_nopout_competitions.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
