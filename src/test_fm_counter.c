#include "fm_counter.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

START_TEST (fm_counter_add_prbabilistic) {
  fm_counter_t c = new_fm_counter(1);
  int n = 99999, i=0;

  for (i=0; i<n; ++i) {
    fm_add(rand(), c);
  }

  // The register should be different from 0
  ck_assert_int_ne(0, c.registers[0]);

  // The only register of c should have, almost certainly, the 0th and first
  // bit set after n add operations with random numbers
  ck_assert_int_eq(1, c.registers[0] & 1);
  ck_assert_int_eq(1, (c.registers[0] >> 1) & 1);

  // The only register of c should have, almost certainly, the most significant
  // bits unset after n add operations with random numbers
  ck_assert_int_eq(0, (c.registers[0] >> (sizeof(fm_hash_t)*8 -1)) & 1);
  ck_assert_int_eq(0, (c.registers[0] >> (sizeof(fm_hash_t)*8 -2)) & 1);

  delete_fm_counter(c);
}
END_TEST

int compare (const void * a, const void * b)
{
  return ( *(fm_hash_t*)a - *(fm_hash_t*)b );
}

START_TEST (fm_size_precision) {
  fm_counter_t c = new_fm_counter(2048);
  int numInsertions = 999999;
  fm_hash_t stream[numInsertions];
  int i=0;
  // Populate stream
  for(; i<numInsertions; ++i) {
    stream[i] = rand();
  }
  // Count number of different elements while pushing into counter
  qsort(stream, numInsertions, sizeof(fm_hash_t), compare);
  int n=0;
  int current = stream[0];
  for(i=1; i<numInsertions; ++i) {
    fm_add(stream[i], c);
    if(stream[i] != current) {
      ++n;
      current = stream[i];
    }
  }

  int estimate = fm_size(c);
  double error = ((double) estimate / n) * 100 - 100;

  printf("n: %d\nEstimate: %d\nError: %f\n", n, estimate, error);

  ck_assert_int_ne(0, estimate);
  ck_assert(-2.5 <= error && error <= 2.5);

  delete_fm_counter(c);
}
END_TEST

START_TEST (fm_counter_rho) {
  ck_assert_int_eq(32, fm_rho(0));

  ck_assert_int_eq(0, fm_rho(1));

  ck_assert_int_eq(3, fm_rho(0b1000));

  ck_assert_int_eq(5, fm_rho(0b101100101010010100100000));
}
END_TEST

START_TEST (fm_counter_observable) {
  ck_assert_int_eq(1, fm_observable(1));

  ck_assert_int_eq(6, fm_observable(0b0111111));

  ck_assert_int_eq(4, fm_observable(0b1101111));
}
END_TEST

Suite * fm_counter_suite () {
  Suite *s = suite_create("Flajolet-Martins Counter");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, fm_counter_add_prbabilistic);
  tcase_add_test(tc_core, fm_counter_rho);
  tcase_add_test(tc_core, fm_counter_observable);
  tcase_add_test(tc_core, fm_size_precision);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed = 0;
  Suite *s = fm_counter_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
