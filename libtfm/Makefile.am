if USE_KPSE
KPSE_CFLAGS = -DUSE_KPATHSEA=1 -I$(srcdir)/../libkpathsea/include
endif


lib_LTLIBRARIES = libtfm.la
libtfm_la_SOURCES = libtfm.c libtfm.h tfm_internal.h tfm_internal.c
libtfm_la_CFLAGS = $(KPSE_CFLAGS)

