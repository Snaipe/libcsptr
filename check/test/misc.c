#include <check.h>
#include <csptr/smart_ptr.h>
#include "config.h"
#include "utils.h"

START_TEST (test_zero_size) {
    void *ptr = smalloc(0, 0, UNIQUE);
    ck_assert_msg(ptr == NULL, "Expected NULL pointer to be returned.");
} END_TEST

#ifndef SMALLOC_FIXED_ALLOCATOR
START_TEST (test_alloc_failure) {
    smalloc_allocator = (s_allocator) {
        lambda(void *, (size_t s) { return NULL; }),
        lambda(void, (void *ptr) {})
    };
    smart void *ptr = unique_ptr(int, 42);
    ck_assert_msg(ptr == NULL, "Expected NULL pointer to be returned.");
    smalloc_allocator = (s_allocator) {malloc, free};
} END_TEST
#endif

TFun misc_tests[] = {
#ifndef SMALLOC_FIXED_ALLOCATOR
    test_alloc_failure,
#endif
    test_zero_size,
    NULL
};
