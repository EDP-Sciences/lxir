xmldir = $(TEXMF_XML_DIR)
xml_DATA = config.xml ligatures.xml transformations.xml
EXTRA_DIST = config.xml ligatures.xml transformations.xml
if KPSE_x64
KPSE_LIB_PATH = lib64
else
KPSE_LIB_PATH = lib
endif

if USE_KPSE
KPSE_CFLAGS = -I$(srcdir)/../libkpathsea/include
KPSE_LD_FLAGS = -L$(srcdir)/../libkpathsea/$(KPSE_LIB_PATH) -lkpathsea
endif

bin_PROGRAMS = lxir
lxir_SOURCES = lxir.c transformations.c utf8string.c log2xml.c entities.c postxml.c \
	utf8string.h log2xml.h transformations.h
lxir_LDADD = ../libdvi/libdvi.la ../libfontmap/libfontmap.la
lxir_CFLAGS = $(CFLAGS) $(KPSE_CFLAGS) $(LIBXML2_CFLAGS) $(LIBXSLT_CFLAGS) -I$(srcdir)/../libdvi -I$(srcdir)/../libfontmap
lxir_LDFLAGS = $(AM_LDFLAGS) $(KPSE_LD_FLAGS) $(LIBXML2_LIBS) $(LIBXSLT_LIBS)
