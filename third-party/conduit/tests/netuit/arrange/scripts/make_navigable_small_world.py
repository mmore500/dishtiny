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
        'name'  : 'navigable_small_world',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:

    G = nx.generators.geometric.navigable_small_world_graph(
        dim,
        seed=1,
        dim=1,
    )

    # relabel nodes to numeric indices from previous coordinate labels
    H = nx.relabel_nodes(
        G,
        { label : idx for idx, label in enumerate(G.nodes()) },
    )

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(H):
            file.write(line + '\n')
