TESTS = check_unit
check_PROGRAMS = check_unit
check_unit_CFLAGS = -std=gnu99 @CHECK_CFLAGS@
check_unit_LDADD = $(top_builddir)/libcsptr.la @CHECK_LIBS@
check_unit_LDFLAGS = -ldl

check_unit_SOURCES = \
	test/test.c \
	test/scalar.c \
	test/array.c
