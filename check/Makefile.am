TESTS = check_unit
check_PROGRAMS = check_unit
check_unit_CFLAGS = \
	-std=gnu99 \
	-I$(top_srcdir)/include/ \
	$(COVERAGE_CFLAGS)

check_unit_LDFLAGS = $(COVERAGE_LDFLAGS)
check_unit_LDADD = $(top_builddir)/libcsptr.la $(CHECKLIBS)
check_unit_SOURCES = \
	test/utils.h \
	test/utils.c \
	test/scalar.c \
	test/misc.c \
	test/shared.c \
	test/array.c
