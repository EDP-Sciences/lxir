#!/bin/env python
# -*- coding: utf-8 -*-
import os, sys
from Ft.Xml.XPath.Context import Context
from Ft.Xml.XPath import Evaluate
from Ft.Xml.Domlette import NonvalidatingReader, Print
from Ft.Xml import EMPTY_NAMESPACE, XHTML_NAMESPACE

NSS = {
	u'xhtml': XHTML_NAMESPACE,
	u'mm': u'http://www.w3.org/1998/Math/MathML',
	u'lxir': u'http://www.latex-lxir.org',
}

VERSION = "0.1"

latexClasses = {
	u'article.cls': "article",
}

tempfiles = []

def find_source(name):
	if os.path.exists(name):
		return name
	base, ext = os.path.splitext(name)
	if ext != '.xhtml':
		texname = name + '.xhtml'
		if os.path.exists(texname):
			return texname
	if ext != '.html':
		texname = name + '.html'
		if os.path.exists(texname):
			return texname
	raise Exception("Source file \"%s\" not found" % name)

def remove(name, istemp):
	global tempfiles
	if not os.path.exists(name):
		return
	if not options.overwrite:
		raise Exception("Overwrite error : file \"%s\" already exists !" % name)
	os.unlink(name)
	if istemp:
		tempfiles.append(name)

class ImageGenerator:
	def __init__(self, file, className, macros):
		self.base_path = os.path.split(file)[0]
		self.className = className
		self.macros = macros
		self.index = 1
	
	def genLaTeXSource(self, formula, file):
		o = open(file, "w")
		o.write("\\documentclass{" + self.className + "}\n")
		o.write("\\pagestyle{empty}\n")
		o.write("\\begin{document}\n")
		for macro in self.macros:
			o.write(macro + "\n")
		o.write(formula + "\n")
		o.write("\\end{document}\n")
		o.close()
	def makeImage(self, formula):
		prefix = os.path.join(self.base_path, "img" + str(self.index))
		self.index += 1

		remove(prefix + ".tex", True)
		remove(prefix + ".aux", True)
		remove(prefix + ".log", True)
		remove(prefix + ".dvi", True)
		remove(prefix + ".ps", True)
		remove(prefix + ".epsi", True)
		remove(prefix + ".png", False)

		self.genLaTeXSource(formula, prefix + ".tex")

		os.system("latex " + prefix + ".tex")
		os.system("dvips -o " + prefix + ".ps " + prefix + ".dvi")
		os.system("ps2epsi " + prefix + ".ps " + prefix + ".epsi")
		os.system("gs -dDOINTERPOLATE -dBATCH -dNOPAUSE -dEPSCrop -q -r92 -sDEVICE=pngalpha -sOutputFile=" + prefix + ".png " + prefix + ".epsi")
		
		return prefix + ".png"

def insert_math_images(file):
	file = os.path.abspath(file)
	doc = NonvalidatingReader.parseUri(file)
	ctxt = Context(doc, processorNss=NSS)
	
	# Check that verbatim math is used
	for node in Evaluate("//xhtml:span[@class='verbatimmath']/@lxir:value", context=ctxt):
		verbatimmath = node.value
	assert(verbatimmath == u'true', "Need verbatim math mode for math conversion")
	
	# Check that the document class is known
	for node in Evaluate("//xhtml:span[@class='ClassOrPackageUsed']/@lxir:name", context=ctxt):
		latexClass = node.value
	assert(latexClasses[latexClass], "Unknown document class used")
	
	# Get All macro text
	macros = []
	for node in Evaluate("//xhtml:span[@class='macro']//text()", context=ctxt):
		macros.append(node.nodeValue)
	
	gen = ImageGenerator(file, latexClasses[latexClass], macros)
	
	# Convert All math images
	for node in Evaluate("//xhtml:span[@class='formule']", context=ctxt):
		c = Context(node, processorNss=NSS)
		formula = ""
		for t in Evaluate(".//xhtml:span[@class='text']//text()", context=c):
			formula += t.nodeValue
			t.parentNode.removeChild(t)
		image = gen.makeImage(formula.replace(u'\u02c6', '^').encode('ascii', 'replace'))
		
		img = node.ownerDocument.createElementNS(XHTML_NAMESPACE, "img")
		img.setAttributeNS(XHTML_NAMESPACE, "src", image)
		img.setAttributeNS(XHTML_NAMESPACE, "alt", formula)
		node.appendChild(img)
	
	base, ext = os.path.splitext(file)
	output = base + "_images" + ext
	o = open(output, "w")
	Print(doc, stream = o)
	o.close()

if __name__ == '__main__':
	from optparse import OptionParser
	def main(args):
		global options, tempfiles
		parser = OptionParser(usage = "%prog [options] <source> [<source> ...]", version = "%prog " + VERSION)
		parser.add_option("-v", "--verbose", help="Verbose mode", action="store_true")
		parser.add_option("-q", "-V", "--quiet", "--not-verbose", help="Deactivate verbose mode", action="store_false", dest="verbose")
		parser.add_option("-o", "--overwrite", help="Overwrite temporary files", action="store_true")
		parser.add_option("-O", "--not-overwrite", help="Do not overwrite temporary files", action="store_false", dest="overwrite")
		parser.add_option("-d", "--delete-temp", help="Delete temporary files", action="store_true")
		parser.add_option("-D", "--not-delete-temp", help="Do not delete temporary files", action="store_false", dest="delete_temp")
		parser.set_defaults(overwrite = True, verbmath = True, verbose = False)
		try:
			options, args = parser.parse_args(args = args)
			if len(args) < 1:
				parser.error("Need at least one source file")
			for arg in args:
				file = find_source(arg)
				insert_math_images(file)
		except Exception, e:
			sys.stderr.write("Error: " + str(e) + "\n")
			sys.exit(1)
		if options.delete_temp:
			for file in tempfiles:
				os.unlink(file)
	
	main(sys.argv[1:])
