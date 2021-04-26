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

    res = plotter(
        *args,
        **{
            k : v
            for k, v in kwargs.items()
            if k != 'outattrs'
        }
    )

    out_filenamer = lambda ext: kn.pack({
        **{
            slugify(k) : slugify(v)
            for k, v in kwargs.items()
            if isinstance(v, str)
        },
        **kwargs.get('outattrs', {}),
        **{
            'viz' : slugify(plotter.__name__),
            'ext' : ext,
        },
    })

    out_folder = 'outplots'
    pathlib.Path(
        out_folder,
    ).mkdir(
        parents=True,
        exist_ok=True,
    )

    out_path = f'outplots/{out_filenamer(".pdf")}'
    print(out_path)
    plt.savefig(
        out_path,
        bbox_inches='tight',
        transparent=True,
    )

    out_path = f'outplots/{out_filenamer(".png")}'
    print(out_path)
    plt.savefig(
        out_path,
        bbox_inches='tight',
        transparent=True,
        dpi=300,
    )

    return res
