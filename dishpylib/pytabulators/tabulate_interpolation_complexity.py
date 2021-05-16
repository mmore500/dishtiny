import functools
from multiprocessing import Pool
import pandas as pd
from tqdm import tqdm

from dishpylib.pyanalysis import estimate_interpolation_complexity
from dishpylib.pyhelpers import preprocess_competition_fitnesses

# have to define this out of line so it's pickle-able for multiprocessing
def _do_estimate_interpolation_complexity_work(kwargs):
    return estimate_interpolation_complexity(**kwargs)

def tabulate_interpolation_complexity(
        interpolation_competitions_df,
        control_fits_df,
    ):

    interpolation_competitions_df['Series'] \
        = interpolation_competitions_df['genome series']

    interpolation_competitions_df=preprocess_competition_fitnesses(
        interpolation_competitions_df,
        control_fits_df,
    )

    def do_work(series):
        return estimate_interpolation_complexity(
            series=series,
            interpolation_competitions_df=preprocess_competition_fitnesses(
                interpolation_competitions_df,
                control_fits_df,
            ),
        )

    with Pool() as pool:
        return pd.DataFrame( pool.map(
            _do_estimate_interpolation_complexity_work,
            tqdm([
                {
                    'interpolation_competitions_df'
                        : interpolation_competitions_df,
                    'series' : series,
                }
                for series in interpolation_competitions_df['Series'].unique()
            ])
        ) )
