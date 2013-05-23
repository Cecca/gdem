#include "hll_counter.h"
#include <assert.h>

void hll_cnt_init (hll_counter_t *counter, size_t bits) {
  assert( bits < sizeof(hll_hash_t)*8
         && "You cannot use more bits than there are in the hash value");
  counter->b = bits;
  counter->m = 1 << bits; // 2^bits
  if (bits != 0) {
    counter->mask = ~(~0 << (sizeof(hll_hash_t)*8 - bits));
  } else {
    counter->mask = ~0;
  }

  size_t mem = counter->m*sizeof(hll_reg_t);

  counter->registers = (hll_reg_t*) malloc(mem);
  memset(counter->registers, 0, mem);
}

void hll_cnt_free (hll_counter_t * counter) {
  free(counter->registers);
}

hll_counter_t * hll_cnt_new(size_t bits) {
  hll_counter_t *c = malloc(sizeof(hll_counter_t));
  hll_cnt_init(c, bits);
  return c;
}

void hll_cnt_delete(hll_counter_t * counter) {
  hll_cnt_free(counter);
  free(counter);
}

hll_counter_t * hll_cnt_copy(hll_counter_t * counter) {
  hll_counter_t *copy = malloc(sizeof(hll_counter_t));
  memcpy(copy, counter, sizeof(hll_counter_t));
  copy->registers = malloc(counter->m*sizeof(hll_reg_t));
  hll_cnt_copy_to(counter, copy);
  return copy;
}

void hll_cnt_copy_to (hll_counter_t *from, hll_counter_t *to) {
  assert(from->b == to->b);
  memcpy(to->registers, from->registers, from->m*sizeof(hll_reg_t));
}

hll_reg_t hll_cnt_rho(hll_hash_t x, unsigned int mask) {
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

void hll_cnt_add(hll_hash_t x, hll_counter_t * cnt) {
  // Computes the register index
  size_t j = (cnt->b == 0)? 0 : x >> (sizeof(hll_hash_t)*8 - cnt->b);
  hll_reg_t r = hll_cnt_rho(x, cnt->mask);
  // assigns the maximum between the current value and rho to the register
  cnt->registers[j] = (cnt->registers[j] > r)? cnt->registers[j] : r;
}

hll_cardinality_t hll_cnt_size(hll_counter_t * counter) {
  double denominator = 0;
  int i;
  for (i = 0; i<counter->m; ++i) {
//    printf("%d\n", counter.registers[i]);
    denominator += 1.0 / (1 << counter->registers[i]); // 1 / 2^register
  }
  return counter->m*counter->m*HLL_ALPHA/denominator;
}

#define MAX(a, b) (a >= b)? a : b

hll_counter_t * hll_cnt_union(hll_counter_t * a, hll_counter_t * b) {
  assert(a->b == b->b);
  hll_counter_t * c = hll_cnt_new(a->b);
  int i = 0;
  for(; i<a->m; ++i) {
    c->registers[i] = MAX(a->registers[i], b->registers[i]);
  }
  return c;
}

void hll_cnt_union_i(hll_counter_t *a, hll_counter_t *b) {
  assert(a->b == b->b);
  int i=0;
  for(; i<a->m; ++i) {
    a->registers[i] = MAX(a->registers[i], b->registers[i]);
  }
}

#undef MAX

int hll_cnt_equals(hll_counter_t * a, hll_counter_t * b) {
  if(a->b != b->b) {
    return 0;
  }
  if(memcmp(a->registers, b->registers, a->m*sizeof(hll_reg_t)) != 0) {
    return 0;
  }
  return 1;
}
