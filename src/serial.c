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
  hll_counter_t *counters;
  hll_counter_t *counters_prev;

  int rc = parse_graph_file("graph.dat", &nodes, &n);
  if (rc < 0) {
    fprintf(stderr, "ERROR: could not parse graph file correctly.\n");
    exit(EXIT_FAILURE);
  } else if (rc != n) {
    fprintf(stderr, "ERROR: Some nodes were not parsed correctly, exiting.\n");
    exit(EXIT_FAILURE);
  }

  printf("Loaded graph with %d nodes\n", n);

  counters = malloc(n * sizeof(hll_counter_t));
  counters_prev = malloc(n * sizeof(hll_counter_t));

  for (int i=0; i<n; ++i) {
    hll_cnt_init(&counters[i], COUNTER_BITS);
    hll_cnt_init(&counters_prev[i], COUNTER_BITS);
    hll_cnt_add(nodes[i].id, &counters[i]);
  }

  // the number of nodes that changed since last iteration
  int changed = n;

  int k = 0; // iteration number

  while (changed != 0 && k < MAX_ITER) {
    changed = 0;
    for (int i=0; i<n; ++i) {
      for (int j=0; j<nodes[i].num_out; ++j) {
        hll_cnt_union_i(&counters[i], &counters[j]);
        if (!hll_cnt_equals(&counters[i], &counters_prev[j])) {
          ++changed;
        }
      }
    }
  }

  // free resources
  for(int i=0; i<n; ++i) {
    node_free(&nodes[i]);
    hll_cnt_free(&counters[i]);
    hll_cnt_free(&counters_prev[i]);
  }
  free(nodes);
  free(counters);
  free(counters_prev);

  return 0;
}
