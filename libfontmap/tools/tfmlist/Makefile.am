xmldir = $(TEXMF_XML_DIR)
noinst_PROGRAMS = tfmlist
EXTRA_DIST = patterns.xml
xml_DATA = fonts.xml
tfmlist_SOURCES = tfmlist.c ../common/find_in_path.c
tfmlist_LDADD = @LIBXML2_LIBS@
tfmlist_CFLAGS = @LIBXML2_CFLAGS@

fonts.xml: tfmlist
	./tfmlist $(srcdir)/patterns.xml /usr/share/texmf/fonts/tfm /var/lib/texmf/tfm $(TEXMF_DEST_DIR)/fonts/tfm $(TFM_SEARCH_PATH) > $@
