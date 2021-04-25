from keyname import keyname as kn
import matplotlib
import matplotlib.pyplot as plt
import pathlib
from slugify import slugify

# how to get the data name
def tee_plot(plotter, *args, **kwargs):

    # enable TrueType fonts
    # see https://gecco-2021.sigevo.org/Paper-Submission-Instructions
    matplotlib.rcParams['pdf.fonttype'] = 42
    matplotlib.rcParams['ps.fonttype'] = 42

    res = plotter(*args, **kwargs)

    out_filename = kn.pack({
        **{
            slugify(k) : slugify(v)
            for k, v in kwargs.items()
            if isinstance(v, str)
        },
        **{
            'viz' : slugify(plotter.__name__),
            'ext' : '.pdf',
        },
    })

    out_folder = 'outplots'
    pathlib.Path(
        out_folder,
    ).mkdir(
        parents=True,
        exist_ok=True,
    )

    out_path = f'{out_folder}/{out_filename}'
    print(out_path)

    plt.savefig(
        f'outplots/{out_filename}',
        bbox_inches='tight',
        transparent=True,
    )

    return res
