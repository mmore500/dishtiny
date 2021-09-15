import deprecation
from keyname import keyname as kn

from . import get_env_context
from . import get_git_revision_hash
from . import get_notebook_cell_execution_count
from . import get_notebook_name
from . import get_notebook_path
from . import make_timestamp

@deprecation.deprecated(details='use nbmetalog package instead')
def make_outattr_metadata():

    return {
        '_context' : get_env_context(),
        '_nbcellexecutioncount' : get_notebook_cell_execution_count(),
        '_nbname' : kn.demote(get_notebook_name()),
        '_nbpath' : kn.demote(get_notebook_path()),
        '_revision' : get_git_revision_hash(),
        '_timestamp' : make_timestamp(),
    }
