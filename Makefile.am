ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libcsptr.la

WARNINGS = -Wall -Wextra \
	-Wno-unused-result -Wno-missing-field-initializers

libcsptr_la_CFLAGS = \
	$(WARNINGS) \
	-std=gnu11 \
	-fplan9-extensions

libcsptr_la_LDFLAGS = $(FLAG_WRAP_MALLOC)

EXTRA_DIST = LICENSE README.md

subdirincludedir = $(includedir)/csptr/
subdirinclude_HEADERS = \
	src/vararg.h \
	src/apply.h \
	src/mman.h \
	src/array.h \
	src/smart_ptr.h

libcsptr_la_SOURCES = \
	src/mman.c \
	src/wrap_malloc.c

man3_MANS = doc/csptr.3
