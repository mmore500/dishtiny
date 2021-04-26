#!/usr/bin/python3
"""
Generate complete graphs.

This script makes use of NetworkX to generate
complete graphs (each node connected to all others).

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn

dims = [3, 10, 15, 27, 56, 99]

def make_filename(dim):
    return kn.pack({
        'name'  : 'complete',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:
    G_complete = nx.complete_graph(dim, nx.DiGraph())

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_complete):
            file.write(line + '\n')
