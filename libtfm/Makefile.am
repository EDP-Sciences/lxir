if KPSE_x64
	KPSE_LIB_PATH = lib64
else
	KPSE_LIB_PATH = lib
endif

if USE_KPSE
	KPSE_CFLAGS = -DUSE_KPATHSEA=1 -I$(srcdir)/../libkpathsea/include
	KPSE_LD_FLAGS = -L$(srcdir)/../libkpathsea/$(KPSE_LIB_PATH) -lkpathsea
endif


lib_LTLIBRARIES = libtfm.la
libtfm_la_SOURCES = libtfm.c libtfm.h tfm_internal.h tfm_internal.c
libtfm_la_LIBADD = @LIBXML2_LIBS@
libtfm_la_CFLAGS = @LIBXML2_CFLAGS@

