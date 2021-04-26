#!/usr/bin/python3
"""
Generate ring graphs.

This script makes use of NetworkX to generate
ring graphs (nodes are connected in a ring).

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn

dims = [3, 10, 15, 27, 56, 99]

def make_filename(dim):
    return kn.pack({
        'name'  : 'ring',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:
    ring = [edge for edge in zip(range(dim), range(1, dim))] + [(dim - 1, 0)]
    G_ring = nx.DiGraph(ring)

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_ring):
            file.write(line + '\n')
