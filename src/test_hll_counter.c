#include "hll_counter.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

START_TEST (hll_counter_rho) {
  hll_counter_t c = new_hll_counter(3);

  ck_assert_int_eq(1, hll_rho(0b1010001, c.mask));

  ck_assert_int_eq(5, hll_rho(0b010000, c.mask));

  ck_assert_int_eq(5, hll_rho(0b010000, c.mask));

  delete_hll_counter(c);
}
END_TEST

START_TEST (hll_insertion) {
  hll_counter_t c = new_hll_counter(0);

  hll_add(0b0010000, c);
  ck_assert_int_eq(5, c.registers[0]);

  hll_add(0b1000000, c);
  ck_assert_int_eq(7, c.registers[0]);

  hll_add(0b10000010, c);
  ck_assert_int_eq(7, c.registers[0]);
}
END_TEST

START_TEST (hll_insertion_2) {
  hll_counter_t c = new_hll_counter(1);

  hll_add(0b0010000, c);
  hll_add(~0, c);
  ck_assert_int_eq(5, c.registers[0]);
  ck_assert_int_eq(1, c.registers[1]);

}
END_TEST

START_TEST (hll_insertion_3) {
  hll_counter_t c = new_hll_counter(2);

  int shift = sizeof(hll_hash_t)*8 -2;

  hll_add(4, c);
  hll_add(1<<shift | 4, c);
  hll_add(2<<shift | 4, c);
  hll_add(3<<shift | 4, c);
  ck_assert_int_ne(0, c.registers[0]);
  ck_assert_int_ne(0, c.registers[1]);
  ck_assert_int_ne(0, c.registers[2]);
  ck_assert_int_ne(0, c.registers[3]);

}
END_TEST

START_TEST (hll_test_size) {
  /*
   * We will set the register value at 5.
   * The value that the hll_size function should return is
   *
   *     0.72134 / (2^-5) = 23.08 -> 23
   *
   */
  hll_counter_t c = new_hll_counter(0);
  c.registers[0] = 5;
  hll_cardinality_t actual = hll_size(c);
  ck_assert_int_eq(23, actual);
}
END_TEST

START_TEST (hll_test_size_2) {
  /*
   * We are going to set the first register at 5 and the second to 3.
   * The value that the hll_size function should return is
   *
   *     0.72134*4 / ( (2^-5) + (2^-3) ) = 18.46 -> 18
   *
   */
  hll_counter_t c = new_hll_counter(1);
  c.registers[0] = 5;
  c.registers[1] = 3;
  hll_cardinality_t actual = hll_size(c);
  ck_assert_int_eq(18, actual);
}
END_TEST

int compare (const void * a, const void * b)
{
  return ( *(hll_hash_t*)a - *(hll_hash_t*)b );
}

START_TEST (hll_size_precision) {
  hll_counter_t c = new_hll_counter(9);
  int numInsertions = 999999;
  hll_hash_t stream[numInsertions];
  int i=0;
  // Populate stream
  for(; i<numInsertions; ++i) {
    // If we don't do like this, the test fails: the problem is not in the
    // counter but in the random number generator.
    stream[i] = rand() + rand() + rand() + rand() + rand() + rand();
  }
  // Count number of different elements while pushing into counter
  qsort(stream, numInsertions, sizeof(hll_hash_t), compare);
  int n=0;
  int current = stream[0];
  for(i=1; i<numInsertions; ++i) {
    hll_add(stream[i], c);
    if(stream[i] != current) {
      ++n;
      current = stream[i];
    }
  }

  int estimate = hll_size(c);
  double error = ((double) estimate / n) * 100 - 100;

  printf("n: %d\nEstimate: %d\nError: %f\n", n, estimate, error);

  ck_assert_int_ne(0, estimate);
  for(i = 0; i<c.m; ++i) {
    ck_assert_int_ne(0, c.registers[i]);
  }
  ck_assert(-2.5 <= error && error <= 2.5);

  delete_hll_counter(c);
}
END_TEST

START_TEST (hll_equals) {
  hll_counter_t
      c1 = new_hll_counter(2),
      c2 = new_hll_counter(2);

  ck_assert(hll_counter_equals(c1,c2));

  hll_add(1234, c1);
  hll_add(1234, c2);

  ck_assert(hll_counter_equals(c1,c2));

  c2.registers[0] = 14; // set the first register to  different value
  ck_assert(!hll_counter_equals(c1,c2));
  delete_hll_counter(c1);
  delete_hll_counter(c2);
}
END_TEST

START_TEST (hll_copy) {
  hll_counter_t c1 = new_hll_counter(2);
  hll_counter_t c2 = hll_counter_copy(c1);

  ck_assert(hll_counter_equals(c1,c2));

  delete_hll_counter(c1);
  delete_hll_counter(c2);
}
END_TEST

Suite * hll_counter_suite () {
  Suite *s = suite_create("HyperLogLog Counter");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, hll_equals);
  tcase_add_test(tc_core, hll_copy);
  tcase_add_test(tc_core, hll_counter_rho);
  tcase_add_test(tc_core, hll_insertion);
  tcase_add_test(tc_core, hll_insertion_2);
  tcase_add_test(tc_core, hll_insertion_3);
  tcase_add_test(tc_core, hll_test_size);
  tcase_add_test(tc_core, hll_test_size_2);
  tcase_add_test(tc_core, hll_size_precision);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  printf(
        "Platform information:\n"
        "sizeof(int): %d\n"
        "sizeof(unsigned int): %d\n"
        "sizeof(hll_reg_t): %d\n"
        "sizeof(hll_hash_t): %d\n",
        sizeof(int),
        sizeof(unsigned int),
        sizeof(hll_reg_t),
        sizeof(hll_hash_t)
        );

  int number_failed = 0;
  Suite *s = hll_counter_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

//  hll_counter_t c1 = new_hll_counter(2);
//  hll_counter_t c2 = hll_counter_copy(c1);

//  printf("%d\n", hll_counter_equals(c1,c2));

//    delete_hll_counter(c1);
//    delete_hll_counter(c2);
}
