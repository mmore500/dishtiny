#!/usr/bin/python3
"""
Generate procon graphs.

This script makes use of NetworkX to generate
procon graphs (nodes are paired up; one outputs
to another).

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn

dims = [3, 10, 12, 15, 27, 56, 99]

def make_filename(dim):
    return kn.pack({
        'name'  : 'procon',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })
# taken from https://stackoverflow.com/a/312464
def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

for dim in dims:
    x = list(range(0, dim))
    if len(x) % 2:
        x.append(x[-1])
    procon = list(chunks(x, 2))
    G_procon = nx.DiGraph(procon)

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_procon):
            file.write(line + '\n')
