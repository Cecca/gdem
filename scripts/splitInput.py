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
import sys

def open_files(args):
    files = []
    for i in range(args.num_processors):
        filename = args.basename + '-' + str(i) + '.adj'
        print 'Opening file', filename
        f = open(filename, mode='w')
        files.append(f)
    return files

def close_files(files):
    for f in files:
        print 'Closing file', f.name
        f.close()

def get_processor_id(node_id, num_processors):
    return int(node_id) % num_processors

def split_graph(args):
    files = open_files(args)
    num_processors = args.num_processors
    for line in sys.stdin:
        node_id = line.split('|')[0]
        processor_id = get_processor_id(node_id, num_processors)
        files[processor_id].write(line)

    close_files(files)

def main():
    argParser = argparse.ArgumentParser(description='Splits a graph file.')
    argParser.add_argument('-b', '--basename', dest='basename',
                           default='graph', metavar='NAME',
                           help='The base name of the output files.')
    argParser.add_argument('-n', metavar='N', dest='num_processors',
                           default=1, type=int,
                           help='The number of processors, defaults to one.')
    args = argParser.parse_args()
    split_graph(args)


if __name__ == '__main__':
    main()
