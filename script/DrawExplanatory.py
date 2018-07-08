import matplotlib.pyplot as plt

# Generates the how-dishtiny-works schematic

# Make a 9x9 grid...
nrows, ncols = 7,7

b = (0.0,0.0,0.0)
w = (1.0,1.0,1.0)
gy = (0.5,0.5,0.5)
r =  (0.15,0.15,0.55)
p = (0.5,0.5,0.75)

bl = (0.37,0.78,1.0)
gr = (72.5,1.0,0.18)
bg = (0.9,0.2,0.6)
y = (1.0,0.8,0.2)
pp = (0.6,0.2,0.9)

images_res = [
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*3+[b]+[w]*3,
        [w]*7,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*3+[b]+[w]*3,
        [w]*2+[b]+[w]+[b]+[w]*2,
        [w]*3+[b]+[w]*3,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*3+[b]+[w]*3,
        [w]*2+[b]+[w]+[b]+[w]*2,
        [w]+[b]+[w]*3+[b]+[w],
        [w]*2+[b]+[w]+[b]+[w]*2,
        [w]*3+[b]+[w]*3,
        [w]*7
    ]
] + [[
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7
    ]]*4

images_sig_a = [
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*7,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*2+[w]+[p]+[w]+[w]*2,
        [w]*3+[w]+[w]*3,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*3+[w]+[w]*3,
        [w]*2+[w]+[p]+[w]+[w]*2,
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [w]*2+[w]+[w]+[w]+[w]*2,
        [w]*3+[w]+[w]*3,
        [w]*7
    ],
    [
        [w]*7,
        [w]*3+[w]+[w]*3,
        [w]*2+[w]+[w]+[w]+[w]*2,
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [w]*2+[w]+[w]+[w]+[w]*2,
        [w]*3+[w]+[w]*3,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7
    ]
]

images_sig_b = [
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*7,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*2+[r]+[p]+[r]+[w]*2,
        [w]*3+[r]+[w]*3,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*2+[r]+[p]+[r]+[w]*2,
        [w]+[r]+[p]+[w]+[p]+[r]+[w],
        [w]*2+[r]+[p]+[r]+[w]*2,
        [w]*3+[r]+[w]*3,
        [w]*7
    ],
    [
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [w]+[w]+[w]+[p]+[w]+[w]+[w],
        [w]+[w]+[p]+[w]+[p]+[w]+[w],
        [w]+[p]+[w]+[w]+[w]+[p]+[w],
        [w]+[w]+[p]+[w]+[p]+[w]+[w],
        [w]+[w]+[w]+[p]+[w]+[w]+[w],
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
    ]] + [
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7
    ]]*3

images_sig_c = [
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*7,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*2+[r]+[p]+[r]+[w]*2,
        [w]*3+[r]+[w]*3,
        [w]*7,
        [w]*7
    ],
    [
        [w]*7,
        [w]*3+[r]+[w]*3,
        [w]*2+[r]+[p]+[r]+[w]*2,
        [w]+[r]+[p]+[w]+[p]+[r]+[w],
        [w]*2+[r]+[p]+[r]+[w]*2,
        [w]*3+[r]+[w]*3,
        [w]*7
    ],
    [
        [w]+[w]+[w]+[r]+[w]+[w]+[w],
        [w]+[w]+[r]+[p]+[r]+[w]+[w],
        [w]+[r]+[p]+[w]+[p]+[r]+[w],
        [r]+[p]+[w]+[w]+[w]+[p]+[r],
        [w]+[r]+[p]+[w]+[p]+[r]+[w],
        [w]+[w]+[r]+[p]+[r]+[w]+[w],
        [w]+[w]+[w]+[r]+[w]+[w]+[w],
    ],
    [
        [w]+[w]+[w]+[p]+[r]+[w]+[w],
        [w]+[w]+[p]+[w]+[p]+[w]+[w],
        [r]+[p]+[w]+[w]+[w]+[p]+[r],
        [p]+[w]+[w]+[w]+[w]+[w]+[p],
        [w]+[p]+[w]+[w]+[w]+[p]+[w],
        [w]+[w]+[p]+[w]+[p]+[w]+[w],
        [w]+[w]+[w]+[p]+[w]+[w]+[w],
    ],
    [
        [w]+[w]+[w]+[w]+[p]+[w]+[w],
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [p]+[w]+[w]+[w]+[w]+[w]+[p],
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
        [w]+[w]+[w]+[w]+[w]+[w]+[w],
    ],
    [
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7,
        [w]*7
    ]
]

ch_img_a = [
            [bl]+[bl]+[bl]+[bg]+[bg]+[bg]+[bg],
            [bl]+[bl]+[bl]+[bg]+[bg]+[bg]+[bg],
            [bl]+[bl]+[bl]+[gr]+[bg]+[bg]+[bg],
            [bl]+[bl]+[bl]+[gr]+[bg]+[bg]+[bg],
            [pp]+[pp]+[pp]+[y ]+[y ]+[y ]+[y ],
            [pp]+[pp]+[pp]+[y ]+[y ]+[y ]+[y ],
            [pp]+[pp]+[pp]+[y ]+[y ]+[y ]+[y ],
        ]

ch_img_b = [
            [bl]+[bl]+[bl]+[bg]+[bg]+[bg]+[bg],
            [bl]+[bl]+[bl]+[gr]+[bg]+[bg]+[bg],
            [bl]+[bl]+[gr]+[gr]+[gr]+[bg]+[bg],
            [bl]+[gr]+[gr]+[gr]+[gr]+[gr]+[bg],
            [pp]+[pp]+[gr]+[gr]+[gr]+[y ]+[y ],
            [pp]+[pp]+[pp]+[gr]+[y ]+[y ]+[y ],
            [pp]+[pp]+[pp]+[y ]+[y ]+[y ]+[y ],
        ]

ch_img_c = [
            [bl]+[bl]+[bl]+[gr]+[gr]+[bg]+[bg],
            [bl]+[bl]+[gr]+[gr]+[gr]+[gr]+[bg],
            [gr]+[gr]+[gr]+[gr]+[gr]+[gr]+[gr],
            [gr]+[gr]+[gr]+[gr]+[gr]+[gr]+[gr],
            [pp]+[gr]+[gr]+[gr]+[gr]+[gr]+[y ],
            [pp]+[pp]+[gr]+[gr]+[gr]+[y ]+[y ],
            [pp]+[pp]+[pp]+[gr]+[y ]+[y ]+[y ],
        ]

end_state_a = [
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 1]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 1]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
        ]

gn=(0.89,1.0,0.82)
rn=(1.0, 0.3, 0.3)

end_color_a = [
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[gn]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[gn]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
        ]

end_state_b = [
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 1]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 1]+[ 1]+[ 1]+[ 0]+[ 0],
            [ 0]+[ 1]+[ 1]+[ 1]+[ 1]+[ 1]+[ 0],
            [ 0]+[ 0]+[ 1]+[ 1]+[ 1]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 1]+[ 0]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0]+[ 0],
        ]

end_color_b = [
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[gn]+[ w]+[ w]+[ w],
            [ w]+[ w]+[gn]+[gn]+[gn]+[ w]+[ w],
            [ w]+[gn]+[gn]+[gn]+[gn]+[gn]+[ w],
            [ w]+[ w]+[gn]+[gn]+[gn]+[ w]+[ w],
            [ w]+[ w]+[ w]+[gn]+[ w]+[ w]+[ w],
            [ w]+[ w]+[ w]+[ w]+[ w]+[ w]+[ w],
        ]

end_state_c = [
            [ 0]+[ 0]+[ 0]+[-5]+[-5]+[ 0]+[ 0],
            [ 0]+[ 0]+[-5]+[ 1]+[-5]+[ 0]+[ 0],
            [-5]+[-5]+[ 1]+[ 1]+[ 1]+[-5]+[-5],
            [-5]+[ 1]+[ 1]+[ 1]+[ 1]+[ 1]+[-5],
            [ 0]+[-5]+[ 1]+[ 1]+[ 1]+[-5]+[ 0],
            [ 0]+[ 0]+[-5]+[ 1]+[-5]+[ 0]+[ 0],
            [ 0]+[ 0]+[ 0]+[-5]+[ 0]+[ 0]+[ 0],
        ]

end_color_c = [
            [ w]+[ w]+[ w]+[rn]+[rn]+[ w]+[ w],
            [ w]+[ w]+[rn]+[gn]+[rn]+[ w]+[ w],
            [rn]+[rn]+[gn]+[gn]+[gn]+[rn]+[rn],
            [rn]+[gn]+[gn]+[gn]+[gn]+[gn]+[rn],
            [ w]+[rn]+[gn]+[gn]+[gn]+[rn]+[ w],
            [ w]+[ w]+[rn]+[gn]+[rn]+[ w]+[ w],
            [ w]+[ w]+[ w]+[rn]+[ w]+[ w]+[ w],
        ]


def draw(im, ax):
    ax.imshow(im,extent=(0,7,7,0))

    major_ticks=[0,1,2,3,4,5,6,7]

    ax.set_xticks(major_ticks)
    ax.set_yticks(major_ticks)

    # hide tickmarks
    for tic in ax.xaxis.get_major_ticks() + ax.yaxis.get_major_ticks():
        tic.tick1On = tic.tick2On = False
        tic.label1On = tic.label2On = False

    # Gridlines based on minor ticks
    ax.grid(which='Major',color='black', linestyle='-', linewidth=1)

    return ax

f, (r1, r2, r3, r4) = plt.subplots(4,9);

r1[0].remove()

draw(ch_img_a, r2[0]).set_ylabel('a)    ', rotation=0, fontsize=24)
draw(ch_img_b, r3[0]).set_ylabel('b)    ', rotation=0, fontsize=24)
axe = draw(ch_img_c, r4[0])
axe.set_ylabel('c)    ', rotation=0, fontsize=24)
axe.set_xlabel('channel\nlayout', fontsize=24)

for t, (im, ax) in enumerate(zip(images_res, r1[1:])):
    if (t == 0): draw(im,ax).set_ylabel('resource', fontsize=24)
    else: draw(im,ax)

for t, (im, ax) in enumerate(zip(images_sig_a, r2[1:])):
    if (t == 0): draw(im,ax).set_ylabel('signal', fontsize=16)
    else: draw(im,ax)

for t, (im, ax) in enumerate(zip(images_sig_b, r3[1:])):
    if (t == 0): draw(im,ax).set_ylabel('signal', fontsize=16)
    else: draw(im,ax)

for t, (im, ax) in enumerate(zip(images_sig_c, r4[1:])):
    if (t == 0):
        axe = draw(im,ax)
        axe.set_ylabel('signal', fontsize=16)
        axe.set_xlabel('$t=%d$'%t, fontsize=24)
    else: draw(im,ax).set_xlabel('$t=%d$'%t, fontsize=24)

r1[-1].remove()

def drawmat(ax, state, color):
    draw(color, ax)

    for i in range(7):
        for j in range(7):
            c = state[j][i]
            ax.text(i+0.1, j+0.75, ('+' if c >= 0 else '$\minus$') + str(abs(c)))

    return ax

drawmat(r2[-1], end_state_a, end_color_a)
drawmat(r3[-1], end_state_b, end_color_b)
drawmat(r4[-1], end_state_c, end_color_c)

r4[-1].set_xlabel('net harvest', fontsize=24)

plt.gcf().set_size_inches(24, 12)
plt.savefig('explanatory.pdf')
