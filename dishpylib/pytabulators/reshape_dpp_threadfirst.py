from .reshape_dpp import reshape_dpp

def reshape_dpp_threadfirst(df):

    # take mean of columns over thread, proc
    return reshape_dpp(df).groupby([
        'Stint',
        'Series',
    ]).first().reset_index()
