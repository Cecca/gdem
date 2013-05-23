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
  hll_cardinality_t *neighbourhood_function;

  neighbourhood_function = malloc(sizeof(hll_cardinality_t) * MAX_ITER);

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
    for (int i=0; i<n; ++i) { // for each counter
      for (int j=0; j<nodes[i].num_out; ++j) { // update it
        hll_cnt_union_i(&counters[i], &counters_prev[j]);
        if (!hll_cnt_equals(&counters[i], &counters_prev[j])) {
          ++changed;
        }
        hll_cnt_copy_to(&counters[i], &counters_prev[i]);
      }
    }
    // and now update N(k)
    hll_cardinality_t sum = 0;
    for (int h=0; h<n; ++h) {
      sum += hll_cnt_size(&counters[h]);
    }
    neighbourhood_function[k] = sum;
    ++k;
  }

  printf("Number of iterations: %d\n", k);
  printf("Neighbourhood function is:\n");
  for (int i=0; i<k; ++i) {
    printf("    N(%d) = %d\n", i, neighbourhood_function[i]);
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
  free(neighbourhood_function);

  return 0;
}
