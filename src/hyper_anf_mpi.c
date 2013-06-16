#include "hyper_anf_mpi.h"
#include "check_ptr.h"
#include <mpi.h>
#include <limits.h>

void init_context ( context_t *context,
                    node_t *partial_graph,
                    size_t partial_graph_cardinality,
                    int bits,
                    int max_iteration)
{
  context->num_nodes = partial_graph_cardinality;
  context->iteration = 0;
  context->num_changed = INT_MAX;
  context->max_iteration = max_iteration;
  // get the number of processors
  MPI_Comm_size(MPI_COMM_WORLD, & context->num_processors);
  // get the rank of this processor
  MPI_Comm_rank(MPI_COMM_WORLD, & context->rank);

  context->nodes = partial_graph;

  // init neighbourhoods
  context->neighbourhoods =
        malloc(context->num_nodes * sizeof(mpi_neighbourhood_t));
  check_ptr(context->neighbourhoods);
  for (int i=0; i<context->num_nodes; ++i) {
    mpi_neighbourhood_init(
          &context->neighbourhoods[i], partial_graph[i].num_out, bits);
  }

  // init counters
  context->counters = malloc(context->num_nodes * sizeof(hll_counter_t));
  check_ptr(context->counters);
  context->counters_prev = malloc(context->num_nodes * sizeof(hll_counter_t));
  check_ptr(context->counters_prev);
  for (int i = 0; i < context->num_nodes; ++i) {
    hll_cnt_init(&context->counters[i], bits);
    hll_cnt_init(&context->counters_prev[i], bits);
    // add the node itself to each counter
    hll_cnt_add(partial_graph[i].id, &context->counters[i]);
    hll_cnt_add(partial_graph[i].id, &context->counters_prev[i]);
  }
}

void free_context (context_t *context) {
  for (int i = 0; i < context->num_nodes; ++i) {
    hll_cnt_free(& context->counters[i]);
    hll_cnt_free(& context->counters_prev[i]);
    mpi_neighbourhood_free(& context->neighbourhoods[i]);
  }
  free(context->counters);
  free(context->counters_prev);
  free(context->neighbourhoods);
}

int mpi_diameter( context_t * context )
{
  while ( context->num_changed != 0 &&
          context->iteration < context->max_iteration)
  {
    // reset the number of changed nodes
    context->num_changed = 0;
    // compute the neighbourhood function before updating counters.

    // - for each node in partial graph
    //   - expect to receive counters from neighbours
    //   - send counter to neighbours
    // - for each node in partial graph
    //   - update counters
    //   - estimate sizes
    // - use mpi_reduce to sum all the sizes and get N(t)
    // - use mpi_reduce to compute the number of changed nodes.
    // - if no nodes changed or we are at max_iteration, stop.

    ++context->iteration;
  }


  return context->iteration - 1;
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
