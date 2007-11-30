DISTCHECK_CONFIGURE_FLAGS=--with-tools --without-kpse
if BUILD_TOOLS
TOOLS_DISTCLEANFILES = \
	$(builddir)/libfontmap/tools/accents/accents.xml \
	$(builddir)/libfontmap/tools/enc2tbl/encodings.xml \
	$(builddir)/libfontmap/tools/enc2tbl/characters.xml \
	$(builddir)/libfontmap/tools/tfmlist/fonts.xml
endif
DISTCLEANFILES = $(TOOLS_DISTCLEANFILES)

if USE_KPSE
KPSE_PATH = libkpathsea
endif

SUBDIRS = $(KPSE_PATH) libfontmap libdvi src texmf
EXTRA_DIST = lppl.txt gpl-3.0.txt

if USE_KPSE
MKTEXLSR = `which mktexlsr`
install-data-hook:
	$(MKTEXLSR) $(TEXMF_DEST_DIR)
endif
