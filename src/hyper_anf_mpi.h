#ifndef _HYPER_ANF_MPI_H_
#define _HYPER_ANF_MPI_H_

#include <mpi.h>
#include "graph.h"
#include "hll_counter.h"

/**
 * Defines an association between an array of counters and its dimension.
 * Useful to keep track of the neighbours' counters.
 */
struct mpi_neighbourhood {
  size_t dimension;
  hll_counter_t *counters;
};
typedef struct mpi_neighbourhood mpi_neighbourhood_t;

/**
 * Struct that carries all the information needed by the diameter computation.
 */
struct context {
  size_t num_nodes; /**< The number of nodes under the responsibility of
                      *  this processor.
                      */
  node_t *nodes; /**< The nodes under the responsibility of this processor */
  hll_counter_t *counters; /**< The array of current counters, one for each
                             *  node. The size is `num_nodes`
                             */
  hll_counter_t *counters_prev; /**< The array of previous counters, one for
                                  *  each node. The size is `num_nodes`
                                  */
  mpi_neighbourhood_t *neighbourhoods; /**< The array of out neighbourhoods.
                                       *  Size is `num_nodes`
                                       */
  int iteration; /**< The current algorithm iteration */
  int num_changed; /**< The number of nodes changed since the last iteration */
  int num_processors; /**< The number of processors */
  int max_iteration; /**< The maximum iteration number we are allowed to do */
};
typedef struct context context_t;

void init_context(context_t * context,
                  node_t *partial_graph,
                  size_t partial_graph_cardinality,
                  int bits,
                  int max_iteration);

void free_context(context_t * context);

/**
 * @brief Compute the diameter of the graph, given the partial view.
 *
 * **Attention**: assumes that MPI_init has already been called.
 *
 * TODO: add a new mpi datatype for the message: the message is composed
 * of two parts:
 *
 *  1. the id of the node
 *  2. the counter associated to that node
 *
 * @param partial_graph
 * @param partial_graph_cardinality
 * @param bits
 * @param max_iteration
 * @return
 */
int mpi_diameter( node_t *partial_graph,
                  size_t partial_graph_cardinality,
                  int bits,
                  int max_iteration);

/**
 * @brief Returns the global rank of the processor responsible for the node.
 * @param node
 * @param num_processors
 * @return
 */
int get_processor_rank( node_id_t node, int num_processors );

void mpi_neighbourhood_init(mpi_neighbourhood_t *neigh, int n, int bits);

void mpi_neighbourhood_free(mpi_neighbourhood_t *neigh);

#endif // _HYPER_ANF_MPI_H_
