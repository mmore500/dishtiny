import os

def get_env_context():
    return 'ci' if os.getenv('CI') else 'local'
