#include "hyper_anf_mpi.h"
#include "graph.h"
#include <stdio.h>

int main(int argc, char **argv) {

  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Rank %d\n", rank);

  char filename[] = "graph/graph";

  node_t * nodes = 0;
  int n;

  load_partial_graph(rank, filename, &nodes, &n);

  printf("(Process %d) loaded %d nodes\n", rank, n);

  if(nodes != 0) {
    free(nodes);
  }

  MPI_Finalize();

  return 0;
}
