from .genome_local_autoload import genome_local_autoload
from .genome_s3_autoload import genome_s3_autoload

# adapted from https://stackoverflow.com/a/31079085
__all__ = [
    'genome_local_autoload',
    'genome_s3_autoload',
]
