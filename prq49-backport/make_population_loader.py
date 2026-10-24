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
        f'<a href="{href}" role="button" class="btn btn-light btn-sm m-1">'
        f"{label}</a>"
    )


def make_section(title, buttons):
    return f"""<li class="list-group-item list-group-item-secondary font-weight-bold">
{title}
</li>
<li class="list-group-item">
<div class="d-flex flex-wrap">
{chr(10).join(buttons)}
</div>
</li>"""


def make_html(genomes_dir):
    focal_stints = available_stints(FOCAL, genomes_dir)
    background_stints = available_stints(BACKGROUND, genomes_dir)
    both_stints = sorted(set(focal_stints) & set(background_stints))
    sections = "\n".join(
        [
            make_section(
                "Monoculture: focal strain, series 16005 (case study)",
                [
                    button(
                        link("monoculture", FOCAL.format(stint=stint)), f"stint {stint}"
                    )
                    for stint in focal_stints
                ],
            ),
            make_section(
                "Monoculture: background strain, series 16005",
                [
                    button(
                        link("monoculture", BACKGROUND.format(stint=stint)),
                        f"stint {stint}",
                    )
                    for stint in background_stints
                ],
            ),
            make_section(
                "Multiculture: focal and background strains, series 16005",
                [
                    button(
                        link(
                            "multiculture",
                            FOCAL.format(stint=stint),
                            BACKGROUND.format(stint=stint),
                        ),
                        f"stint {stint}",
                    )
                    for stint in both_stints
                ],
            ),
        ]
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
{sections}
</ul>
</div>
</div>
"""


if __name__ == "__main__":
    with open(DIR + OUTPUT, "w") as file:
        file.write(make_html(sys.argv[1]))
