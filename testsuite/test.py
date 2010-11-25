#!/usr/bin/python
import os
from subprocess import Popen, PIPE
from Ft.Xml.XPath.Context import Context
from Ft.Xml.XPath import Evaluate
from Ft.Xml.Domlette import NonvalidatingReader, Print
from Ft.Xml import EMPTY_NAMESPACE, XHTML_NAMESPACE
try:
	import cStringIO as StringIO
except ImportError:
	import StringIO
from optparse import OptionParser

__version__ = '0.1'

class Test:
	LATEX_SRC_P = """
\\RequirePackage{lxir}
\\documentclass{article}
\\begin{document}

%s

\\end{document}
"""
	LATEX_CMD_P = "latex %s"
	LXIR_CMD_P = "lxir %s.dvi"
	URI_P = "file://./%s.xml"
	def __init__(self, name, src):
		self.name = name
		self.src = src
	def _make_source(self):
		self.curpath = os.path.abspath(os.curdir)
		self.subdir = os.path.join(self.curpath, self.name)
		os.makedirs(self.subdir)
		os.chdir(self.subdir)
		self.filename = os.path.join(self.subdir, self.name + '.tex')
		f = open(self.filename, 'w')
		f.write(self.LATEX_SRC_P % self.src)
		f.close()
	def _compile_latex(self):
		o = Popen(self.LATEX_CMD_P % self.name, shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE, close_fds=True)
		outdata, errdata = o.communicate()
		returncode = o.wait()
		assert returncode == 0, "LaTeX compilation failed for `%s'" % self.name
	def _compile_lxir(self):
		o = Popen(self.LXIR_CMD_P % self.name, shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE, close_fds=True)
		outdata, errdata = o.communicate()
		returncode = o.wait()
		assert returncode == 0
		self.output = outdata
	def _get_result(self):
		return self.output
	def _cleanup(self):
		os.chdir(self.curpath)
		for file in os.listdir(self.subdir):
			try:
				os.unlink(os.path.join(self.subdir, file))
			except:
				pass
		os.removedirs(self.subdir)
	def __call__(self):
		try:
			self._make_source()
			self._compile_latex()
			self._compile_lxir()
			return self._get_result()
		finally:
			self._cleanup()

MATHML_NAMESPACE = u'http://www.w3.org/1998/Math/MathML'
LXIR_NAMESPACE = u'http://www.latex-lxir.org'

NSS = {
	u'xmlns': u'',
	u'xhtml': XHTML_NAMESPACE,
	u'mml': MATHML_NAMESPACE,
	u'lxir': LXIR_NAMESPACE,
}

class MathMLTest(Test):
	IMAGE_CMD_LIST = [
		"dvips -q {0} -o {0}.ps",
		"ps2eps -P -l -H -f -q {0}.ps",
		"gs -dEPSCrop -dSAFER -dBATCH -dNOPAUSE -r250 -sDEVICE=pngalpha -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -sOutputFile=../{0}.png {0}.eps"
	]
	def __init__(self, name, src):
		Test.__init__(self, name, src)
		self.compile_image = False
	def _compile_image(self):
		for cmd_p in self.IMAGE_CMD_LIST:
			o = Popen(cmd_p.format(self.name), shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE, close_fds=True)
			outdata, errdata = o.communicate()
			returncode = o.wait()
			assert returncode == 0
	def _compile_latex(self):
		Test._compile_latex(self)
		if self.compile_image:
			self._compile_image()
	def _get_result(self):
		doc = NonvalidatingReader.parseString(self.output, self.URI_P % self.name)
		ctxt = Context(doc, processorNss=NSS)
		nodes = Evaluate("//mml:math", context=ctxt)
		assert len(nodes) == 1
		node = nodes[0]
		node.removeAttributeNS(None, u'begin-id')
		node.removeAttributeNS(None, u'end-id')
		o = StringIO.StringIO()
		Print(node, stream = o)
		return o.getvalue()

class TestFolder:
	def __init__(self, path, testclass = Test):
		self.path = path
		self.TestClass = testclass
		self.succeeded = 0
		self.failed = 0
		self.fill_empty = False
		self.print_success = False
		self.print_failure = True
		self.compile_image = False
	def fill(self, name, result):
		filename = os.path.join(self.path, name)
		f = open(filename, 'r')
		src = f.readline()
		f.close()
		f = open(filename, 'w')
		f.write(src)
		f.write(result + '\n')
		f.close()

	def check(self, name, result, check):
		if not check and self.fill_empty:
			self.fill(name, result)
		else:
			if result == check:
				self.success(name)
			else:
				self.failure(name)
	def success(self, name):
		self.succeeded += 1
		if self.print_success:
			print name, "OK"
	def failure(self, name):
		self.failed += 1
		if self.print_failure:
			print name, "FAIL"
	def __call__(self):
		for name in os.listdir(self.path):
			filename = os.path.join(self.path, name)
			if os.path.isfile(filename):
				f = open(filename, 'r')
				src = f.readline()
				check = f.readline()[:-1]
				f.close()
				test = self.TestClass(name, src)
				if self.compile_image:
					test.compile_image = True
				self.check(name, test(), check)
		print("RESULTS: %d succeeded, %d failed" % (self.succeeded, self.failed))
		if self.failed > 0:
			raise SystemExit(1)

def MakeOptionParser(version):
	parser = OptionParser(usage = "%prog [options]", version = "%prog " + version)
	parser.add_option("-v", "--verbose", help="Verbose mode", action="store_true")
	parser.add_option("--fill-test-results", action="store_true")
	parser.add_option("--srcdir", action="store")
	parser.set_defaults(verbose = False, fill_test_results = False)
	return parser

def main():
	parser = MakeOptionParser(__version__)
	options, args = parser.parse_args(sys.argv[1:])
	try:
		if options.srcdir:
			srcdir = options.srcdir
		else:
			srcdir = os.environ['srcdir']
	except KeyError:
		srcdir = os.path.split(__file__)[0]
	f = TestFolder(os.path.join(srcdir, 'lxir.math'), MathMLTest)
	if options.verbose:
		f.print_success = True
	if options.fill_test_results:
		f.fill_empty = True
	f()

if __name__ == '__main__':
	import sys
	main()
