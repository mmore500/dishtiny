import toml
from bs4 import BeautifulSoup as bs

filename = 'keys.toml'
output = 'output.html'
data = {}

with open(filename, 'r') as file:
    data = toml.load(file)

html = '<div id="view_key-body" class="text-large collapse">\n'

for key_name, key_body in data.items():
    html += '<div id="' + key_name + '-key">'
    html += '<ul class="list-group list-group-flush">'

    if 'header' in key_body:
        header = key_body['header']

        html += '<li class="list-group-item list-group-item-dark">'

        # single-line header
        if isinstance(header, str):
            html += header
        # multi-line header
        if isinstance(header, list):
            for i in key_body['header']:
                html += i + '\n'
        
        html += '</li>'
    
    if 'items' in key_body:
        for i in key_body['items']:
            html += '<li class="list-group-item">'
            html += i + '\n'
            html += '</li>'
    
#    print(key_body.keys())

    html += '</ul>'
    html += '</div>'

# make html pretty
pretty_html = bs(html).prettify()  
print(pretty_html)

with open(output, 'w') as out:
    out.write(pretty_html)