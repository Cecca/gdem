#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "hll_counter.h"
#include "parser.h"
#include "null.h"
#include "debug.h"

#define COUNTER_BITS 2
#define MAX_ITER 10

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Please provide a graph file name\n");
    exit(EXIT_FAILURE);
  }

  int n = 0;
  node_t *nodes;
  hll_counter_t *counters;
  hll_counter_t *counters_prev;
  hll_cardinality_t *neighbourhood_function;

  neighbourhood_function = malloc(sizeof(hll_cardinality_t) * MAX_ITER);

  int rc = parse_graph_file(argv[1], &nodes, &n);
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
    assert(counters[i].registers != NULL);
    assert(counters_prev[i].registers != NULL);
  }

  // the number of nodes that changed since last iteration
  int changed = n;

  int k = 0; // iteration number

  while (changed != 0 && k < MAX_ITER) {
    changed = 0;
    printd("Iteration %d\n", k);
    for (int i=0; i<n; ++i) { // for each counter
      printd("  Counter %d\n", i);
      hll_counter_t *current_node_counter = & (counters[i]);
      hll_counter_t *current_node_counter_prev = & (counters_prev[i]);
      for (int j=0; j<nodes[i].num_out; ++j) { // update it
        node_id_t neighbour = nodes[i].out[j];
        printd("    Neighbour %d: %d\n", j, neighbour);
        hll_counter_t *neighbour_counter = & (counters_prev[neighbour]);
        hll_cnt_union_i(current_node_counter, neighbour_counter);
        if (!hll_cnt_equals(current_node_counter, current_node_counter_prev)) {
          ++changed;
        }
        hll_cnt_copy_to(current_node_counter, current_node_counter_prev);
      }
    }
    // and now update N(k)
    hll_cardinality_t sum = 0;
    for (int h=0; h<n; ++h) {
      sum += hll_cnt_size(& (counters[h]));
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
