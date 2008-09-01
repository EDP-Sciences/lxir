#!/bin/env python
# -*- coding: utf-8 -*-
import os, sys, subprocess
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
	u'article.cls': "{article}",
	u'aa.cls': "{aa}",
	u'edpsjour.cls': "[vr]{edpsjour}", # FIXME: il faut déterminer la sous-classe. Cette information n'est pour le moment pas taggée.
}

symbolPackages = [
	u'amsmath.sty',
	u'amssymb.sty',
	u'txfonts.sty',
]

mathEnvironments = [
	'equation',
	'multline',
	'eqnarray',
]

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
	if len(name) > 5 and name[-5:] != '_lxir':
		return find_source(name + '_lxir')
	raise Exception("Source file \"%s\" not found" % name)

def remove(name, istemp):
	global tempfiles, options
	if not os.path.exists(name):
		return
	if not options.overwrite:
		raise Exception("Overwrite error : file \"%s\" already exists !" % name)
	os.unlink(name)
	if istemp:
		tempfiles.append(name)

class ImageGenerator:
	def __init__(self, file, className, macros, symbols):
		self.base_path, self.filename = os.path.split(file)
		self.className = className
		self.macros = macros
		self.symbols = symbols
		self.index = 1
		self.images = {}
		self.mathml = {}
		remove(file + ".images-log", True)
		self.log = open(file + ".images-log", "w")

	def system(self, cmd, result):
		self.log.write(">>>>>>>>>>>>>>>> Executing %s\n" % cmd)
		self.log.flush()
		o = subprocess.Popen(cmd, shell=True, stdout=self.log, stderr=subprocess.STDOUT)
		errcode = o.wait()
		self.log.write("<<<<<<<<<<<<<<<< Result : %d, %s is %s \n" % (errcode, result, os.path.exists(result)))
		if errcode != 0 or not os.path.exists(result):
			raise Exception("Image %d of %s failed (while executing '%s' process)" % (self.index, self.filename, cmd))

	def genLaTeXSource(self, formula, file, lxir):
		o = open(file, "w")
		if lxir:
			o.write("\\RequirePackage{lxir}")
		o.write("\\documentclass" + self.className + "\n")
		for symbol in self.symbols:
			o.write("\\usepackage{" + symbol + "}\n")
		o.write("\\pagestyle{empty}\n")
		o.write("\\begin{document}\n")
		for macro in self.macros:
			o.write(macro + "\n")
		o.write(formula + "\n")
		o.write("\\end{document}\n")
		o.close()
	def _makeImage(self, formula):
		global options
		prefix = os.path.join(self.base_path, "img" + str(self.index))

		remove(prefix + ".tex", True)
		remove(prefix + ".aux", True)
		remove(prefix + ".log", True)
		remove(prefix + ".dvi", True)
		remove(prefix + ".ps", True)
		remove(prefix + ".epsi", True)
		remove(prefix + ".png", False)

		self.genLaTeXSource(formula, prefix + ".tex", False)

		self.system("latex -interaction=batchmode " + prefix + ".tex", prefix + ".dvi")
		self.system("dvips -o " + prefix + ".ps " + prefix + ".dvi", prefix + ".ps")
		self.system("ps2ps " + prefix + ".ps " + prefix + ".epsi", prefix + ".epsi")
		if options.ghostscript:
			self.system("gs -dDOINTERPOLATE -dBATCH -dNOPAUSE -dEPSCrop -q -r" + str(options.resolution) + " -sDEVICE=pngalpha -sOutputFile=" + prefix + ".png " + prefix + ".epsi", prefix + ".png")
		else:
			self.system("convert -density 600 " + prefix + ".epsi -resample " + str(options.resolution) + " -trim +repage " + prefix + ".png", prefix + ".png")

		return prefix + ".png"
	def _makeMathML(self, formula):
		prefix = os.path.join(self.base_path, "img" + str(self.index) + "_lxir")
		remove(prefix + ".tex", True)
		remove(prefix + ".aux", True)
		remove(prefix + ".log", True)
		remove(prefix + ".dvi", True)
		remove(prefix + ".xhtml", True)
		self.genLaTeXSource(formula, prefix + ".tex", True)
		self.system("latex -interaction=batchmode " + prefix + ".tex", prefix + ".dvi")
		self.system("lxir " + prefix + ".dvi > " + prefix + ".xhtml", prefix + ".xhtml")
		doc = NonvalidatingReader.parseUri(prefix + ".xhtml")
		ctxt = Context(doc, processorNss=NSS)
		nodes = Evaluate("//mm:math", context=ctxt)
		if len(nodes) == 1:
			formula = nodes[0]
			if formula:
				return formula
		else:
			print "Generation of MathML for formula '%s' produced no output (%d, %d)" % (formula, self.index, len(nodes))
	def makeImage(self, formula):
		if not self.images.has_key(formula):
			self.images[formula] = self._makeImage(formula)
			try:
				self.mathml[formula] = self._makeMathML(formula)
			except:
				print "Generation of MathML for formula '%s' failed" % formula
				self.mathml[formula] = False
			self.index += 1
		return self.images[formula], self.mathml[formula]

def insert_math_images(file):
	file = os.path.abspath(file)
	doc = NonvalidatingReader.parseUri(file)
	ctxt = Context(doc, processorNss=NSS)

	# Check that verbatim math is used
	for node in Evaluate("//xhtml:span[@class='verbatimmath']/@lxir:value", context=ctxt):
		verbatimmath = node.value
	assert(verbatimmath == u'true', "Need verbatim math mode for math conversion")

	# Check that the document class is known
	latexClass = None
	symbols = []
	for node in Evaluate("//xhtml:span[@class='ClassOrPackageUsed']/@lxir:name", context=ctxt):
		if latexClasses.has_key(node.value):
			latexClass = latexClasses[node.value]
		elif node.value in symbolPackages:
			symbols.append(node.value[:-4])
	assert(latexClass, "Unknown document class used")

	# Get All macro text
	macros = []
	for node in Evaluate("//xhtml:span[@class='macro']//text()", context=ctxt):
		macros.append(node.nodeValue)

	gen = ImageGenerator(file, latexClass, macros, symbols)

	for env in mathEnvironments:
		for node in Evaluate("//xhtml:div[@class='" + env + "']", context=ctxt):
			c = Context(node, processorNss=NSS)
			formula = "\\begin{" + env + "}\n"
			for t in Evaluate(".//xhtml:span[@class='formule']//text()", context=c):
				formula += t.nodeValue
				t.parentNode.removeChild(t)
			formula += "\n\\end{" + env + "}"
			image, mathml = gen.makeImage(formula.replace(u'\u02c6', '^').strip())
			# remove the empty text node(s)
			for t in Evaluate(".//xhtml:span[@class='formule']", context=c):
				t.parentNode.removeChild(t)

			img = node.ownerDocument.createElementNS(XHTML_NAMESPACE, "img")
			img.setAttributeNS(XHTML_NAMESPACE, "src", image)
			img.setAttributeNS(XHTML_NAMESPACE, "alt", formula)
			node.appendChild(img)

			if mathml:
				node.appendChild(node.ownerDocument.importNode(mathml, True))

	# Convert All math images
	for node in Evaluate("//xhtml:span[@class='formule']", context=ctxt):
		c = Context(node, processorNss=NSS)
		formula = ""
		for t in Evaluate(".//xhtml:span[@class='text']//text()", context=c):
			formula += t.nodeValue
			t.parentNode.removeChild(t)
		image, mathml = gen.makeImage(formula.replace(u'\u02c6', '^').strip())
		# remove the empty text node(s)
		for t in Evaluate(".//xhtml:span[@class='text']", context=c):
			t.parentNode.removeChild(t)

		img = node.ownerDocument.createElementNS(XHTML_NAMESPACE, "img")
		img.setAttributeNS(XHTML_NAMESPACE, "src", image)
		img.setAttributeNS(XHTML_NAMESPACE, "alt", formula)
		node.appendChild(img)

		if mathml:
			node.appendChild(node.ownerDocument.importNode(mathml, True))

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
		parser.add_option("-r", "--resolution", help="Resolution of the generated images", action="store", type="int")
		parser.add_option("-g", "--ghostscript", help="Use ghostscript instead of imagemagick to convert postscript", action="store_true")
		parser.set_defaults(overwrite = True, verbmath = True, verbose = False, resolution = 92, ghostscript = False)
		try:
			options, args = parser.parse_args(args = args)
			if len(args) < 1:
				parser.error("Need at least one source file")
			for arg in args:
				file = find_source(arg)
				insert_math_images(file)
		except Exception, e:
			sys.stderr.write("Error: " + str(e) + "\n")
			raise
		if options.delete_temp:
			for file in tempfiles:
				os.unlink(file)

	main(sys.argv[1:])
