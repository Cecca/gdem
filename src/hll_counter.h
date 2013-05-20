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

/**
 * @brief Type of the counter.
 */
typedef struct hll_counter hll_counter_t;

// ----------------------------------------------------------------------------
//   Memory Management for counters
// ----------------------------------------------------------------------------

/**
 * @brief Creates a new hyperLogLog counter, with the specified number of
 * bits to index the registers.
 *
 * The number of registers is m = 2^bits.
 *
 * **Attention**: the memory for the registers of the counter is dynamically
 * allocated. Hence, once used, the counter _must_ be deallocated with the
 * function hll_cnt_delete(hll_counter_t).
 *
 * @param bits: the bits to index the registers.
 * @return a new hll_counter_t instance
 */
hll_counter_t hll_cnt_new(size_t bits);

/**
 * @brief Deletes a counter.
 *
 * This functions is responsible of deallocating the right amount of memory.
 *
 * @param counter the counter to be deallocated.
 */
void hll_cnt_delete(hll_counter_t counter);

/**
 * @brief Copies a counter.
 *
 * A deep copy of the counter is performed: the pointer ot the registers array
 * is not shared.
 *
 * @param counter the counter to copy
 * @return the new copy of the counter.
 */
hll_counter_t hll_cnt_copy(hll_counter_t counter);

// ----------------------------------------------------------------------------
//   Counter operations
// ----------------------------------------------------------------------------

/**
 * @brief Returns the position of the rightmost 1-bit in a hash.
 *
 * **Attention**: bit indexes returned by this method
 * start at 1, not at 0!
 *
 * @param elem the hash to be evaluated
 * @param mask the bit mask to hide the first `b` bits of the hash.
 * @return the position of the rightmost 1-bit in the hash, starting from 1.
 */
inline hll_reg_t hll_cnt_rho(hll_hash_t elem, unsigned int mask);

/**
 * @brief Add a new element to the counter.
 *
 * @param elem the element to be added.
 * @param counter the counter to be updated.
 */
void hll_cnt_add(hll_hash_t elem, hll_counter_t counter);

/**
 * @brief Estimates the cardinality of the underlying multiset.
 *
 * @param counter the counter used to get the estimation.
 * @return the estimated cardinality of the multiset.
 */
hll_cardinality_t hll_cnt_size(hll_counter_t counter);

/**
 * @brief Performs theunion of two counters.
 *
 * The union of two counters is defined as the register by register maximum.
 *
 * @param a
 * @param b
 * @return
 */
hll_counter_t hll_cnt_union(hll_counter_t a, hll_counter_t b);

/**
 * @brief Checks if two counters are equals.
 *
 * Two counters are equal if they have the same `b` and their registers
 * are equals.
 *
 * @param a the first counter
 * @param b the second counter
 * @return `0` if the counters are not equals.
 */
int hll_cnt_equals(hll_counter_t a, hll_counter_t b);

#endif // _HLL_COUNTER_H_
