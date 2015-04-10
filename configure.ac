AC_PREREQ([2.60])

AC_INIT([csptr], [2.0.0], [], [csptr], [franklinmathieu@gmail.com])
AC_CONFIG_SRCDIR([src/mman.c])

LT_PREREQ([2.2.4])
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip foreign subdir-objects parallel-tests color-tests])
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AM_PROG_CC_C_O

AC_CHECK_LIB([check], [srunner_run_all], [], [AC_MSG_NOTICE([Could not find libcheck.])], [])

AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S

AC_PROG_MAKE_SET
AC_SUBST([LIBTOOL_DEPS])

AC_ARG_WITH([fixed-allocator],
    [AS_HELP_STRING([--with-fixed-allocator], [Always use malloc/free as the allocation system])],
    [AC_DEFINE([SMALLOC_FIXED_ALLOCATOR], [1], [Define if malloc should always be used.])],
    [])

AC_ARG_ENABLE([gcov],
  [AS_HELP_STRING([--enable-gcov],
    [Compile the project with converage enabled])],
    [COVERAGE_CFLAGS="-O0 -fprofile-arcs -ftest-coverage"
     COVERAGE_LDFLAGS="-lgcov"
     AC_SUBST([COVERAGE_CFLAGS])
     AC_SUBST([COVERAGE_LDFLAGS])
    ],
    [])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile check/Makefile])

AC_OUTPUT
