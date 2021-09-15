import boto3
import pandas as pd

from dishpylib.pydecorators import announce_job_decorator_factory
from dishpylib.pydecorators import only_every_nth_stint_decorator_factory
from dishpylib.pydecorators import try_except_missing_data_decorator_factory
from dishpylib.pytabulators import tabulate_mutant_phenotype_differentiation

from ._validate_assembled_dataframe import validate_assembled_dataframe

@try_except_missing_data_decorator_factory('mutant phenotype-differentiation')
@announce_job_decorator_factory('mutant phenotype-differentiation')
@only_every_nth_stint_decorator_factory(10)
def assemble_mutant_phenotype_differentiation( *, bucket, endeavor, stint ):

    s3_handle = boto3.resource('s3')
    bucket_handle = s3_handle.Bucket(bucket)

    mutant_phenotype_differentiation, = bucket_handle.objects.filter(
        Prefix=f'endeavor={endeavor}/mutant-phenotype-differentiation/stage=2+what=collated/stint={stint}/',
    )

    mutant_df = pd.read_csv(
        f's3://{bucket}/{mutant_phenotype_differentiation.key}',
    )

    res_df = tabulate_mutant_phenotype_differentiation(
        mutant_df,
    )
    if 'Stint' not in res_df.columns: res_df['Stint'] = stint
    res_sources = [
        mutant_phenotype_differentiation.key,
    ]

    validate_assembled_dataframe( res_df, endeavor, stint )

    return res_df, res_sources
