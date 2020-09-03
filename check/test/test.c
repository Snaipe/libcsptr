#include <stdlib.h>
#include <check.h>
#include "csptr/smart_ptr.h"

#include "test.h"
#include "utils.h"

const struct meta m = {1, 2, 3};

Suite *master_suite(void) {
    Suite *s = suite_create("master");
    suite_add_tcase(s, make_misc_tests());
    suite_add_tcase(s, make_scalar_tests());
    suite_add_tcase(s, make_array_tests());
    suite_add_tcase(s, make_shared_tests());
    return s;
}

int main(void) {
    SRunner *sr = srunner_create(master_suite());

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
