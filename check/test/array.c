#include <criterion/criterion.h>
#include <csptr/array.h>
#include "utils.h"

#define ARRAY_SIZE 25

__attribute__((always_inline))
inline void assert_valid_array(void *ptr, size_t len, size_t size) {
    assert_valid_ptr(ptr);
    assert_eq(array_length(ptr), len, "Mismatching array lengths.");
    assert_eq(array_type_size(ptr), size, "Mismatching compound type sizes.");
}

Test(array, creation) {
    smart int *arr = unique_ptr(int[ARRAY_SIZE], {});
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));
}

Test(array, dtor_run) {
    int dtor_run = 0;
    int *arr;
    f_destructor dtor = lambda(void, (void *ptr, void *meta) {
            assert_eq(ptr, arr + dtor_run);
            dtor_run++;
        });

    arr = unique_ptr(int[ARRAY_SIZE], {}, dtor);
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));

    sfree(arr);
    assert_eq(dtor_run, ARRAY_SIZE, "Expected destructor to run");
}

Test(array, meta) {
    smart int *arr = unique_ptr(int[ARRAY_SIZE], {}, .meta = { &m, sizeof (m) });
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));
    assert_valid_meta(arr, &m, array_user_meta(arr));
}

Test(array, dtor_run_with_meta) {
    int dtor_run = 0;
    f_destructor dtor = lambda(void, (void *ptr, void *meta) { dtor_run = 1; });

    int *arr = unique_ptr(int[ARRAY_SIZE], {}, dtor, { &m, sizeof (m) });
    assert_valid_array(arr, ARRAY_SIZE, sizeof (int));
    assert_valid_meta(arr, &m, array_user_meta(arr));

    sfree(arr);
    assert(dtor_run, "Expected destructor to run");
}
