import colorsys
import jinja2
from markdown2 import Markdown
import re
import toml

INPUT = 'keys.toml'
OUTPUT = 'index.html'
TEMPLATE = "template.html.jinja"
DIR = "web/"

# load our TOML file
with open(DIR + INPUT, 'r') as file:
    data = toml.load(file)

# process and generate badge
def make_badge(str, color):
    # it's a badge!

    # determine brightness to decide background color
    # based on https://stackoverflow.com/a/3943023
    (r, g, b) = (color[1:3], color[3:5], color[5:7])
    colors = [int(x, 16) for x in (r, g, b)]

    L = 0.299 * colors[0] + 0.587 * colors[1] + 0.114 * colors[2]

    if L > 140:
        type = "badge badge-light"
        # bootstrap's badge-light text is dark gray, so use black
        text_str = " color: #000000;"
    else:
        type = "badge badge-dark"
        text_str = ""

    # for color string
    color_str = "background-color: " + color + ";" + text_str

    # form badge!
    badge = '<span class="' + type + '" style="' + color_str + '">' + str + '</span>'

    return badge

# a colorbar with a label centered on each color stop
# colors[i-1] -> colors[i] is a hard threshold if i is in hard,
# and may be labeled by threshold_labels[i]
def make_colorbar(colors, labels, text="", hard=(), threshold_labels=None):
    n = len(colors)
    stops = []
    captions = []
    for i, (color, label) in enumerate(zip(colors, labels)):
        lo = 100 * (i - 0.5 if i in hard else i) / (n - 1)
        hi = 100 * (i + 0.5 if i + 1 in hard else i) / (n - 1)
        stops.append(f"{color} {lo}%")
        if hi != lo:
            stops.append(f"{color} {hi}%")
        # labels at the edges are anchored there, others centered on the band
        if lo <= 0:
            center, shift = 0, "0"
        elif hi >= 100:
            center, shift = 100, "-100%"
        else:
            center, shift = (lo + hi) / 2, "-50%"
        captions.append(
            '<span style="position: absolute; white-space: nowrap; '
            f'left: {center}%; transform: translateX({shift});">{label}</span>'
        )
    for i, label in (threshold_labels or {}).items():
        captions.append(
            '<span style="position: absolute; white-space: nowrap; '
            f'left: {100 * (i - 0.5) / (n - 1)}%; transform: translateX(-50%);">'
            f"{label}</span>"
        )
    gradient = f"linear-gradient(to right, {', '.join(stops)})"
    bar = (
        '<div style="height: 1.25em; border: 1px solid black; '
        f'background: {gradient};"></div>'
    )
    return (
        '<div class="my-1">'
        + (markdown.convert(text) if text else "")
        + bar
        + '<div class="small" style="position: relative; height: 1.5em;">'
        + "".join(captions)
        + "</div></div>"
    )


# number of kin group levels, as compiled
with open("include/dish2/spec/_NLEV.hpp", "r") as file:
    NLEV = int(re.search(r"define DISH2_NLEV (\d+)", file.read()).group(1))


# categorical legend for each requestable replication level, up to NLEV
def make_rep_lev_legend():
    lines = []
    for lev in range(NLEV):
        r, g, b = colorsys.hsv_to_rgb((lev * 90 % 360) / 360, 1.0, 1.0)
        color = "#{:02X}{:02X}{:02X}".format(*(round(255 * c) for c in (r, g, b)))
        lines.append(
            make_badge(f"Level {lev}", color)
            + f" indicates a request at <strong>group level {lev}</strong>."
        )
    return "<br>".join(lines)


# instantiate Markdown
markdown = Markdown()

# find template, adapted from https://stackoverflow.com/a/38642558
template_loader = jinja2.FileSystemLoader(searchpath=DIR)
template_env = jinja2.Environment(loader=template_loader)
# load template
template = template_env.get_template(TEMPLATE)
# output templated html
# we pass it our data, our markdown instance, and eval() and make_badge(),
# since they are all used by it
outputText = template.render(
    data=data,
    markdown=markdown,
    eval=eval,
    make_badge=make_badge,
    make_colorbar=make_colorbar,
    make_rep_lev_legend=make_rep_lev_legend,
)

# output to file
with open(DIR + OUTPUT, 'wb+') as out:
    out.write( outputText.encode('utf8') )
