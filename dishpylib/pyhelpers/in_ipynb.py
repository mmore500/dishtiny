# adapted from https://stackoverflow.com/a/24937408
def in_ipynb():
    try:
        cfg = get_ipython().config
        return cfg['IPKernelApp']['parent_appname'] == 'ipython-notebook'
    except NameError:
        return False
