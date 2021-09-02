import deprecation
import os

@deprecation.deprecated(details='use nbmetalog package instead')
def get_env_context():
    return 'ci' if os.getenv('CI') else 'local'
