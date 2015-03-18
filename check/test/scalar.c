#include <check.h>
#include <csptr/smart_ptr.h>
#include "utils.h"

START_TEST (test_pointer_valid) {
    smart int *a = unique_ptr(int, 42);
    assert_valid_ptr(a);
    ck_assert_msg(get_smart_ptr_meta(a) == NULL, "Expected pointer to have no metadata");
} END_TEST

START_TEST (test_dtor_run) {
    int dtor_run = 0;
    f_destructor dtor = lambda(void, (void *ptr, void *meta) { dtor_run = 1; });
    int *a = unique_ptr(int, 42, dtor);
    assert_valid_ptr(a);
    ck_assert_msg(get_smart_ptr_meta(a) == NULL, "Expected pointer to have no metadata");
    sfree(a);
    ck_assert_msg(dtor_run, "Expected destructor to run");
} END_TEST

START_TEST (test_meta) {
    smart int *a = unique_ptr(int, 42, .meta = { &m, sizeof (m) });
    assert_valid_ptr(a);
    assert_valid_meta(a, &m, get_smart_ptr_meta(a));
} END_TEST

START_TEST (test_dtor_run_with_meta) {
    int dtor_run = 0;

    f_destructor dtor = lambda(void, (void *ptr, void *meta) { dtor_run = 1; });
    int *a = unique_ptr(int, 42, dtor, { &m, sizeof (m) });
    assert_valid_ptr(a);
    assert_valid_meta(a, &m, get_smart_ptr_meta(a));

    sfree(a);
    ck_assert_msg(dtor_run, "Expected destructor to run");
} END_TEST

TFun scalar_tests[] = {
    test_pointer_valid,
    test_meta,
    test_dtor_run,
    test_dtor_run_with_meta,
    NULL
};

