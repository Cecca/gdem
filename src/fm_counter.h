#ifndef COUNTER_H
#define COUNTER_H

#include <malloc.h>
#include <string.h>


typedef unsigned int fm_cardinality_t;

// ----------------------------------------------------------------------------
//   Counter Definitions
// ----------------------------------------------------------------------------

typedef unsigned int fm_reg_t;
typedef unsigned int fm_hash_t;

#define FM_PHI 0.77351

struct fm_counter {
  size_t m;
  fm_reg_t *registers;
};

typedef struct fm_counter fm_counter_t;

// ----------------------------------------------------------------------------
//   Memory Management for counters
// ----------------------------------------------------------------------------

fm_counter_t new_fm_counter(size_t numRegisters);

void delete_fm_counter(fm_counter_t counter);

fm_counter_t fm_counter_copy(fm_counter_t counter);

// ----------------------------------------------------------------------------
//   Counter operations
// ----------------------------------------------------------------------------

inline size_t fm_rho(fm_hash_t elem);

inline size_t fm_observable(fm_reg_t reg);

void fm_add(fm_hash_t elem, fm_counter_t counter);

fm_cardinality_t fm_size(fm_counter_t counter);

fm_counter_t fm_union(fm_counter_t a, fm_counter_t b);

int fm_counter_equals(fm_counter_t a, fm_counter_t b);

#endif // _COUNTER_H
