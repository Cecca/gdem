#include "fm_counter.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>

fm_counter_t new_fm_counter(size_t numRegisters) {
  fm_counter_t c;
  c.m = numRegisters;

  size_t mem = numRegisters * sizeof(fm_reg_t);

  c.registers = (fm_reg_t*) malloc( mem );
  memset( c.registers, 0, mem );

  return c;
}

void delete_fm_counter(fm_counter_t counter) {
  free(counter.registers);
}

fm_counter_t fm_counter_copy(fm_counter_t counter) {
  fm_counter_t c;
  c.m = counter.m;
  memcpy(c.registers, counter.registers, counter.m*sizeof(fm_reg_t));
  return c;
}

inline size_t fm_rho(fm_hash_t elem) {
  if(elem == 0) {
    return sizeof(fm_hash_t)*8;
  }
  size_t i = 0;
  for(; (elem & 1) != 1; elem = elem >> 1) {
    ++i;
  }

  return i;
}

inline size_t fm_observable(fm_reg_t reg) {
  size_t o = 0;
  for (; (reg & 1) != 0; reg = reg >> 1 ) {
    ++o;
  }
  return o;
}

void fm_add(fm_hash_t elem, fm_counter_t counter) {
  size_t
      alpha = elem % counter.m,
      i = fm_rho(elem);

  counter.registers[alpha] |= 1 << i;
}

fm_cardinality_t fm_size(fm_counter_t counter) {
  double A = 0;
  size_t i = 0;
  for ( ; i < counter.m ; ++i ) {
    A += fm_observable(counter.registers[i]);
  }
  A = A / counter.m;

  double ratio = (double) counter.m/FM_PHI;
  double power = pow(2, A);
  double result = ratio * power;

  printf("A: %f\nratio: %f\npower: %f\nresult: %f\nresult int:%d\n",
         A, ratio, power, result, (fm_cardinality_t) result);
  return (fm_cardinality_t) result;
}

fm_counter_t fm_union(fm_counter_t a, fm_counter_t b) {
  assert(a.m == b.m);
  fm_counter_t c = new_fm_counter(a.m);
  size_t i = 0;
  for (; i < a.m; ++i) {
    c.registers[i] = a.registers[i] | b.registers[i];
  }
  return c;
}

int fm_counter_equals(fm_counter_t a, fm_counter_t b) {
  if(a.m != b.m)
    return 0;
  return memcmp(a.registers, b.registers, a.m*sizeof(fm_reg_t)) == 0 ;
}
