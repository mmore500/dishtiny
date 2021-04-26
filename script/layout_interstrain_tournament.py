#!/usr/bin/env python3
"""Generates tournament parings between series competitors.

Each series participates in degree competitions.
Competition pairings are printed to stdout.

Usage:
    ./layout_interstrain_tournament.py [seed] [degree] [series]...
"""

import sys

import networkx as nx

try:
    seed, degree, *series = map(int, sys.argv[1:])
except:
    print( __doc__ )
    sys.exit(1)

n = len(series)
G = nx.generators.random_graphs.random_regular_graph(degree, n, seed)

G = nx.relabel_nodes( G, lambda i: series[i] )

for a, b in G.edges:
    print(a, b)
