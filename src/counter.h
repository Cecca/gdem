#ifdef USE_FLAJOLET_MARTINS_COUNTERS

#include "fm_counter.h"

typedef fm_cardinality_t cardinality_t;
typedef fm_hash_t hash_t;
typedef fm_counter_t counter_t;

inline counter_t new_counter(size_t numRegisters) {
  return new_fm_counter(numRegisters);
}

inline void delete_counter(counter_t counter) {
  delete_fm_counter(counter);
}

inline void counter_add(hash_t elem, counter_t counter) {
  fm_add(elem, counter);
}

inline cardinality_t counter_size(counter_t counter) {
  return fm_size(counter);
}

inline counter_t counter_union(counter_t a, counter_t b) {
  return fm_union(a, b);
}

inline int counter_equals(counter_t a, counter_t b) {
  return fm_counter_equals(a, b);
}

inline counter_t counter_copy(counter_t counter) {
  return fm_counter_copy(counter);
}

#else // USE_HYPER_LOG_LOG_COUNTERS

#include "hll_counter.h"

typedef hll_cardinality_t cardinality_t;
typedef hll_hash_t hash_t;
typedef hll_counter_t counter_t;

inline counter_t new_counter(size_t numBits) {
  return new_hll_counter(numBits);
}

inline void delete_counter(counter_t counter) {
  delete_hll_counter(counter);
}

inline void counter_add(hash_t elem, counter_t counter) {
  hll_add(elem, counter);
}

inline cardinality_t counter_size(counter_t counter) {
  return hll_size(counter);
}

inline counter_t counter_union(counter_t a, counter_t b) {
  return hll_union(a, b);
}

inline int counter_equals(counter_t a, counter_t b) {
  return hll_counter_equals(a, b);
}

inline counter_t counter_copy(counter_t counter) {
  return hll_counter_copy(counter);
}

#endif

