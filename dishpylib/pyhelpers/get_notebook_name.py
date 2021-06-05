import ipynbname
import os

def get_notebook_name():
    try:
        return ipynbname.name()
    except:
        return str(
            os.getenv('NOTEBOOK_NAME')
        )
