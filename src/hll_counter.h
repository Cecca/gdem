/**
 * @file   hll_counter.h
 * @brief  Header for HyperLogLog counters.
 *
 * This header defines data structures and operations related to
 * HyperLogLog counters
 * [FlFuGaMe07](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf).
 *
 * This counters are probabilistic data structures used to keep track
 * of the cardinality of large multisets.
 *
 * On a counter we can perform the following operations
 *
 *  - add a new element
 *  - query for the estimated size of the underlying multiset
 *  - get the union of two counters.
 */

#ifndef _HLL_COUNTER_H_
#define _HLL_COUNTER_H_

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/**
 * @brief Numeric type of estimated cardinalities.
 *
 * This typedef is used to hide the actual numeric type used to represent
 * multiset cardinalities.
 */
typedef uint32_t hll_cardinality_t;

// ----------------------------------------------------------------------------
//   Counter Definitions
// ----------------------------------------------------------------------------

/**
 * Correction constant for the estimation
 */
#define HLL_ALPHA 0.72134

/**
 * @brief Type of counter registers
 */
typedef uint8_t hll_reg_t;

/**
 * @brief Type of hash values.
 *
 * This data type is the input of the add operation.
 */
typedef uint32_t hll_hash_t;

/**
 * @brief This struct is the actual counter.
 *
 * It contains an array of registers and their number, the number of
 * bits to be used in partitioning operations and the mask to be used during
 * update operations.
 */
struct hll_counter {
  /**
   * The number of registers.
   */
  size_t m;
  /**
   * The number of bits used to select the register to update.
   */
  unsigned int b;
  /**
   * The mask used during updates.
   */
  hll_hash_t mask;
  /**
   * The array of registers of this counter.
   */
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
hll_counter_t hll_cnt_new(size_t bits);

void hll_cnt_delete(hll_counter_t counter);

hll_counter_t hll_cnt_copy(hll_counter_t counter);

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
inline hll_reg_t hll_cnt_rho(hll_hash_t elem, unsigned int mask);

void hll_cnt_add(hll_hash_t elem, hll_counter_t counter);

hll_cardinality_t hll_cnt_size(hll_counter_t counter);

hll_counter_t hll_cnt_union(hll_counter_t a, hll_counter_t b);

int hll_cnt_equals(hll_counter_t a, hll_counter_t b);

#endif // _HLL_COUNTER_H_
