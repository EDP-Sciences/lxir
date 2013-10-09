xmldir = $(TEXMF_XML_DIR)
xml_DATA = config.xml ligatures.xml transformations.xml
EXTRA_DIST = config.xml ligatures.xml transformations.xml

BUILT_SOURCES = cmdline.h
bin_PROGRAMS = lxir
lxir_SOURCES = lxir.c transformations.c utf8string.c log2xml.c entities.c \
	postxml.c table.c \
	utf8string.h log2xml.h transformations.h entities.h
lxir_LDADD = cmdline.o ../libdvi/libdvi.la ../libfontmap/libfontmap.la \
	../libtfm/libtfm.la
lxir_CFLAGS = -Werror $(CFLAGS) $(LIBXML2_CFLAGS) $(LIBXSLT_CFLAGS)  $(ICU_CFLAGS) \
	-I$(srcdir)/../libdvi -I$(srcdir)/../libfontmap -I$(srcdir)/../libtfm
lxir_LDFLAGS = $(AM_LDFLAGS) $(LIBXML2_LIBS) $(LIBXSLT_LIBS) -lkpathsea $(ICU_LIBS)
cmdline.h cmdline.c: $(srcdir)/lxir.ggo
	gengetopt < $<
