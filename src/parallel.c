#include "hyper_anf_mpi.h"
#include "graph.h"
#include <stdio.h>

int main(int argc, char **argv) {

  int rc = MPI_Init(&argc,&argv);
  if (rc != MPI_SUCCESS) {
    printf ("Error starting MPI program. Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char filename[] = "graph/graph";
  node_t * nodes = 0;
  int n;
  load_partial_graph(rank, filename, &nodes, &n);
  printf("(Process %d) loaded %d nodes\n", rank, n);


  context_t context;
  init_context(&context, nodes, n, 1, 100);

  mpi_diameter(&context);

  if(nodes != 0) {
    free(nodes);
  }
  free_context(&context);

  printf("Done!!\n");

  MPI_Finalize();

  return 0;
}
