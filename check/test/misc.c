#include <check.h>
#include <csptr/smart_ptr.h>
#include "config.h"
#include "utils.h"

START_TEST (test_more_args) {
    smart void *ptr = unique_ptr(int, NULL, NULL, NULL, NULL);
    assert_valid_ptr(ptr);
} END_TEST

#ifndef FIXED_SMALLOC_ALLOCATOR
START_TEST (test_alloc_failure) {
    smalloc_allocator = (s_allocator) {
        lambda(void *, (size_t s) { return NULL; }),
        lambda(void, (void *ptr) {})
    };
    smart void *ptr = unique_ptr(int);
    assert_valid_ptr(ptr);
    smalloc_allocator = (s_allocator) {malloc, free};
} END_TEST
#endif

TFun misc_tests[] = {
    test_more_args,
    NULL
};
