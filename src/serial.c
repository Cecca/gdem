#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "hll_counter.h"
#include "parser.h"
#include "null.h"
#include "debug.h"
#include "check_ptr.h"
#include "hyper_anf_serial.h"

#define COUNTER_BITS 3
#define MAX_ITER 150

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Please provide a graph file name\n");
    exit(EXIT_FAILURE);
  }

  int n = 0;
  node_t *nodes;

  int rc = parse_graph_file(argv[1], &nodes, &n);
  if (rc < 0) {
    fprintf(stderr, "ERROR: could not parse graph file correctly.\n");
    exit(EXIT_FAILURE);
  } else if (rc != n) {
    fprintf(stderr, "ERROR: Some nodes were not parsed correctly, exiting.\n");
    exit(EXIT_FAILURE);
  }

  printf("Loaded graph with %d nodes\n", n);

  int diameter = effective_diameter(nodes, n, 0, COUNTER_BITS, MAX_ITER);

  printf("Diameter is %d\n", diameter);

  // free resources
  for(int i=0; i<n; ++i) {
    node_free(&nodes[i]);
  }
  free(nodes);

  return 0;
}
