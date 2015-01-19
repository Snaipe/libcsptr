#include <check.h>
#include <csptr/smart_ptr.h>
#include "config.h"
#include "utils.h"

START_TEST (test_more_args) {
    smart void *ptr = unique_ptr(int, NULL, NULL, NULL, NULL);
    assert_valid_ptr(ptr);
} END_TEST

START_TEST (test_zero_size) {
    void *ptr = smalloc(0, 0, UNIQUE);
    ck_assert_msg(ptr == NULL, "Expected NULL pointer to be returned.");
} END_TEST

#ifndef FIXED_SMALLOC_ALLOCATOR
START_TEST (test_alloc_failure) {
    smalloc_allocator = (s_allocator) {
        lambda(void *, (size_t s) { return NULL; }),
        lambda(void, (void *ptr) {})
    };
    smart void *ptr = unique_ptr(int);
    ck_assert_msg(ptr == NULL, "Expected NULL pointer to be returned.");
    smalloc_allocator = (s_allocator) {malloc, free};
} END_TEST
#endif

TFun misc_tests[] = {
    test_more_args,
    test_alloc_failure,
    test_zero_size,
    NULL
};
