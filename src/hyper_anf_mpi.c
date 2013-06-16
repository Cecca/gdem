#include "hyper_anf_mpi.h"
#include "check_ptr.h"
#include <mpi.h>


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
  // Allocate memory to receive counters from neighbours, for each node

  for (int i=0; i<partial_graph_cardinality; ++i) {

  }
}


void mpi_neighbourhood_init (mpi_neighbourhood *neigh, int n) {
  neigh->dimension = n;
  neigh->counters = malloc(n*sizeof(hll_counter_t));
  check_ptr(neigh->counters);
}

void mpi_neighbourhood_free (mpi_neighbourhood *neigh) {
  free(neigh->counters);
}
