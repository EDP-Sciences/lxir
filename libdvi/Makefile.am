if KPSE_x64
	KPSE_LIB_PATH = lib64
else
	KPSE_LIB_PATH = lib
endif

if USE_KPSE
	KPSE_CFLAGS = -DUSE_KPATHSEA=1 -I$(srcdir)/../libkpathsea/include
	KPSE_LD_FLAGS = -L$(srcdir)/../libkpathsea/$(KPSE_LIB_PATH) -lkpathsea
endif

if BUILD_TOOLS
	DVIDUMP = dvidump
endif

noinst_PROGRAMS = $(DVIDUMP)
lib_LTLIBRARIES = libdvi.la
libdvi_la_SOURCES = libdvi.c libdvi.h
libdvi_la_LIBADD = @LIBXML2_LIBS@
libdvi_la_CFLAGS = @LIBXML2_CFLAGS@
EXTRA_PROGRAMS = dvidump

dvidump_SOURCES = dvidump.c
dvidump_CFLAGS= $(KPSE_CFLAGS)
dvidump_LDFLAGS = $(AM_LDFLAGS) $(KPSE_LD_FLAGS)
dvidump_LDADD = libdvi.la ../libfontmap/libfontmap.la

