#ifndef TEST_H_
# define TEST_H_

#include <check.h>

Suite *unique_ptr_suite(void);

extern TFun scalar_tests[];
extern TFun array_tests[];
extern TFun misc_tests[];
extern TFun shared_tests[];

#endif /* !TEST_H_ */
