/**
 * @file parser.h
 * @brief parser definitions.
 *
 * This header contains functions that parse files and strings to build graph
 * nodes.
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include "graph.h"

/**
 * @brief Builds a heap-allocated node from a string description
 *
 * This function takes a string that contains the description of a node, parses
 * it, allocates a new node_t struct and populates it with the correct values.
 *
 * **Attention**: it the description os not well formed, the function
 * returns NULL.
 *
 * Description format
 * ------------------
 *
 * The format used in node descriptions _must_ follow this specification
 *
 *     id | <forward star> | <reverse star>
 *
 * that is: the id of the node, a pipe symbol, a list of identifiers for the
 * outgoing neighbours, a pipe symbol, a list of identifiers for the incomping
 * neighbours.
 *
 * ### Example ###
 *
 * For example we have a node, `0`, whose outgoing neighbours are `1`, `3` and
 * `5` and whose incoming neighbour is `2`. The string to describe this node is
 * the following
 *
 *     0 | 1 3 5 | 2
 *
 * A node with no neighbours is represented in this way (suppose its id is `2`)
 *
 *     2 | |
 *
 * That is, the pipes are **mandatory**
 *
 * @param descr the description of the node
 * @return a heap allocated node struct, NULL if there are errors
 */
node_t * parse_node_descr(char * descr);

/*
 * Given a string, counts the number of distinct numbers in it.
 * Returns `-1` on error
 */
int count_numbers(char * str);

/*
 * Populates the `adj` array with `n` integers extracted from the string
 * `adj_str`.
 * Returns -1 on error, the number of inserted elements otherwise
 */
int populate_adjacency(char * adj_str, node_id_t * adj, int n);

#endif // _PARSER_H_
