AUTOMAKE_OPTIONS = dejagnu
DISTCHECK_CONFIGURE_FLAGS=--with-tools --without-kpse
ACLOCAL_AMFLAGS=-I m4

if BUILD_TOOLS
TOOLS_DISTCLEANFILES = \
	$(builddir)/libfontmap/tools/accents/accents.xml \
	$(builddir)/libfontmap/tools/enc2tbl/encodings.xml \
	$(builddir)/libfontmap/tools/enc2tbl/characters.xml \
	$(builddir)/libfontmap/tools/tfmlist/fonts.xml
endif
DISTCLEANFILES = $(TOOLS_DISTCLEANFILES)


SUBDIRS = libfontmap libdvi libtfm src texmf
EXTRA_DIST = lppl.txt gpl-3.0.txt

MKTEXLSR = `which mktexlsr`
install-data-hook:
	$(MKTEXLSR) $(TEXMF_DEST_DIR)

RUNTESTDEFAULTFLAGS = --tool lxir LXIR=$(builddir)/src/lxir --srcdir $$srcdir/testsuite
