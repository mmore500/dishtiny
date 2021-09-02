import deprecation
import ipynbname
import os

@deprecation.deprecated(details='use nbmetalog package instead')
def get_notebook_name():
    try:
        return ipynbname.name()
    except:
        return str(
            os.getenv('NOTEBOOK_NAME')
        )
