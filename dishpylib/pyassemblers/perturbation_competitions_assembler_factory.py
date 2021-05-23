import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pyhelpers import get_control_t_distns
from dishpylib.pytabulators import tabulate_perturbation_fitness

from ._validate_assembled_dataframe import validate_assembled_dataframe

def perturbation_competitions_assembler_factory(
        target_state, perturbation_type
    ):

    @try_except_missing_data_decorator_factory(
        f'{target_state} state {perturbation_type} perturbation competitions'
    )
    @announce_job_decorator_factory(
            f'{target_state} state {perturbation_type} perturbation competitions'
    )
    @only_every_nth_stint_decorator_factory(10)
    def assemble_perturbation_competitions( *, bucket, endeavor, stint ):

        s3_handle = boto3.resource('s3')
        bucket_handle = s3_handle.Bucket(bucket)

        control_distns = get_control_t_distns( bucket, endeavor, stint )

        perturbation_competitions, = bucket_handle.objects.filter(
            Prefix=f'endeavor={endeavor}/perturbation-{target_state.lower()}-state-{perturbation_type.lower()}-competitions/stage=2+what=collated/stint={stint}/',
        )

        perturbation_df = pd.read_csv(
            f's3://{bucket}/{perturbation_competitions.key}',
        )

        res_df = tabulate_perturbation_fitness(
            perturbation_df,
            control_distns,
            target_state,
            perturbation_type,
        )
        if 'Stint' not in res_df.columns: res_df['Stint'] = stint
        res_sources = [
            perturbation_competitions.key,
        ]

        validate_assembled_dataframe( res_df, endeavor, stint )

        return res_df, res_sources

    return assemble_perturbation_competitions
