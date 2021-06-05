import ipynbname
import os

def get_notebook_path():
    try:
        return str(
            ipynbname.path()
        )
    except:
        return str(
            os.getenv('NOTEBOOK_PATH')
        )
