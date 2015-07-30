include_directories(../include)

set(TEST_SOURCES
  test/test.c
  test/test.h
  test/utils.h
  test/scalar.c
  test/misc.c
  test/shared.c
  test/array.c
)

add_executable(check_unit ${TEST_SOURCES})
target_link_libraries(check_unit ${CHECK_LIBRARIES} csptr)

add_test(check_unit check_unit)
