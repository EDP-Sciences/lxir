
if BUILD_TOOLS
	DVIDUMP = dvidump
endif

noinst_PROGRAMS = $(DVIDUMP)
lib_LTLIBRARIES = libdvi.la
libdvi_la_SOURCES = libdvi.c libdvi.h
libdvi_la_LIBADD = @LIBXML2_LIBS@
libdvi_la_CFLAGS = @LIBXML2_CFLAGS@ -I$(srcdir)/../libfontmap \
	-I$(srcdir)/../libtfm
EXTRA_PROGRAMS = dvidump

dvidump_SOURCES = dvidump.c
dvidump_CFLAGS =
dvidump_LDFLAGS = $(AM_LDFLAGS)
dvidump_CFLAGS = -I$(srcdir)/../libfontmap -I$(srcdir)/../libtfm
dvidump_LDADD = libdvi.la ../libfontmap/libfontmap.la ../libtfm/libtfm.la -lkpathsea

