//#define USE_FLAJOLET_MARTINS_COUNTERS
#define USE_HYPER_LOG_LOG_COUNTERS

#include "counter.h"
#include "hll_counter.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

/**
 * From here http://www.cse.yorku.ca/~oz/hash.html
 */
hash_t shash(char *str) {
  unsigned long h = 5381;
  int c;

  while ((c = *str++))
    h = ((h << 5) + h) + c; /* hash * 33 + c */

  return h;
}

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

int main(int argc, char **argv) {
  if(argc != 2) {
    exit(EXIT_FAILURE);
  }
  printf("Reading file into memory\n");

  char *delimiters = " \n\t\r";
  unsigned char *f;
  f = read_whole_file(argv[1]);

  char *token = strtok(f, delimiters);

  counter_t counter = new_counter(13);

  while(token != NULL) {
    token = strtok(NULL, delimiters);
    if(token != NULL){
      counter_add(shash(token), counter);
    }
  }

  printf("%d\n", counter_size(counter));
  delete_counter(counter);
  free(f);

  return EXIT_SUCCESS;
}
