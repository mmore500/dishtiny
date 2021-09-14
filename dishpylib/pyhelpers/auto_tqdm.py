from tqdm import tqdm as native_tqdm
from tqdm.notebook import tqdm as notebook_tqdm

from dishpylib.pyhelpers.in_ipynb import in_ipynb

auto_tqdm = notebook_tqdm if in_ipynb() else native_tqdm
