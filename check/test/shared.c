#include <criterion/criterion.h>
#include <csptr/smart_ptr.h>
#include "utils.h"

Test(shared, init) {
    smart void *ptr = shared_ptr(int, 42);
    assert_valid_ptr(ptr);
}

Test(shared, sref) {
    int dtor_run = 0;
    f_destructor dtor = lambda(void, (void *ptr, void *meta) { dtor_run = 1; });
    smart void *ptr = shared_ptr(int, 42, dtor);
    assert_valid_ptr(ptr);

    {
        smart void *ptr2 = sref(ptr);
        assert_eq(ptr, ptr2, "Expected reference to be the same pointer.");
    }
    assert_eq(dtor_run, 0, "Expected destructor NOT to have run.");
}
