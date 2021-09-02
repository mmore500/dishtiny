import deprecation
from inspect import currentframe, getframeinfo
import re

@deprecation.deprecated(details='use nbmetalog package instead')
def get_notebook_cell_execution_count():
    frame = currentframe()
    while not re.search('ipython-input-[0-9]+-', getframeinfo(frame).filename):
        frame = frame.f_back

    return int(re.search(
        'ipython-input-([0-9]+)-',
        getframeinfo(frame).filename,
    ).group(1))
