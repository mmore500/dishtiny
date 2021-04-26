import boto3
import pandas as pd

from dishpylib.pydecorators import \
    announce_job_decorator_factory, \
    only_every_nth_stint_decorator_factory, \
    try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pytabulators import tabulate_either_perturbation_fitness

def either_perturbation_competitions_assembler_factory(target_state):

    @try_except_missing_data_decorator_factory(
        f'{target_state} state either perturbation competitions'
    )
    @announce_job_decorator_factory(
            f'{target_state} state either perturbation competitions'
    )
    @only_every_nth_stint_decorator_factory(10)
    def assemble_either_perturbation_competitions(*, bucket, endeavor, stint):

        s3_handle = boto3.resource('s3')
        bucket_handle = s3_handle.Bucket(bucket)

        exchange_competitions, = bucket_handle.objects.filter(
            Prefix=f'endeavor={endeavor}/perturbation-{target_state.lower()}-state-exchange-competitions/stage=2+what=collated/stint={stint}/',
        )
        rotate_competitions, = bucket_handle.objects.filter(
            Prefix=f'endeavor={endeavor}/perturbation-{target_state.lower()}-state-rotate-competitions/stage=2+what=collated/stint={stint}/',
        )

        control_distns = get_control_t_distns( bucket, endeavor, stint )

        exchange_df = pd.read_csv(
            f's3://{bucket}/{exchange_competitions.key}',
        )
        rotate_df = pd.read_csv(
            f's3://{bucket}/{rotate_competitions.key}',
        )

        res_df = tabulate_either_perturbation_fitness(
            exchange_df,
            rotate_df,
            control_distns,
            target_state,
        )
        res_sources = [
            exchange_competitions.key,
            rotate_competitions.key,
        ]

        return res_df, res_sources

    return assemble_either_perturbation_competitions
