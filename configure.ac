AC_PREREQ([2.60])

AC_INIT([csptr], [1.0], [], [csptr], [franklinmathieu@gmail.com])
AC_CONFIG_SRCDIR([src/mman.c])

LT_PREREQ([2.2.4])
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip foreign subdir-objects])
LT_INIT

AC_PROG_CC
AM_PROG_CC_C_O

AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S

AC_PROG_MAKE_SET
AC_SUBST([LIBTOOL_DEPS])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
