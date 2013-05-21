#include <check.h>
#include <stdio.h>
#include "parser.h"

START_TEST (count_numbers_test) {
  char *str = "1 2 3 4";
  ck_assert_int_eq(count_numbers(str), 4);

  str = "12 3 4 5 2 54  ";
  ck_assert_int_eq(count_numbers(str), 6);

  str = "12 3 4 5 2 string 54  ";
  ck_assert_int_eq(count_numbers(str), -1);
}
END_TEST

START_TEST (populate_adjacency_test) {
  char *str = "1 2 3 4";
  node_id_t adj[4];

  int rc = populate_adjacency(str, adj, 4);
  ck_assert_int_eq(adj[0], 1);
  ck_assert_int_eq(adj[1], 2);
  ck_assert_int_eq(adj[2], 3);
  ck_assert_int_eq(adj[3], 4);
  ck_assert_int_eq(rc, 4);
}
END_TEST

Suite * parser_suite () {
  Suite *s = suite_create("Parser");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, count_numbers_test);
  tcase_add_test(tc_core, populate_adjacency_test);
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
