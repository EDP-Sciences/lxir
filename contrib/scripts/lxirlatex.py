#!/bin/env python
import os, sys, re

VERSION = "0.2"

translation_map = {}

def find_source(name):
	if os.path.exists(name):
		return name
	base, ext = os.path.splitext(name)
	if ext != '.tex':
		texname = name + '.tex'
		if os.path.exists(texname):
			return texname
	raise Exception("Source file \"%s\" not found" % name)

def temp_source_filename(source):
	name, ext = os.path.splitext(source)
	if ext != '.tex':
		name = source
		ext = '.tex'
	filename = name + "_lxir" + ext
	if os.path.exists(filename):
		if not options.overwrite:
			raise Exception("Temporary file \"%s\" already exists" % filename)
		else:
			os.unlink(filename)
	return filename

def first_special_char(line, index):
	n = -1
	for char in ["$", "{", "}", "%"]:
		m = line.find(char, index)
		if m > -1 and (n == -1 or m < n):
			if char != "%" or m <= 0 or line[m-1] != "\\":
				n = m
	return n

def escape_math(expr):
	expr = expr.replace("!", "!!")
	expr = expr.replace("{", "![")
	expr = expr.replace("}", "!]")
	expr = expr.replace("^", "!*")
	expr = expr.replace("_", "!-")
	expr = expr.replace("\\", "!/")
	return expr

def make_lxir_source(source):
	if options.verbose:
		print("processing file :%s" % source)
	global translation_map
	if translation_map.has_key(source):
		return translation_map[source]
	real_source = find_source(source)
	temp = temp_source_filename(real_source)
	translation_map[source] = temp
	s = open(real_source, "r")
	d = open(temp, "w")
	has_requirepackage = False
	has_documentclass = False
	math_type = 0
	bracket_level = 0
	rp = re.compile("\\\\RequirePackage(\[.*\])?{lxir}")
	dc = re.compile("\\\\documentclass(\[.*\])?{.*}")
	it = re.compile("\\\\input ([a-zA-Z@]+)")
	for line in s:
		m = it.match(line)
		if m:
			start, stop = m.span()
			trans = make_lxir_source(m.group(1))
			base, ext = os.path.splitext(trans)
			if ext != '.tex':
				raise Exception("Invalid translation filename generated \"%s\"" % trans)
			d.write(line[:start] + "\\input " + base + line[stop:])
		elif not has_requirepackage and rp.match(line):
			has_requirepackage = True
			d.write(line)
		elif not has_documentclass and dc.match(line):
			has_documentclass = True
			if not has_requirepackage:
				if not options.verbmath:
					d.write("\\RequirePackage{lxir}\n")
				else:
					d.write("\\RequirePackage[verbatimmath]{lxir}\n")
			d.write(line)
			bracket_level = 0
		elif not has_documentclass or not options.verbmath:
			d.write(line)
		else:
			index = 0
			d.write("%d\t" % bracket_level)
			while index >= 0:
				nindex = first_special_char(line, index)
				if nindex > -1:
					c = line[nindex]
					if c == "{":
						bracket_level += 1
						d.write(line[index:nindex + 1])
						index = nindex + 1
					elif c == "}":
						bracket_level -= 1
						if bracket_level < 0:
							raise Exception("Error in input, too many closing brackets")
						d.write(line[index:nindex + 1])
						index = nindex + 1
					elif c == "%":
						d.write(line[index:]) # keep comments (not in math ?)
						index = -1
					elif c == "$":
						if math_type == 0:
							if line[nindex:nindex+1] == "$$":
								if bracket_level != 0:
									raise Exception("Error in input, $$ inside brackets")
								math_type = 2
								d.write(line[index:nindex] + "\n\\begin{formule}\n$$")
								index = nindex + 2
							else:
								if bracket_level == 0:
									math_type = 1
									d.write(line[index:nindex] + "\n\\begin{formule}\n$")
									index = nindex + 1
								else:
									findex = line.find("$", nindex + 1)
									if findex == -1:
										raise Exception("Error in input, multiline math expression inside brackets")
									d.write(line[index:nindex] + "\\verbatimmathformule{%s}" % escape_math(line[nindex + 1:findex]))
									index = findex + 1
						elif math_type == 1:
								d.write(line[index:nindex] + "$\n\\end{formule}\n")
								math_type = 0
								index = nindex + 1
						elif math_type == 2:
							if line[nindex:nindex+1] == "$$":
								d.write(line[index:nindex] + "$$\n\\end{formule}\n")
								math_type = 0
								index = nindex + 2
							else:
								d.write(line[index:nindex+1])
								index = nindex + 1
				else:
					d.write(line[index:])
					index = -1
	s.close()
	d.close()
	return temp

def compile_latex_source(filename):
	if options.verbose:
		print("Compiling latex source :%s" % filename)
	result = os.system("latex -interaction=batchmode %s" % filename)
	if result != 0:
		raise Exception("latex compilation failed (%d)" % result)
	return os.path.splitext(filename)[0] + '.dvi'

def compile_lxir_source(dvi):
	xml = os.path.splitext(dvi)[0] + '.xhtml'
	result = os.system("lxir %s > %s" % (dvi, xml))
	if result != 0:
		raise Exception("lxir translation failed (%d)" % result)
	return xml

if __name__ == '__main__':
	from optparse import OptionParser
	def main():
		global options
		parser = OptionParser(usage = "%prog [options] <source> [<source> ...]", version = "%prog " + VERSION)
		parser.add_option("-m", "--verbmath", help="Use verbatim math mode", action="store_true")
		parser.add_option("-M", "--not-verbmath", help="Don't use verbatim math mode", action="store_false", dest="verbmath")
		parser.add_option("-v", "--verbose", help="Verbose mode", action="store_true")
		parser.add_option("-q", "-V", "--quiet", "--not-verbose", help="Deactivate verbose mode", action="store_false", dest="verbose")
		parser.add_option("-o", "--overwrite", help="Overwrite temporary files", action="store_true")
		parser.add_option("-O", "--not-overwrite", help="Do not overwrite temporary files", action="store_false", dest="overwrite")
		parser.add_option("-d", "--delete-temp", help="Delete temporary files", action="store_true")
		parser.add_option("-D", "--not-delete-temp", help="Do not delete temporary files", action="store_false", dest="delete_temp")
		parser.set_defaults(overwrite = True, verbmath = True, verbose = False)
		try:
			options, args = parser.parse_args()
			if len(args) < 1:
				parser.error("Need at least one source file")
			for arg in args:
				source = make_lxir_source(arg)
				dvi = compile_latex_source(source)
				xml = compile_lxir_source(dvi)
				if options.delete_temp:
					os.unlink(source)
					os.unlink(dvi)
		except Exception, e:
			sys.stderr.write("Error: " + str(e) + "\n")
			sys.exit(1)
	main()
