"""Generate web/includes/population-loader.html for prq49-backport.

Lists bundled series 16005 genomes from every available stint.
Run from the repository root, passing the directory of bundled genomes,
which the page serves at genomes/.
Usage: python3 web/make_population_loader.py path/to/genomes
"""

import os
import re
import sys
from urllib.parse import quote

DIR = "web/"
OUTPUT = "includes/population-loader.html"
GENOMES = "genomes/"

# config overrides applied to every link
PARAMS = (
    "COLLECTIVE_HARVEST_RATE=0%090.25"
    "&GROUP_EXPIRATION_DURATIONS=0%091024"
    "&OPTIMAL_QUORUM_COUNT=0%0912"
    "&P_QUORUM_CAP_KILL=0%090.0825"
    "&P_SET_QUORUM_BIT=0%091"
    "&QUORUM_CAP=0%0936"
    "&N_CELLS=900"
)

FOCAL = (
    "a=genome+criteria=abundance+morph=wildtype+proc=0+series=16005"
    "+stint={stint}+thread=0+variation=master+ext=.json.gz"
)
BACKGROUND = (
    "a=genome+criteria=abundance_highest_root+morph=wildtype+proc=0"
    "+series=16005+stint={stint}+thread=0+variation=master+ext=.json.gz"
)


def available_stints(pattern, genomes_dir):
    regex = re.compile(re.escape(pattern).replace(re.escape("{stint}"), r"(\d+)") + "$")
    return sorted(
        int(match.group(1))
        for filename in os.listdir(genomes_dir)
        if (match := regex.match(filename))
    )


def autoinstall(filename):
    return "autoinstall=" + quote(GENOMES + filename, safe="/")


def link(genesis, *filenames):
    query = "&".join([f"GENESIS={genesis}", *map(autoinstall, filenames), PARAMS])
    return f"?{query}"


def button(href, label):
    return (
        f'<a href="{href}" role="button" class="btn btn-light btn-sm">' f"{label}</a>"
    )


def make_row(stint, focal_stints, background_stints):
    focal = FOCAL.format(stint=stint)
    background = BACKGROUND.format(stint=stint)
    has_focal = stint in focal_stints
    has_background = stint in background_stints
    cells = [
        str(stint),
        button(link("monoculture", focal), "monoculture") if has_focal else "",
        (
            button(link("monoculture", background), "monoculture")
            if has_background
            else ""
        ),
        (
            button(link("multiculture", focal, background), "multiculture")
            if has_focal and has_background
            else ""
        ),
    ]
    return "<tr>" + "".join(f"<td>{cell}</td>" for cell in cells) + "</tr>"


def make_html(genomes_dir):
    focal_stints = available_stints(FOCAL, genomes_dir)
    background_stints = available_stints(BACKGROUND, genomes_dir)
    stints = sorted(set(focal_stints) | set(background_stints))
    rows = "\n".join(
        make_row(stint, focal_stints, background_stints) for stint in stints
    )
    return f"""<div class="card">
<div class="card-header">
    <span data-toggle="collapse" href="#population-loader-body">
    <button
        class="btn btn-lg btn-block btn-primary text-left"
        type="button"
        data-toggle="button"
        autocomplete="off"
    >
        Population Loader
    </button>
    </span>
</div>
<div id="population-loader-body" class="text-large collapse">
<ul class="list-group list-group-flush">
<li class="list-group-item list-group-item-success">
  Click a link to restart the viewer and seed the population with pre-evolved strains.
</li>
<li class="list-group-item list-group-item-info">
  Bundled genomes: load from files hosted alongside this page, with no dependency on AWS S3.
  All are wildtype genomes from series 16005 (thread 0) of endeavor 16, one per stint.
  <strong>Focal strain</strong> is the most abundant genome (abundance criterion), available for stints {focal_stints[0]} through {focal_stints[-1]}.
  <strong>Coevolved background strain</strong> is the most abundant genome of the highest phylogenetic root (abundance_highest_root criterion), available for stints {background_stints[0]} through {background_stints[-1]}.
  <strong>Multiculture</strong> seeds both strains from the same stint together.
</li>
<li class="list-group-item" style="max-height: 32em; overflow-y: auto;">
<table class="table table-sm table-striped">
<thead>
<tr><th>Stint</th><th>Focal Strain</th><th>Coevolved Background Strain</th><th>Both Strains</th></tr>
</thead>
<tbody>
{rows}
</tbody>
</table>
</li>
</ul>
</div>
</div>
"""


if __name__ == "__main__":
    with open(DIR + OUTPUT, "w") as file:
        file.write(make_html(sys.argv[1]))
