#include "hll_counter.h"
#include <assert.h>

/**
 * From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
inline size_t nearest_power(size_t v) {
  // If it is already a power of two, return it.
  if((v & (v - 1)) == 0)
    return v;

  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  if(sizeof(size_t) == 8) { // if it is a 64 bit word
    v |= v >> 32;
  }
  v++;
  return v;
}

/**
 * Finds the log of a number that is a power of 2
 * TODO check if it works
 */
inline unsigned int log_of_power(unsigned int v) {
  static const unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0,
                                   0xFF00FF00, 0xFFFF0000};
  register unsigned int r = (v & b[0]) != 0;
  int i;
  for (i = 4; i > 0; i--) // unroll for speed...
  {
    r |= ((v & b[i]) != 0) << i;
  }
  return r;
}

hll_counter_t new_hll_counter(size_t bits) {
  assert(("You cannot use more bits than there are in the hash value",
          bits < sizeof(hll_hash_t)*8));
  hll_counter_t c;
  c.b = bits;
  c.m = 1 << bits; // 2^bits
  if (bits != 0) {
    c.mask = ~(~0 << (sizeof(hll_hash_t)*8 - bits));
  } else {
    c.mask = ~0;
  }

  size_t mem = c.m*sizeof(hll_reg_t);

  c.registers = (hll_reg_t*) malloc(mem);
  memset(c.registers, 0, mem);
  return c;
}

void delete_hll_counter(hll_counter_t counter){
  free(counter.registers);
}

inline hll_reg_t hll_rho(hll_hash_t x, unsigned int mask) {
  if( (x & mask) == 0 ) {
    return sizeof(hll_hash_t)*8;
  }
  hll_reg_t i = 1;
  int k;
  for(k = x & mask; (k & 1) != 1 ; k = k >> 1) {
    ++i;
  }
  return i;
}

void hll_add(hll_hash_t x, hll_counter_t cnt) {
  // Computes the register index
  size_t j = (cnt.b == 0)? 0 : x >> (sizeof(hll_hash_t)*8 - cnt.b);
  hll_reg_t r = hll_rho(x, cnt.mask);
  // assigns the maximum between the current value and rho to the register
  cnt.registers[j] = (cnt.registers[j] > r)? cnt.registers[j] : r;
}

hll_cardinality_t hll_size(hll_counter_t counter) {
  double denominator = 0;
  int i;
  for (i = 0; i<counter.m; ++i) {
//    printf("%d\n", counter.registers[i]);
    denominator += 1.0 / (1 << counter.registers[i]); // 1 / 2^register
  }
  return counter.m*counter.m*HLL_ALPHA/denominator;
}

hll_counter_t hll_union(hll_counter_t a, hll_counter_t b) {
  assert(a.b == b.b);
  hll_counter_t c = new_hll_counter(a.b);
  int i = 0;
  for(; i<a.m; ++i) {
    c.registers[i] = // max
        (a.registers[i] >= b.registers[i])? a.registers[i] : b.registers[i];
  }
  return c;
}
