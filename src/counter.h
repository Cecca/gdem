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


