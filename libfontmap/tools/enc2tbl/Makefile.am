xmldir = $(TEXMF_XML_DIR)
AM_YFLAGS = -d
noinst_PROGRAMS = enc2tbl mkcharxml
EXTRA_DIST = \
enc/6w.enc             \
enc/8a.enc	       \
enc/8r.enc	       \
enc/amssymba.enc       \
enc/amssymbb.enc       \
enc/blackboard.enc     \
enc/CB.enc	       \
enc/cm-super-t1.enc    \
enc/cm-super-t2a.enc   \
enc/cm-super-t2b.enc   \
enc/cm-super-t2c.enc   \
enc/cm-super-ts1.enc   \
enc/cm-super-x2.enc    \
enc/cork.enc	       \
enc/dummy.enc	       \
enc/euex.enc	       \
enc/eufrak.enc	       \
enc/eurosym.enc	       \
enc/gkerkisc.enc       \
enc/gkerkis.enc	       \
enc/gpkerkisc.enc      \
enc/gpkerkis.enc       \
enc/kerkisc.enc	       \
enc/kerkisec.enc       \
enc/kerkisecsc.enc     \
enc/kerkis.enc	       \
enc/kmath.enc	       \
enc/kmex.enc	       \
enc/kmsym.enc	       \
enc/latexsym.enc       \
enc/lxir_default.enc   \
enc/marvo00.enc	       \
enc/marvo98.enc	       \
enc/ot1.enc	       \
enc/ot1wofl.enc	       \
enc/t5d.enc	       \
enc/t5uni.enc	       \
enc/texmext.enc	       \
enc/texmital.enc       \
enc/texmsym.enc	       \
enc/texnansi.enc       \
enc/textcomp.enc       \
enc/textt.enc              

xml_DATA = encodings.xml
enc2tbl_SOURCES = main.c grammar.y parser.l ../common/find_in_path.c enc2tbl.h
enc2tbl_LDADD = @LIBXML2_LIBS@
enc2tbl_CFLAGS = @LIBXML2_CFLAGS@
enc2tbl_DEPENDENCIES = characters.xml

mkcharxml_SOURCES = mkcharxml.c adobetbl.h
mkcharxml_LDADD = @LIBXML2_LIBS@
mkcharxml_CFLAGS = @LIBXML2_CFLAGS@

encodings.xml: enc2tbl
	./enc2tbl $(srcdir)/enc > $@


characters.xml: mkcharxml
	./mkcharxml > $@
