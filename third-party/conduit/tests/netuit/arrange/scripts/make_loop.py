#!/usr/bin/python3
"""
Generate loop graphs.

This script makes use of NetworkX to generate
loop graphs (nodes are connected to themselves).

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn

dims = [3, 10, 15, 27, 56, 99]

def make_filename(dim):
    return kn.pack({
        'name'  : 'loop',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:
    loop = [(i, i) for i in range(dim)]
    G_loop = nx.DiGraph(loop)

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_loop):
            file.write(line + '\n')
