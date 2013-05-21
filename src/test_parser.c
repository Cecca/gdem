#include <check.h>
#include <stdio.h>
#include "parser.h"

START_TEST (count_numbers_test) {
  char *str = "1 2 3 4";
  ck_assert_int_eq(count_numbers(str), 4);

  str = "12 3 4 5 2 54  ";
  ck_assert_int_eq(count_numbers(str), 6);
}
END_TEST

Suite * parser_suite () {
  Suite *s = suite_create("Parser");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, count_numbers_test);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed = 0;
  Suite *s = parser_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
