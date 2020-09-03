#include <check.h>
#include "csptr/smart_ptr.h"
#include "csptr/config.h"
#include "utils.h"

START_TEST (test_zero_size) {
    void *ptr = smalloc(0, 0, UNIQUE);
    ck_assert_msg(ptr == NULL, "Expected NULL pointer to be returned.");
} END_TEST

#ifndef SMALLOC_FIXED_ALLOCATOR
START_TEST (test_alloc_failure) {
    smalloc_allocator = (s_allocator) {
        lambda(void *, (UNUSED size_t s) { return NULL; }),
        lambda(void, (UNUSED void *ptr) {})
    };
    smart void *ptr = unique_ptr(int, 42);
    ck_assert_msg(ptr == NULL, "Expected NULL pointer to be returned.");
    smalloc_allocator = (s_allocator) {malloc, free};
} END_TEST
#endif

TCase *make_misc_tests(void) {
    TCase *tc = tcase_create("misc");
#ifndef SMALLOC_FIXED_ALLOCATOR
    tcase_add_test(tc, test_alloc_failure);
#endif
    tcase_add_test(tc, test_zero_size);
    return tc;
}
