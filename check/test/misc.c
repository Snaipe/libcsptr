#include <criterion/criterion.h>
#include <csptr/smart_ptr.h>
#include "config.h"
#include "utils.h"

Test(misc, zero_size) {
    void *ptr = smalloc(0, 0, UNIQUE);
    assert_eq(ptr, NULL, "Expected NULL pointer to be returned.");
}

#ifndef SMALLOC_FIXED_ALLOCATOR
Test(misc, alloc_failure) {
    smalloc_allocator = (s_allocator) {
        lambda(void *, (size_t s) { return NULL; }),
        lambda(void, (void *ptr) {})
    };
    smart void *ptr = unique_ptr(int, 42);
    assert_eq(ptr, NULL, "Expected NULL pointer to be returned.");
    smalloc_allocator = (s_allocator) {malloc, free};
}
#endif
