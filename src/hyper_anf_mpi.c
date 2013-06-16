#include "hyper_anf_mpi.h"
#include "check_ptr.h"
#include <mpi.h>
#include <limits.h>

void init_neighbourhoods( mpi_neighbourhood_t **neighbourhoods,
                          int n, int bits,
                          node_t *partial_graph) {
  *neighbourhoods =
        malloc(n * sizeof(mpi_neighbourhood_t));
  check_ptr(neighbourhoods);
  for (int i=0; i<n; ++i) {
    mpi_neighbourhood_init(
          &(*neighbourhoods[i]), partial_graph[i].num_out, bits);
  }
}

void init_counters( hll_counter_t **counters,
                    hll_counter_t **counters_prev,
                    int n, int bits,
                    node_t *partial_graph)
{
  *counters = malloc(n * sizeof(hll_counter_t));
  check_ptr(counters);
  *counters_prev = malloc(n * sizeof(hll_counter_t));
  check_ptr(counters_prev);
  for (int i = 0; i < n; ++i) {
    hll_cnt_init(&(*counters[i]), bits);
    hll_cnt_init(&(*counters_prev[i]), bits);
    // add the node itself to each counter
    hll_cnt_add(partial_graph[i].id, &(*counters[i]));
    hll_cnt_add(partial_graph[i].id, &(*counters_prev[i]));
  }
}

/*
 * We need several things for each node:
 *
 *  - the list of processors responsible for their out neighbours
 *  - the list of processors responsible for their in neighbourhoods
 *  - the list of hll_counters of their outgoing neighbours
 */
int mpi_diameter( node_t *partial_graph,
                  size_t partial_graph_cardinality,
                  int bits,
                  int max_iteration)
{
  // - for each node in partial graph
  //   - expect to receive counters from neighbours
  //   - send counter to neighbours
  // - for each node in partial graph
  //   - update counters
  //   - estimate sizes
  // - use mpi_reduce to sum all the sizes and get N(t)
  // - use mpi_reduce to compute the number of changed nodes.
  // - if no nodes changed or we are at max_iteration, stop.
  // ---------------------------------------------------------
  // Allocate memory to receive counters from out neighbours, for each node
  int num_processors;
  MPI_Comm_size(MPI_COMM_WORLD,&num_processors);

  mpi_neighbourhood_t *neighbourhoods;
  init_neighbourhoods( &neighbourhoods,
                       partial_graph_cardinality, bits, partial_graph);

  // Allocate memory for the current and previous version of the counter
  // for each node
  hll_counter_t *counters, *counters_prev;
  init_counters( &counters, &counters_prev,
                 partial_graph_cardinality, bits, partial_graph);

  int num_changed = INT_MAX;
  int iteration = 0;
  // main loop
  while (num_changed != 0 && iteration < max_iteration) {

    ++iteration;
  }

  // free the memory
  for (int i = 0; i < partial_graph_cardinality; ++i) {
    hll_cnt_free(&counters[i]);
    hll_cnt_free(&counters_prev[i]);
    mpi_neighbourhood_free(&neighbourhoods[i]);
  }
  free(counters);
  free(counters_prev);
  free(neighbourhoods);
  return iteration -1;
}


inline int get_processor_rank (node_id_t node, int num_processors) {
  return node % num_processors;
}

void mpi_neighbourhood_init (mpi_neighbourhood_t *neigh, int n, int bits) {
  neigh->dimension = n;
  neigh->counters = malloc(n*sizeof(hll_counter_t));
  check_ptr(neigh->counters);
  for (int i = 0; i < n; ++i) {
    hll_cnt_init(&neigh->counters[i], bits);
  }
}

void mpi_neighbourhood_free (mpi_neighbourhood_t *neigh) {
  for (int i = 0; i < neigh->dimension; ++i) {
    hll_cnt_free(&neigh->counters[i]);
  }
  free(neigh->counters);
}
