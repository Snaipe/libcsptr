#ifndef TEST_H_
# define TEST_H_

#include <check.h>

Suite *unique_ptr_suite(void);

TCase *make_scalar_tests(void);
TCase *make_array_tests(void);
TCase *make_misc_tests(void);
TCase *make_shared_tests(void);

#endif /* !TEST_H_ */
