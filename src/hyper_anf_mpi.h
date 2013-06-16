#ifndef _HYPER_ANF_MPI_H_
#define _HYPER_ANF_MPI_H_

#include <mpi.h>
#include "graph.h"

int mpi_diameter( node_t *partial_graph,
                  size_t partial_graph_cardinality,
                  int bits,
                  int max_iteration);


#endif // _HYPER_ANF_MPI_H_
