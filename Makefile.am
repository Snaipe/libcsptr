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

libcsptr_la_LDFLAGS = $(COVERAGE_LDFLAGS) -version-info 1:0:0

EXTRA_DIST = LICENSE README.md

subdirincludedir = $(includedir)/csptr/
subdirinclude_HEADERS = \
	include/csptr/config.h \
	include/csptr/smalloc.h \
	include/csptr/array.h \
	include/csptr/smart_ptr.h

libcsptr_la_SOURCES = \
	src/mman.h \
	src/mman.c

dist_man3_MANS = doc/csptr.3
