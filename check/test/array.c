#include <check.h>
#include "csptr/array.h"
#include "utils.h"

#define ARRAY_SIZE 25

__attribute__((always_inline))
inline void assert_valid_array(void *ptr, size_t len, size_t size) {
    assert_valid_ptr(ptr);
    ck_assert_msg(array_length(ptr) == len,
            "Mismatching array lengths.");
    ck_assert_msg(array_type_size(ptr) == size,
            "Mismatching compound type sizes.");
}

START_TEST (test_array) {
    smart int *arr = unique_ptr(int[ARRAY_SIZE], {});
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));
} END_TEST

START_TEST (test_array_dtor_run) {
    int dtor_run = 0;
    int *arr;
    f_destructor dtor = lambda(void, (void *ptr, UNUSED void *meta) {
            ck_assert(ptr == arr + dtor_run);
            dtor_run++;
        });

    arr = unique_ptr(int[ARRAY_SIZE], {}, dtor);
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));

    sfree(arr);
    ck_assert_msg(dtor_run == ARRAY_SIZE, "Expected destructor to run");
} END_TEST

START_TEST (test_array_meta) {
    smart int *arr = unique_ptr(int[ARRAY_SIZE], {}, .meta = { &m, sizeof (m) });
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));
    assert_valid_meta(&m, array_user_meta(arr));
} END_TEST

START_TEST (test_array_dtor_run_with_meta) {
    int dtor_run = 0;
    f_destructor dtor = lambda(void, (UNUSED void *ptr, UNUSED void *meta) { dtor_run = 1; });

    int *arr = unique_ptr(int[ARRAY_SIZE], {}, dtor, { &m, sizeof (m) });
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));
    assert_valid_meta(&m, array_user_meta(arr));

    sfree(arr);
    ck_assert_msg(dtor_run, "Expected destructor to run");
} END_TEST

TCase *make_array_tests(void) {
    TCase *tc = tcase_create("array");
    tcase_add_test(tc, test_array);
    tcase_add_test(tc, test_array_meta);
    tcase_add_test(tc, test_array_dtor_run);
    tcase_add_test(tc, test_array_dtor_run_with_meta);
    return tc;
}
