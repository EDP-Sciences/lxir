#!/bin/env python
# -*- coding: utf-8 -*-
import os, sys, subprocess, traceback
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

def relativePath(basePath, fileName):
	l1, l2 = basePath.split(os.path.sep), fileName.split(os.path.sep)
	while len(l1) >= 1 and len(l2) >= 1 and l1[0] == l2[0]:
		l1, l2 = l1[1:], l2[1:]
	p = ['../' * len(l1)] + l2
	return os.path.join( *p )


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

	def system(self, cmd, result, accept_fail = False):
		self.log.write(">>>>>>>>>>>>>>>> Executing %s\n" % cmd)
		self.log.flush()
		o = subprocess.Popen(cmd, shell=True, stdout=self.log, stderr=subprocess.STDOUT)
		errcode = o.wait()
		self.log.write("<<<<<<<<<<<<<<<< Result : %d, %s is %s \n" % (errcode, result, os.path.exists(result)))
		if errcode != 0 and not accept_fail:
			raise Exception("Image %d of %s failed (%d while executing '%s' process)" % (self.index, self.filename, errcode, cmd))
		if not os.path.exists(result):
			raise Exception("Image %d of %s failed (no output while executing '%s' process)" % (self.index, self.filename, errcode, cmd))

	def genLaTeXSource(self, formula, file, lxir):
		o = open(file, "w")
		if lxir:
			o.write("\\RequirePackage{lxir}")
		o.write("\\documentclass" + self.className + "\n")
		for symbol in self.symbols:
			o.write("\\usepackage{" + symbol + "}\n")
			if symbol == "amsmath" and not lxir:
				o.write("""%% The following force equation numbers NOT to be included in the graphical output
\\makeatletter
\\tag@false
\\let\\tag@true=\\relax
\\st@rredtrue
\\let\\st@rredfalse=\\relax
\\makeatother
""")
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
		imgprefix = os.path.join(self.base_path, options.img_path)
		if not os.path.isdir(imgprefix):
			os.mkdir(imgprefix)
		image = os.path.join(imgprefix, "img" + str(self.index) + ".png")

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
			self.system("gs -dDOINTERPOLATE -dBATCH -dNOPAUSE -dEPSCrop -q -r" + str(options.resolution) + " -sDEVICE=pngalpha -sOutputFile=" + image + " " + prefix + ".epsi", image)
		else:
			self.system("convert -density 600 " + prefix + ".epsi -resample " + str(options.resolution) + " -trim +repage " + image, image)

		return image
	def _makeMathML(self, formula):
		prefix = os.path.join(self.base_path, "img" + str(self.index) + "_lxir")
		remove(prefix + ".tex", True)
		remove(prefix + ".aux", True)
		remove(prefix + ".log", True)
		remove(prefix + ".dvi", True)
		remove(prefix + ".xhtml", True)
		self.genLaTeXSource(formula, prefix + ".tex", True)
		self.system("latex -interaction=batchmode " + prefix + ".tex", prefix + ".dvi", True)
		self.system("lxir " + prefix + ".dvi > " + prefix + ".xhtml", prefix + ".xhtml")
		doc = NonvalidatingReader.parseUri(prefix + ".xhtml")
		ctxt = Context(doc, processorNss=NSS)
		nodes = Evaluate("//mm:math", context=ctxt)
		if len(nodes) == 1:
			formula = nodes[0]
			if formula:
				return formula
		elif len(nodes) == 3 and formula.find('eqnarray'):
			formula = nodes[0]
			if formula:
				return formula
		else:
			nodes = Evaluate("//xhtml:span[@class='msub' or @class='msup']", context=ctxt)
			if len(nodes) == 1:
				formula = nodes[0]
				if formula:
					print "Found simple math expression for formula %d" % self.index
					return formula
			else:
				print "Generation of MathML for formula %d produced %d output(s)" % (self.index, len(nodes))
	def makeImage(self, formula):
		if not self.images.has_key(formula):
			try:
				img = self._makeImage(formula)
				self.images[formula] = relativePath(self.base_path, img)
			except:
				print "Generation of Image for formula '%s' failed: %s" % (self.index, traceback.format_exc())
				self.images[formula] = False
			try:
				self.mathml[formula] = self._makeMathML(formula)
			except:
				print "Generation of MathML for formula '%s' failed: %s" % (self.index, traceback.format_exc())
				self.mathml[formula] = False
			self.index += 1
		return self.images[formula], self.mathml[formula]

def get_prev_span_node(node):
	prev = node.previousSibling
	if prev.nodeType == node.TEXT_NODE and prev.nodeValue.strip() == "":
		prev = prev.previousSibling
	assert prev.nodeType == node.ELEMENT_NODE and prev.tagName == 'span'
	tnode = prev.firstChild
	assert tnode.nodeType == node.TEXT_NODE
	text = tnode.nodeValue.strip()
	pos = text.rfind(" ") + 1
	if pos == 0:
		return prev

	doc = node.ownerDocument
	prev.replaceChild(doc.createTextNode(text[:pos]), tnode)
	n = prev.cloneNode(False)
	n.appendChild(doc.createTextNode(text[pos:]))
	return n

def insert_math_images(file):
	file = os.path.abspath(file)
	doc = NonvalidatingReader.parseUri(file)
	ctxt = Context(doc, processorNss=NSS)

	# Check that verbatim math is used
	for node in Evaluate("//xhtml:span[@class='verbatimmath']/@lxir:value", context=ctxt):
		verbatimmath = node.value
	assert verbatimmath == u'true', "Need verbatim math mode for math conversion"

	# Check that the document class is known
	latexClass = None
	symbols = []
	for node in Evaluate("//xhtml:span[@class='ClassOrPackageUsed']/@lxir:name", context=ctxt):
		if latexClasses.has_key(node.value):
			latexClass = latexClasses[node.value]
		elif node.value in symbolPackages:
			symbols.append(node.value[:-4])
	assert latexClass, "Unknown document class used"

	# Get All macro text
	macros = []
	for node in Evaluate("//xhtml:span[@class='macro']//text()", context=ctxt):
		macros.append(node.nodeValue)

	gen = ImageGenerator(file, latexClass, macros, symbols)

	# Convert All math images
	for node in Evaluate("//xhtml:span[@class='formule']", context=ctxt):
		c = Context(node, processorNss=NSS)
		formula = ""
		for t in Evaluate("xhtml:span[@class='text']/text()", context=c):
			formula += t.nodeValue
		formula = formula.strip()
		if len(formula) > 1 and formula[0] != "$":
			p = node.parentNode
			env = p.getAttributeNS(None, 'class')
			assert env, "No env found for equation"
			if env[-5:] == "-star":
				env = env[:-5]+"*"
			if len(env) > 0:
				formula = "\\begin{" + env + "}\n" + formula + "\n\\end{" + env + "}"
		image, mathml = gen.makeImage(formula)
		# remove the empty text node(s)
		for t in Evaluate("xhtml:span[@class='text']", context=c):
			t.parentNode.removeChild(t)

		if image:
			img = node.ownerDocument.createElementNS(XHTML_NAMESPACE, "img")
			img.setAttributeNS(XHTML_NAMESPACE, "src", image)
			img.setAttributeNS(XHTML_NAMESPACE, "alt", formula)
			node.appendChild(img)

		if mathml:
			if mathml.tagName != 'math':
				# here, we have the case : a$_b$
				# mathml is: <span class='msub'><span class='mi'/><span>b</span></span>
				# original xml is : ... <span>a</span><span class="formula"> ...</span>
				# and node is the formula
				# p is <span>a</span>
				p = get_prev_span_node(node)
				newNode = node.parentNode.insertBefore(mathml.cloneNode(True), node)
				newNode.firstChild.appendChild(p)
				node.parentNode.removeChild(node)
				print "Formula '%s' replaced by simple form (%s)." % (formula, newNode.tagName + '.' + newNode.getAttributeNS(None, u'class'))
			else:
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
		parser.add_option("-i", "--img-path", help="Set image path", action="store", type="string")
		parser.set_defaults(overwrite = True, verbmath = True, verbose = False, resolution = 92, ghostscript = False, img_path = 'images_math')
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
