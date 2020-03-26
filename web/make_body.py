import toml
import jinja2
from markdown2 import Markdown

from markup import dishMark

INPUT = 'keys.toml'
OUTPUT = 'output.html'
TEMPLATE = "template.html.jinja"

# load our TOML file
with open(INPUT, 'r') as file:
    data = toml.load(file)

# process and generate badge
def make_badge(str, color):
    # it's a badge!

    # determine brightness to decide background color
    # based on https://stackoverflow.com/a/3943023
    (r, g, b) = (color[1:3], color[3:5], color[5:7])
    colors = [int(x, 16) for x in (r, g, b)]

    L = 0.299 * colors[0] + 0.587 * colors[1] + 0.114 * colors[2]

    if L > 186:
        type = "badge badge-light"
    else:
        type = "badge badge-dark"

    # for color string
    color_str = "background-color: " + color + ";"

    # form badge!
    badge = '<span class="' + type + '" style="' + color_str + '">' + str + '</span>'
    
    return badge

# instanciate Markdown 
markdown = Markdown()

# find template, adapted from https://stackoverflow.com/a/38642558 
template_loader = jinja2.FileSystemLoader(searchpath="./")
template_env = jinja2.Environment(loader=template_loader)
# load template
template = template_env.get_template(TEMPLATE)
# output templated html
# we pass it our data, our markdown instance, and eval() and make_badge(), 
# since they are all used by it
outputText = template.render(data=data, markdown=markdown, eval=eval, make_badge=make_badge)

# output to file
with open(OUTPUT, 'w') as out:
    out.write(outputText)