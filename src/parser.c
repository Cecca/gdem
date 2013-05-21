#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "null.h"
#include "boolean.h"

void error_message(char* descr) {
  fprintf(stderr, "Error in format of description: < %s > SKIPPING\n", descr);
}

#define MAX_ADJ_LEN 1000

node_t * parse_node_descr(char *descr) {
  int id;
  char out_s[MAX_ADJ_LEN], in_s[MAX_ADJ_LEN];

  int read =
      sscanf(descr, "%d | %[0123456789 ] | %[0123456789 ]", &id, out_s, in_s);

  if(read != 3) {
    error_message(descr);
    return NULL;
  }

  int n_in = count_numbers(in_s);
  int n_out = count_numbers(out_s);

  if (n_in < 0 || n_out < 0) {
    error_message(descr);
    return NULL;
  }

  node_t * node = node_new(id, n_out, n_in);

  int inserted = -1;
  inserted = populate_adjacency(in_s, node->in, n_in);
  if (inserted != n_in) {
    error_message(descr);
    node_delete(node);
    return NULL;
  }

  populate_adjacency(out_s, node->out, n_out);
  if (inserted != n_out) {
    error_message(descr);
    node_delete(node);
    return NULL;
  }

  return node;
}

int count_numbers (char *str) {
  int count = 0;
  int i=0;
  int was_digit = FALSE;
  for (; i<strlen(str); ++i) {
    if (isdigit(str[i])) {
      if (!was_digit) {
        ++count;
      }
      was_digit = TRUE;
    } else if (isspace(str[i])) {
      was_digit = FALSE;
    } else {
      return -1;
    }
  }
  return count;
}

int populate_adjacency(char *adj_str, node_id_t *adj, int n) {

  return -1;
}
