#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "hll_counter.h"
#include "parser.h"

#define COUNTER_BITS 2
#define MAX_ITER 10


int main(int argc, char **argv) {

  int n = 0;
  node_t *nodes;

  int rc = parse_graph_file("graph.dat", &nodes, &n);
  if (rc < 0) {
    fprintf(stderr, "ERROR: could not parse graph file correctly.\n");
    exit(EXIT_FAILURE);
  }

  printf("Loaded graph with %d nodes\n", n);

  return 0;
}
