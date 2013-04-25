#ifndef _HLL_COUNTER_H_
#define _HLL_COUNTER_H_

#include <malloc.h>
#include <string.h>

typedef unsigned int hll_cardinality_t;

// ----------------------------------------------------------------------------
//   Counter Definitions
// ----------------------------------------------------------------------------

#define HLL_ALPHA 0.72134

typedef unsigned char hll_reg_t;
typedef unsigned int hll_hash_t;

struct hll_counter {
  size_t m;
  unsigned int b;
  unsigned int mask;
  hll_reg_t *registers;
};

typedef struct hll_counter hll_counter_t;

// ----------------------------------------------------------------------------
//   Memory Management for counters
// ----------------------------------------------------------------------------

/**
 * @brief Creates a new hyperLogLog counter, with the specified number of
 * bits to index the registers.
 * @param numRegisters
 * @return
 */
hll_counter_t new_hll_counter(size_t bits);

void delete_hll_counter(hll_counter_t counter);

// ----------------------------------------------------------------------------
//   Counter operations
// ----------------------------------------------------------------------------

/**
 * Returns the position of the rightmost 1-bit.
 *
 * **Attention**: bit indexes returned by this method
 * start at 1, not at 0!
 *
 * @param x
 * @return
 */
inline hll_reg_t hll_rho(hll_hash_t elem, unsigned int mask);

void hll_add(hll_hash_t elem, hll_counter_t counter);

hll_cardinality_t hll_size(hll_counter_t counter);

hll_counter_t hll_union(hll_counter_t a, hll_counter_t b);

#endif // _HLL_COUNTER_H_
