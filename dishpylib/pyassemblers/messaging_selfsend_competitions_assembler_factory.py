import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pytabulators import tabulate_selfsend_fitness

from ._validate_assembled_dataframe import validate_assembled_dataframe

def messaging_selfsend_competitions_assembler_factory(messaging_target):

    @try_except_missing_data_decorator_factory(
        f'{messaging_target} messaging self-send'
    )
    @announce_job_decorator_factory( f'{messaging_target} messaging self-send' )
    @only_every_nth_stint_decorator_factory(10)
    def assemble_messaging_selfsend_competitions(*, bucket, endeavor, stint):

        s3_handle = boto3.resource('s3')
        bucket_handle = s3_handle.Bucket(bucket)

        selfsend_competitions, = bucket_handle.objects.filter(
            Prefix=f'endeavor={endeavor}/selfsend-{messaging_target.lower()}-competitions/stage=2+what=collated/stint={stint}/',
        )

        selfsend_df = pd.read_csv(
            f's3://{bucket}/{selfsend_competitions.key}',
        )

        res_df = tabulate_selfsend_fitness(
            selfsend_df,
            get_control_t_distns( bucket, endeavor, stint ),
            messaging_target,
        )
        if 'Stint' not in res_df.columns: res_df['Stint'] = stint
        res_sources = [
            selfsend_competitions.key,
        ]

        validate_assembled_dataframe( res_df, endeavor, stint )

        return res_df, res_sources

    return assemble_messaging_selfsend_competitions
