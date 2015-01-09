ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libcsptr.la

WARNINGS = -Wall -Wextra \
	-Wno-unused-result -Wno-missing-field-initializers

libcsptr_la_CFLAGS = $(WARNINGS) -std=gnu11

EXTRA_DIST = LICENSE

subdirincludedir = $(includedir)/csptr/
subdirinclude_HEADERS = \
	src/vararg.h \
	src/apply.h \
	src/mman.h \
	src/smart_ptr.h

libcsptr_la_SOURCES = \
	src/mman.c

man3_MANS = doc/csptr.3
