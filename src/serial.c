#include "hll_counter.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

#define COUNTER_BITS 2
#define MAX_ITER 10

/* This routine returns the size of the file it is called with. */

static unsigned
get_file_size (const char * file_name)
{
    struct stat sb;
    if (stat (file_name, & sb) != 0) {
        fprintf (stderr, "'stat' failed for '%s': %s.\n",
                 file_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    return sb.st_size;
}

/* This routine reads the entire file into memory. */

static char *
read_whole_file (const char * file_name)
{
    unsigned s;
    char * contents;
    FILE * f;
    size_t bytes_read;
    int status;

    s = get_file_size (file_name);
    contents = malloc (s + 1);
    if (! contents) {
        fprintf (stderr, "Not enough memory.\n");
        exit (EXIT_FAILURE);
    }

    f = fopen (file_name, "r");
    if (! f) {
        fprintf (stderr, "Could not open '%s': %s.\n", file_name,
                 strerror (errno));
        exit (EXIT_FAILURE);
    }
    bytes_read = fread (contents, sizeof (unsigned char), s, f);
    if (bytes_read != s) {
        fprintf (stderr, "Short read of '%s': expected %d bytes "
                 "but got %lu: %s.\n", file_name, s, bytes_read,
                 strerror (errno));
        exit (EXIT_FAILURE);
    }
    status = fclose (f);
    if (status != 0) {
        fprintf (stderr, "Error closing '%s': %s.\n", file_name,
                 strerror (errno));
        exit (EXIT_FAILURE);
    }
    return contents;
}

typedef unsigned int node_id_t;

typedef struct node node_t;
struct node {
  hll_counter_t counter;
  hll_counter_t prev_counter;
  node_id_t id;
  size_t num_neighbours;
  node_id_t *neighbours; // array of indices of out neighbours
};

node_t new_node(char *descr) {
//  printf("\n%s\n", descr);
  char rest[100];
  int rc;
  node_t n;

  // read the ID
  int id;
  rc = sscanf(descr, "%d %[01223456789 ]", &id, rest);
  if(rc <= 0)
    printf("Matching failed\n");
  n.id = id;

  // init counters
  n.counter = hll_cnt_new(COUNTER_BITS);
  n.prev_counter = hll_cnt_new(COUNTER_BITS);

  hll_cnt_add(id, n.counter);
  hll_cnt_add(id, n.prev_counter);

  // Read neighbours
  size_t len = strlen(rest);
  node_id_t neighbours[len];
  int i = -1;
  node_id_t neigh;
  while (rc == 2) {
    ++i;
    rc = sscanf(rest, "%d %[01223456789 ]", &neigh, rest);
    neighbours[i] = neigh;
//    hll_cnt_add(neigh, n.counter);
//    hll_cnt_add(neigh, n.prev_counter);
  }
  if(i>=0) { // if any, copy neighbours
    size_t mem = (i+1)*sizeof(node_id_t);
    n.num_neighbours = i+1;
    n.neighbours = malloc(mem);
    memcpy(n.neighbours, neighbours, mem);
  } else { // else set the array to NULL
    n.neighbours = NULL;
  }

//  printf("New node: %d\n", n.id);
  return n;
}

int main(int argc, char **argv) {

  // TODO array doubling when necessary!
  node_t nodes[100];
  memset(nodes, 0, sizeof(node_t)*100);

  char *f = read_whole_file("graph.dat");
  char *line = strtok(f, "\n");
  int node_count = 0;
  while(line != NULL) {
    line = strtok(NULL, "\n");
    if(line != NULL) {
      node_t n = new_node(line);
      nodes[n.id] = n;
      node_count++;
    }
  }

  int changed = node_count;
  int i, j, iteration = 0;

  hll_cardinality_t neigbourhood_function[MAX_ITER];
  memset(neigbourhood_function, 0, MAX_ITER*sizeof(hll_cardinality_t));

  while(changed > 0 && iteration < MAX_ITER) {
    // compute neighbourhood function

    for(i=0; i<node_count; ++i) { // for each node
      neigbourhood_function[iteration] += hll_cnt_size(nodes[i].counter);
    }
    printf("Iteration %d: N(%d) = %d\n",
           iteration, iteration, neigbourhood_function[iteration]);
    iteration++;

    changed = 0;

    for(i=0; i<node_count; ++i) { // for each node
      node_t n = nodes[i];

      for(j = 0; j<n.num_neighbours; ++j) { // for each neighbour
        n.counter =
            hll_cnt_union(n.counter, nodes[n.neighbours[j]].prev_counter);
      }
      if(!hll_cnt_equals(n.counter, n.prev_counter)) {
        ++changed;
      }
      hll_cnt_delete(n.prev_counter);
      n.prev_counter = hll_cnt_copy(n.counter);
    }

  }

  return 0;
}
