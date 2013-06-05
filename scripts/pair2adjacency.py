#!/usr/bin/env python

#################################################################################
#
# Script that accepts a graph incidence matrix on standard input and outputs the 
# adjacency list representation of the graph, in the format accepted and 
# described in parser.h
#
#################################################################################

import sys
from optparse import OptionParser

# Adjacencies dictionary: the key is the node ID, the value is
# a pair: the first position is a list of the outgoing neighbours, the second is
# a list of the incoming neighbours
adjacencies = dict()

def add_edge(u, v, directed = False):
    if u in adjacencies:
        adjacencies[u][0].append(v)
    else:
        adjacencies[u] = ([v],[])
    if v in adjacencies:
        adjacencies[v][1].append(u)
    else:
        adjacencies[v] = ([],[u])
    if not directed:
        if v in adjacencies:
            adjacencies[v][0].append(u)
        else:
            adjacencies[v] = ([u],[])
        if u in adjacencies:
            adjacencies[u][1].append(v)
        else:
            adjacencies[u] = ([],[u])
  

def convert(directed = False):
    for line in sys.stdin.readlines():
        edge = line.split()
        if len(edge) != 2:
            raise Exception("edge with a number of nodes different than 2 " + str(edge))
        add_edge(int(edge[0]), int(edge[1]), directed)
    for (node_id, (out_edges, in_edges)) in adjacencies.iteritems():
        sys.stdout.write("%d |" % (node_id))
        for v in out_edges:
            sys.stdout.write(" %d" % (v))
        sys.stdout.write(" |")
        for v in in_edges:
            sys.stdout.write(" %d" % (v))
        sys.stdout.write("\n")

def main():
    parser = OptionParser()
    parser.add_option("-d", "--directed", 
                      action="store_true",
                      dest="directed", default=False,
                      help="Create a directed graph")

    (options, args) = parser.parse_args()

    sys.stderr.write("Directed graph? " + str(options.directed) + "\n")

    convert(options.directed)


if __name__ == "__main__":
    main()

