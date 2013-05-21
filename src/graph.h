#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <inttypes.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
//    Data structures
// ----------------------------------------------------------------------------

typedef uint32_t node_id_t;

typedef struct _node {
  node_id_t id;     /**< Unique ID of the node */
  size_t num_out;   /**< Number of outgoing neighbours */
  size_t num_in;    /**< Number of outgoing neighbours */
  node_id_t *out;   /**< Array of outgoing neighbours */
  node_id_t *in;    /**< Array of incoming neighbours */
} node_t;


// ----------------------------------------------------------------------------
//    Memory management routines
// ----------------------------------------------------------------------------

/**
 * @brief Dynamically allocates a new node.
 * @param id the id of the node
 * @param out_n the cardinality of the forward star of the node
 * @param in_n the cardinality of the reverse star
 * @return a pointer to the heap-allocated node.
 */
node_t * node_new(node_id_t id, size_t out_n, size_t in_n);

/**
 * @brief Deletes a heap-allocated node.
 * @param node the node to delete.
 */
void node_delete(node_t * node);

#endif // _GRAPH_H_
