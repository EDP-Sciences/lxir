if BUILD_TOOLS
TOOLS = tools
endif

if USE_KPSE
KPSE_CFLAGS = -I$(srcdir)/../libkpathsea/include
endif

xmldir = $(TEXMF_XML_DIR)
dist_xml_DATA = mathfonts.xml zapfdingbats.xml

SUBDIRS = $(TOOLS)
lib_LTLIBRARIES = libfontmap.la
libfontmap_la_SOURCES = libfontmap.c libfontmap.h
libfontmap_la_LIBADD = @LIBXML2_LIBS@
libfontmap_la_CFLAGS = @LIBXML2_CFLAGS@  $(KPSE_CFLAGS)
