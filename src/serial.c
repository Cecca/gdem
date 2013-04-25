#include "counter.h" // use HyperLogLog counters
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

#define COUNTER_BITS 9

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

static unsigned char *
read_whole_file (const char * file_name)
{
    unsigned s;
    unsigned char * contents;
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
                 "but got %d: %s.\n", file_name, s, bytes_read,
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
  counter_t counter;
  counter_t prev_counter;
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
  n.counter = new_counter(COUNTER_BITS);
  n.prev_counter = new_counter(COUNTER_BITS);

  // Read neighbours and add to counter
  size_t len = strlen(rest);
  node_id_t neighbours[len];
  int i = -1;
  node_id_t neigh;
  while (rc == 2) {
    ++i;
    rc = sscanf(rest, "%d %[01223456789 ]", &neigh, rest);
    neighbours[i] = neigh;
    counter_add(neigh, n.counter);
    counter_add(neigh, n.prev_counter);
  }
  if(i>=0) { // if any, copy neighbours
    size_t mem = (i+1)*sizeof(node_id_t);
    n.num_neighbours = i+1;
    n.neighbours = malloc(mem);
    memcpy(n.neighbours, neighbours, mem);
  } else { // else set the array to NULL
    n.neighbours = NULL;
  }

  printf("New node: %d\n", n.id);
  return n;
}

int main(int argc, char **argv) {

  // TODO array doubling when necessary!
  node_t nodes[100];
  memset(nodes, 0, sizeof(node_t)*100);

  unsigned char *f = read_whole_file("graph.dat");
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
  int i, j;
  while(changed > 0) {

    for(i=0; i<node_count; ++i) { // for each node
      node_t n = nodes[i];

      for(j = 0; j<n.num_neighbours; ++j) { // for each neighbour
        counter_union(n.counter, nodes[n.neighbours[j]].prev_counter);
      }

    }

  }

}
