
#include <check.h>
#include "csptr/smart_ptr.h"
#include "utils.h"

START_TEST (test_shared_init) {
    smart void *ptr = shared_ptr(int, 42);
    assert_valid_ptr(ptr);
} END_TEST

START_TEST (test_shared_sref) {
    int dtor_run = 0;
    f_destructor dtor = lambda(void, (UNUSED void *ptr, UNUSED void *meta) { dtor_run = 1; });
    smart void *ptr = shared_ptr(int, 42, dtor);
    assert_valid_ptr(ptr);

    {
        smart void *ptr2 = sref(ptr);
        ck_assert_msg(ptr == ptr2, "Expected reference to be the same pointer.");
    }
    ck_assert_msg(dtor_run == 0, "Expected destructor NOT to have run.");
} END_TEST

TFun shared_tests[] = {
    test_shared_init,
    test_shared_sref,
    NULL
};
