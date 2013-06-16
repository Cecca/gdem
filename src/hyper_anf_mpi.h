#ifndef _HYPER_ANF_MPI_H_
#define _HYPER_ANF_MPI_H_

#include <mpi.h>
#include "graph.h"
#include "hll_counter.h"

/**
 * @brief Compute the diameter of the graph, given the partial view.
 *
 * **Attention**: assumes that MPI_inithas already been called.
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

/**
 * Defines an association between an array of counters and its dimension.
 * Useful to keep track of the neighbours' counters.
 */
typedef struct _mpi_neighbourhood {
  size_t dimension;
  hll_counter_t *counters;
} mpi_neighbourhood;

void mpi_neighbourhood_init(mpi_neighbourhood *neigh, int n, int bits);

void mpi_neighbourhood_free(mpi_neighbourhood *neigh);

#endif // _HYPER_ANF_MPI_H_
