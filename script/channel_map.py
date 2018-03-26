import pandas as pd
from matplotlib import pyplot as plt
import matplotlib.patches as patches
import colorsys
from tqdm import tqdm
import numpy as np
import sys
from pathlib import Path

CH_MAX = 4194304

assert 2 <= len(sys.argv) <= 3, \
    "First argument is filename, optional second argument is target update."

res = pd.read_csv(sys.argv[1])

# filter out just the target update
if len(sys.argv) == 3:
    update = int(sys.argv[2])
else:
    update = res['update'].max()

fil = res[res['update'] == update]

# what are the dimensions of the map we should draw?
X = int(fil['ch_x'].max())+1
Y = int(fil['ch_y'].max())+1

# arrange the pandas rows into a 2D array
mat = [[fil.loc[(fil['ch_x'] == x) & (fil['ch_y'] == y), ['channel0', 'channel1']] for y in range(Y)] for x in tqdm(range(X))]

# convert to RGB color --- hue for channel1, value for channel0
cmat = [[
        colorsys.hsv_to_rgb(
            row['channel1'].item()/CH_MAX,
            (row['channel0'].item()+ 0.25*CH_MAX)/(1.25*CH_MAX),
            0 if row['channel1'].item() == 0 else 1
    ) for row in s] for s in tqdm(mat)]

# draw cells
plt.imshow(np.swapaxes(np.array(cmat), 1, 0),extent=(0,X,Y,0))

# create borders ---
# solid black for different channel1,
# solid white for different channel0
lines = [(
    patches.Polygon(
        [[x,y], [x+1,y]],
        linewidth=0.5,
        edgecolor=('white' if mat[x][(y-1)%Y]['channel1'].item() == mat[x][y]['channel1'].item() else 'black'),
        alpha=(0.0 if mat[x][(y-1)%Y]['channel0'].item() == mat[x][y]['channel0'].item() else 1.0 if mat[x][y]['channel1'].item() == mat[x][(y-1)%Y]['channel1'].item() else 1.0),
        facecolor=None
        ),
    patches.Polygon(
        np.array([[x,y], [x,y+1]]),
        linewidth=0.5,
        edgecolor=('white' if mat[(x-1)%X][y]['channel1'].item() == mat[x][y]['channel1'].item() else 'black'),
        alpha=(0.0 if mat[(x-1)%X][y]['channel0'].item() == mat[x][y]['channel0'].item() else 1.0 if mat[(x-1)%X][y]['channel1'].item() == mat[x][y]['channel1'].item() else 1.0),
        facecolor=None
        )
    ) for y in tqdm(range(Y)) for x in range(X)]

flat_list = [item for sublist in lines for item in sublist]

# draw borders
for poly in flat_list:
    plt.gca().add_patch(poly)

# save result
plt.savefig(
        str(Path(sys.argv[1]).with_suffix('')) # strip .csv suffix
        + "_update" + str(update)
        + ".pdf"
    )
