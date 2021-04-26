#!/usr/bin/python3
"""
Generate small world grid graphs.

This script makes use of NetworkX to generate
a toroidal grid with all nodes connected to one other random node.

This tool creates adjacency list files (.adj)
whose filename represent the characteristics
of the graph created.
"""

import networkx as nx
from keyname import keyname as kn
import random

dims = [3, 10, 15, 27, 94]

def make_filename(dim):
    return kn.pack({
        'name'  : 'small_world_grid',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj',
    })

for dim in dims:

    random.seed(1)

    G = nx.grid_graph(
        (dim, ), periodic=True
    ).to_directed()

    nodes = list(G.nodes())
    random.shuffle( nodes )
    for a, b in zip(nodes[0::2], nodes[1::2]):
        G.add_edge(a, b)
        G.add_edge(b, a)

    # relabel nodes to numeric indices from previous coordinate labels
    H = nx.relabel_nodes(
        G,
        { label : idx for idx, label in enumerate(G.nodes()) },
    )

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(H):
            file.write(line + '\n')
