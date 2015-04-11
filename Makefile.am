ACLOCAL_AMFLAGS = -I m4

if ENABLE_TESTS
SUBDIRS = check
endif

lib_LTLIBRARIES = libcsptr.la

WARNINGS = -Wall -Wextra \
	-Wno-unused-result -Wno-missing-field-initializers

libcsptr_la_CFLAGS = \
	$(WARNINGS) \
	-std=gnu11 \
	-fplan9-extensions \
	-I$(top_srcdir)/include/csptr/ \
	$(COVERAGE_CFLAGS)

libcsptr_la_LDFLAGS = $(FLAG_WRAP_MALLOC) $(COVERAGE_LDFLAGS)

EXTRA_DIST = LICENSE README.md

subdirincludedir = $(includedir)/csptr/
subdirinclude_HEADERS = \
	include/csptr/smalloc.h \
	include/csptr/array.h \
	include/csptr/smart_ptr.h

libcsptr_la_SOURCES = \
	src/mman.h \
	src/mman.c

dist_man3_MANS = doc/csptr.3
