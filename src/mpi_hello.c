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
//  printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);

  // do some ping pong

  char *inMsg, *outMsg = "ping_pong_ball";
  int msgLen = strlen(outMsg);
  MPI_Status status;

  if(rank == 0) {
    int dest = 1,
        src = 1;
    MPI_Send(&outMsg, msgLen, MPI_CHAR, dest, PING_PONG_BALL, MPI_COMM_WORLD);
    printf("Task %d: Sent %s to task %d\n", rank, outMsg, dest);
    MPI_Recv(&inMsg, msgLen, MPI_CHAR, src, PING_PONG_BALL, MPI_COMM_WORLD, &status);
    printf("Task %d: Received %s from task %d\n", rank, inMsg, src);
  } else if (rank == 1) {
    int dest = 0,
        src = 0;
    MPI_Recv(&inMsg, msgLen, MPI_CHAR, src, PING_PONG_BALL, MPI_COMM_WORLD, &status);
    printf("Task %d: Received %s from task %d\n", rank, inMsg, src);
    MPI_Send(&outMsg, msgLen, MPI_CHAR, dest, PING_PONG_BALL, MPI_COMM_WORLD);
    printf("Task %d: Sent %s to task %d\n", rank, outMsg, dest);
  }

  int count;
  MPI_Get_count(&status, MPI_CHAR, &count);
  printf("Task %d: Received %d char(s) from task %d with tag %d \n",
         rank, count, status.MPI_SOURCE, status.MPI_TAG);

  MPI_Finalize();
  return 0;
}
