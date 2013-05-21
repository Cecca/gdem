#include "graph.h"

node_t * node_new (node_id_t id, size_t out_n, size_t in_n) {
  node_t *n = malloc(sizeof(node_t));
  n->id = id;
  n->num_out = out_n;
  n->num_in = in_n;
  n->out = malloc(out_n*sizeof(node_id_t));
  n->in = malloc(in_n*sizeof(node_id_t));
  return n;
}


void node_delete (node_t *node) {
  free(node->out);
  free(node->in);
  free(node);
}
