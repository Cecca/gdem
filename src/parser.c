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

int parse_node_descr_to (char *descr, node_t *node) {
  int id;
  char out_s[MAX_ADJ_LEN], in_s[MAX_ADJ_LEN];

  int read =
      sscanf(descr, "%d | %[0123456789 ] | %[0123456789 ]", &id, out_s, in_s);

  if(read != 3) {
    error_message(descr);
    return 1;
  }

  int n_out = count_numbers(out_s);
  int n_in = count_numbers(in_s);

  if (n_in < 0 || n_out < 0) {
    error_message(descr);
    return 1;
  }

  node_init(node, id, n_out, n_in);

  int inserted = -1;

  inserted = populate_adjacency(out_s, node->out, n_out);
  if (inserted != n_out) {
    error_message(descr);
    node_free(node);
    return 1;
  }

  inserted = populate_adjacency(in_s, node->in, n_in);
  if (inserted != n_in) {
    error_message(descr);
    node_free(node);
    return 1;
  }

  return 0;
}

node_t * parse_node_descr (char *descr) {
  int id;
  char out_s[MAX_ADJ_LEN], in_s[MAX_ADJ_LEN];

  int read =
      sscanf(descr, "%d | %[0123456789 ] | %[0123456789 ]", &id, out_s, in_s);

  if(read != 3) {
    error_message(descr);
    return NULL;
  }

  int n_out = count_numbers(out_s);
  int n_in = count_numbers(in_s);

  if (n_in < 0 || n_out < 0) {
    error_message(descr);
    return NULL;
  }

  node_t * node = node_new(id, n_out, n_in);

  int inserted = -1;

  inserted = populate_adjacency(out_s, node->out, n_out);
  if (inserted != n_out) {
    error_message(descr);
    node_delete(node);
    return NULL;
  }

  inserted = populate_adjacency(in_s, node->in, n_in);
  if (inserted != n_in) {
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
  char *str = adj_str;
  node_id_t elem;
  int i = 0;

  while (str != NULL && i<n) {
    elem = strtol(str, &str, 10);
    adj[i] = elem;
    ++i;
  }

  // there are still some elements
  if (str != NULL && strcmp(str, "") != 0) {
    int j = 0;
    for(; j< strlen(str); ++j) {
      if(!isspace(str[j])) {
        fprintf(stderr,
                "%s:%d: ERROR: trying to populate an andjacency list with more "
                "values than the array can host\n", __FILE__, __LINE__);
        return -1;
      }
    }
  }

  return i;
}

/*
 * Reads an entire file into a dinamically allocated buffer.
 */
char * read_file (char * filename) {
  FILE *f = fopen("text.txt", "rb");
  fseek(f, 0, SEEK_END);
  long pos = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *bytes = malloc(pos);
  if (bytes == NULL) {
    fprintf(stderr, "ERROR: Not enough memory to load file %s in memory\n",
            filename);
  }
  fread(bytes, pos, 1, f);
  fclose(f);

  return bytes;
}

int count_lines (char *str) {
  int count = 0;
  for (int i=0; i < strlen(str); ++i) {
    if (str[i] == '\n') {
      ++count;
    }
  }
  return count;
}

int parse_graph_file (char *filename, node_t **nodes, int *n) {
  char * file_contents = read_file(filename);

  int num_parsed = parse_graph_string(file_contents, nodes, n);

  free(file_contents);
  return num_parsed;
}

int parse_graph_string (char *str, node_t **nodes, int *n) {
  *n = count_lines(str);
  int num_parsed = 0;

  *nodes = malloc(*n * sizeof(node_t));

  char *line = strtok(str, "\n");

  while(line != NULL) {
    line = strtok(NULL, "\n");
    if(line != NULL) {

    }
  }

  return num_parsed;
}
