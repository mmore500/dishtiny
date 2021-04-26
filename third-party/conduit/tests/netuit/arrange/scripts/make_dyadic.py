#!/usr/bin/python3
"""
Generate dyadic graphs.

This script makes use of NetworkX to generate
dyadic graphs (pairs of nodes are connected).

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn

dims = [3, 4, 7, 10]

def make_filename(dim):
    return kn.pack({
        'name'  : 'dyadic',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:
    if dim % 2:
        procon = [(i, i + 1) for i in range(0, dim - 1, 2)] + [(dim - 1, dim - 1)]
    else:
        procon = [(i, i + 1) for i in range(0, dim, 2)]

    G_dyatic = nx.Graph(procon).to_directed()

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_dyatic):
            file.write(line + '\n')
