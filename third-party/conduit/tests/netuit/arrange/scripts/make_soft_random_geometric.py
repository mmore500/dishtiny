#!/usr/bin/python3
"""
Generate navigable small world graphs.

This script makes use of NetworkX to generate
navigable small world graphs.

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn

dims = [3, 10, 15, 27, ]

def make_filename(dim):
    return kn.pack({
        'name'  : 'soft_random_geometric',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:

    G = nx.generators.geometric.soft_random_geometric_graph(
        dim,
        0.1,
        dim=2,
        seed=1,
    )

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G):
            file.write(line + '\n')
