import colorsys
import numpy as np
from matplotlib import pyplot as plt
import matplotlib.patches as patches
from tqdm import tqdm

CH_MAX = 4194304

sample = range(1,len(range(1,CH_MAX,200000))+1)
dim = len(sample)

x_restricted = [0] + list(sample) + [dim+1]
x_restricted.remove(19)

y_restricted = [0] + list(sample) + [dim+1]
y_restricted.remove(6)

def draw(name, x_exclude=[], y_exclude=[]):

    cmat = [[
            colorsys.hsv_to_rgb(
                ch1/dim,
                (ch0 + 0.25*dim)/(1.25*dim),
                0 if ch1 == 0 else 1
        ) + (0.0 if ch1 in x_exclude or ch0 in y_exclude else 1.0,) for ch0 in sample] for ch1 in tqdm(sample)]


    plt.imshow(np.swapaxes(np.array(cmat), 1, 0),extent=(0,dim,dim,0))


    lines = [(
        patches.Polygon(
            [[x,y], [x+1,y]],
            linewidth=1.0,
            edgecolor=('white'),
            facecolor=None,
            alpha=(0.0 if len(y_exclude) != 0 or x+1 in x_exclude or y+1 in y_exclude else 1.0)
            ),
        patches.Polygon(
            np.array([[x,y], [x,y+1]]),
            linewidth=1.0,
            edgecolor=('black'),
            facecolor=None,
            alpha=(0.0 if len(x_exclude) != 0 or x+1 in x_exclude or y+1 in y_exclude else 1.0)
            )
        ) for y in tqdm(range(len(sample)+1)) for x in range(len(sample)+1)]

    flat_list = [item for sublist in lines for item in sublist]

    # draw borders
    for poly in flat_list:
        plt.gca().add_patch(poly)

    plt.gca().get_xaxis().set_ticks([])
    plt.gca().get_yaxis().set_ticks([])
    plt.xlabel('Level 2 Channel ID')
    plt.ylabel('Level 1 Channel ID')

    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)
    plt.gca().spines['bottom'].set_visible(False)
    plt.gca().spines['left'].set_visible(False)


    plt.savefig(
        'hsvmap-' + name + '.pdf',
        transparent=True,
        bbox_inches='tight',
        pad_inches=0
    )

    plt.clf()

draw("clumpsofclumps")

draw("clumpofclumps", x_restricted)

draw("clump", x_restricted, y_restricted)
