import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pytabulators import tabulate_interpolation_complexity

@try_except_missing_data_decorator_factory(
    'noncritical-phenotypeneutral nopinterpolation competitions'
)
@announce_job_decorator_factory(
    'noncritical-phenotypeneutral nopinterpolation competitions'
)
@only_every_nth_stint_decorator_factory(10)
def assemble_noncritical_phenotypeequivalent_nopinterpolation_competitions(
        *, bucket, endeavor, stint
    ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    interpolation_competitions, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/noncritical-phenotypeneutral-nopinterpolation-competitions/stage=6+what=collated/stint={stint}/',
    )

    interpolation_competitions_df = pd.read_csv(
        f's3://{bucket}/{interpolation_competitions.key}',
    )

    res_df = tabulate_interpolation_complexity(
        interpolation_competitions_df,
        get_control_t_distns(bucket, endeavor, stint),
    )
    res_sources = [
        interpolation_competitions.key
    ]

    return res_df, res_sources
