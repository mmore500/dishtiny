import deprecation
import ipynbname
import os

@deprecation.deprecated(details='use nbmetalog package instead')
def get_notebook_path():
    try:
        return str(
            ipynbname.path()
        )
    except:
        return str(
            os.getenv('NOTEBOOK_PATH')
        )
