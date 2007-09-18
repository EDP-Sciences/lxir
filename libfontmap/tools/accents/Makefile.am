xmldir = $(TEXMF_XML_DIR)
EXTRA_DIST = UnicodeData.txt mkaccents.py
xml_DATA = accents.xml

accents.xml: UnicodeData.txt mkaccents.py
	python $(srcdir)/mkaccents.py $(srcdir)/UnicodeData.txt
