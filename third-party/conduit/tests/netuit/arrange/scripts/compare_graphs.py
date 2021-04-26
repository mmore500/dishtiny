#!/usr/bin/python3
"""Check whether two graphs are isomorphic.

This script makes use of NetworkX to check whether
there exists an isomorphism between two graphs,
aka if two graphs are equivalent.

This tool takes in two adjacency list files (.adj) and
prints whether the graph they represent are isomorphic,
as well as the reason in case they aren't.
"""

import networkx as nx
import sys

(exe, filename1, filename2) = sys.argv

if len(open(filename1).readlines()) == 0:
    print(filename1, "is empty")

if len(open(filename2).readlines()) == 0:
    print(filename2, "is empty")

G1 = nx.read_adjlist(filename1)
G2 = nx.read_adjlist(filename2)

if (len(G1.nodes()) != len(G2.nodes())):
    print("unequal node counts")
    print("num nodes G1", len(G1.nodes()))
    print("num nodes G2", len(G2.nodes()))

    print("G1 filename:", filename1)
    print("G2 filename:", filename2)

if (len(G1.edges()) != len(G2.edges())):
    print("unequal edge counts")
    print("num edges G1", len(G1.edges()))
    print("num edges G2", len(G2.edges()))

if (nx.is_isomorphic(G1, G2)):
    print("isomorphic")
else:
    print("not isomorphic")
