#include <criterion/criterion.h>
#include <csptr/smart_ptr.h>
#include "utils.h"

Test(scalar, valid_pointer) {
    smart int *a = unique_ptr(int, 42);
    assert_valid_ptr(a);
    assert_eq(get_smart_ptr_meta(a), NULL, "Expected pointer to have no metadata");
}

Test(scalar, dtor_run) {
    int dtor_run = 0;
    f_destructor dtor = lambda(void, (void *ptr, void *meta) { dtor_run = 1; });
    int *a = unique_ptr(int, 42, dtor);
    assert_valid_ptr(a);
    assert_eq(get_smart_ptr_meta(a), NULL, "Expected pointer to have no metadata");
    sfree(a);
    assert(dtor_run, "Expected destructor to run");
}

Test(scalar, meta) {
    smart int *a = unique_ptr(int, 42, .meta = { &m, sizeof (m) });
    assert_valid_ptr(a);
    assert_valid_meta(a, &m, get_smart_ptr_meta(a));
}

Test(scalar, dtor_run_with_meta) {
    int dtor_run = 0;

    f_destructor dtor = lambda(void, (void *ptr, void *meta) { dtor_run = 1; });
    int *a = unique_ptr(int, 42, dtor, { &m, sizeof (m) });
    assert_valid_ptr(a);
    assert_valid_meta(a, &m, get_smart_ptr_meta(a));

    sfree(a);
    assert(dtor_run, "Expected destructor to run");
}
