#!/usr/bin/env python

# This script reads the adjacency representation of a graph from standard
# input and creates a file for each processor. Each file will contain
# all the nodes of the graph that are under the responsibility of a
# specific processor. A node is assigned to processor `i` if `i mod n == 0`,
# where `n` is the number of processors.
#
# Naming of output files
# ----------------------
#
# The output files of this program are named in the following way
#
#     basename-i.adj
#
# where basename can be specified on the command line (default is `graph`)
# and `i` is the ID of the processor that will load that file.

import argparse

def main():
    argParser = argparse.ArgumentParser(description='Splits a graph file.')
    argParser.add_argument('-b', '--basename', dest='basename',
                           default='graph', metavar='NAME',
                           help='The base name of the output files.')
    argParser.add_argument('-n', metavar='N', dest='num_processors',
                           default=1, type=int,
                           help='The number of processors, defaults to one.')
    args = argParser.parse_args()


if __name__ == '__main__':
    main()
