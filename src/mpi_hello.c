#include "mpi.h"
#include <stdio.h>
#include <string.h>

#define PING_PONG_BALL 0

int main(int argc, char** argv) {
  int  numtasks, rank, len, rc;
  char hostname[MPI_MAX_PROCESSOR_NAME];

  rc = MPI_Init(&argc,&argv);
  if (rc != MPI_SUCCESS) {
    printf ("Error starting MPI program. Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Get_processor_name(hostname, &len);

  // each process will receive a counter from its left nearest neighbour
  // and send its one to the nearest right neighbour.

  int reg = rank;

#define NUM_NEIGHS 1

  int dests[] = {(rank+1) % numtasks};
  int srcs[] = {(rank-1) % numtasks};

  int results[NUM_NEIGHS];

  // the first half is for receivs, the second one is for sends
  MPI_Request requests[2*NUM_NEIGHS];

  // statuses of receive operations
  MPI_Status stats[2*NUM_NEIGHS];

  for(int i=0; i<NUM_NEIGHS; ++i) {
    // receive non blocking from neighbours
    MPI_Irecv (&results[i], 1, MPI_INT, srcs[i], 0, MPI_COMM_WORLD, &requests[i]);
  }

  for(int i=0; i<NUM_NEIGHS; ++i) {
    // send non blocking to neighbours
    MPI_Isend (&reg, 1, MPI_INT, dests[i], 0, MPI_COMM_WORLD, &requests[i+NUM_NEIGHS]);
  }

  // wait for communication to complete
  MPI_Waitall(NUM_NEIGHS*2, requests, stats);

  for (int i=0; i<NUM_NEIGHS; ++i) {
    printf("Task %d received from neighbours: %d\n", rank, results[i]);
  }

  MPI_Finalize();
  return 0;
}
