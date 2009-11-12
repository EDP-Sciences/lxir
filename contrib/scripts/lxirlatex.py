#!/bin/env python
import os, sys, re, subprocess

VERSION = "0.3"

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
	for char in ["$", "{", "}", "%", "[", "]"]:
		m = line.find(char, index)
		if m > -1 and (n == -1 or m < n):
			if (char != "%" and char != "$") or m <= 0 or line[m-1] != "\\":
				n = m
	return n

def escape_math(expr):
	expr = expr.replace("!", "!!")
	expr = expr.replace("{", "![")
	expr = expr.replace("}", "!]")
	expr = expr.replace("^", "!*")
	expr = expr.replace("_", "!-")
	expr = expr.replace("\\", "!/")
	expr = expr.replace("\n", "!n")
	expr = expr.replace("%", "!p")
	expr = expr.replace("#", "!s")
	expr = expr.replace("~", "!t")
	return expr

def fix_tex_line(line):
	line = line.replace("\\[", "$$")
	line = line.replace("\\]", "$$")
	line = line.replace("\\(", "$$")
	line = line.replace("\\)", "$$")
	return line

def find_end_of_macro(content, start):
	pos = start
	count = 1
	opened = content.find("{", pos)
	closed = content.find("}", pos)
	while count > 0 and opened > 0 and closed > 0:
		if opened < closed:
			count += 1
			pos = opened + 1
			opened = content.find("{", pos)
		else:
			count -= 1
			pos = closed + 1
			closed = content.find("}", pos)
	assert count == 0, "Error: unable to find matching brackets in macro : "+content[:start]
	return pos

def extract_macros(source):
	macros = []
	s = open(find_source(source), "r")
	content = s.read()
	s.close()

	rdef = re.compile("\\\\def[^{]*{")
	rcmd = re.compile("\\\\newcommand\\s*{[^}]*}\\s*(\\[[^]]+\\])?\\s*{")
	rrcmd = re.compile("\\\\renewcommand\\s*{[^}]*}\\s*(\\[[^]]+\\])?\\s*{")

	while content:
		start, stop = len(content), -1
		m = rdef.search(content)
		if m:
			start, stop = m.span()
		m = rcmd.search(content)
		if m:
			ostart, ostop = m.span()
			if ostart < start:
				start, stop = ostart, ostop
		m = rrcmd.search(content)
		if m:
			ostart, ostop = m.span()
			if ostart < start:
				start, stop = ostart, ostop
		if stop >= 0:
			stop = find_end_of_macro(content, stop)
			macros.append(content[start:stop])
			content = content[stop+1:]
		else:
			content = None
	return macros

def make_lxir_source(source, macros):
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
	has_inserted_macros = False
	math_type = 0
	bracket_level = 0
	rp = re.compile("\\\\RequirePackage(\[.*\])?{lxir}")
	dc = re.compile("\\\\documentclass(\[.*\])?{.*}")
	it = re.compile("\\\\input ([a-zA-Z@]+)")
	line_count = 0
	for line in s:
		line = fix_tex_line(line)
		line_count += 1
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
			if not has_inserted_macros:
				for macro in macros:
					d.write("\\verbatimmacro{%s}\n" % escape_math(macro))
				has_inserted_macros = True
			index = 0
			while index >= 0:
				nindex = first_special_char(line, index)
				if nindex > -1:
					c = line[nindex]
					if c == "{" or c == "[":
						bracket_level += 1
						d.write(line[index:nindex + 1])
						index = nindex + 1
					elif c == "}" or c == "]":
						bracket_level -= 1
						if bracket_level < 0:
							# raise Exception("too many closing brackets at line %d" % line_count)
							bracket_level = 0
						d.write(line[index:nindex + 1])
						index = nindex + 1
					elif c == "%":
						d.write(line[index:]) # keep comments (not in math ?)
						index = -1
					elif c == "$":
						if math_type == 0:
							if line[nindex:nindex+2] == "$$":
								if bracket_level != 0:
									raise Exception("Blockmode math ($$) inside brackets")
								math_type = 2
								d.write(line[index:nindex] + "\n\\begin{formule}\n$$")
								index = nindex + 2
							else:
								if bracket_level == 0:
									math_type = 1
									d.write(line[index:nindex] + "\\begin{formule}$")
									index = nindex + 1
								else:
									findex = line.find("$", nindex + 1)
									if findex == -1:
										raise Exception("Multiline math expression inside brackets at line %d" % line_count)
									d.write(line[index:nindex] + "\\verbatimmathformule{%s}" % escape_math(line[nindex + 1:findex]))
									index = findex + 1
						elif math_type == 1:
								d.write(line[index:nindex] + "$\\end{formule}\n")
								math_type = 0
								index = nindex + 1
								if line[index] == ' ':
									index += 1
									d.write("[entity!\#x20!]")
								elif line[index] == '\n':
									index = -1
						elif math_type == 2:
							if line[nindex:nindex+2] == "$$":
								d.write(line[index:nindex] + "$$\n\\end{formule}\n")
								math_type = 0
								index = nindex + 2
								if line[index] == '\n':
									index = -1
							else:
								d.write(line[index:nindex+1])
								index = nindex + 1
				else:
					d.write(line[index:])
					index = -1
	s.close()
	d.close()
	return temp

def system(cmd, result, log):
	log.write(">>>>>>>>>>>>>>>> Executing %s\n" % cmd)
	log.flush()
	o = subprocess.Popen(cmd, shell=True, stdout=log, stderr=subprocess.STDOUT)
	errcode = o.wait()
	log.write("<<<<<<<<<<<<<<<< Result : %d, %s is %s \n" % (errcode, result, os.path.exists(result)))
	assert os.path.exists(result)
	return errcode

def compile_latex_source(filename, log):
	if options.verbose:
		print("Compiling latex source :%s" % filename)
	output = os.path.splitext(filename)[0] + '.dvi'
	system("latex -interaction=batchmode %s" % filename, output, log)
	return output

def compile_lxir_source(dvi, log):
	xml = os.path.splitext(dvi)[0] + '.xhtml'
	result = system("lxir %s > %s" % (dvi, xml), xml, log)
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
				log = open(arg + ".lxir-log", "w")
				macros = extract_macros(arg)
				source = make_lxir_source(arg, macros)
				dvi = compile_latex_source(source, log)
				xml = compile_lxir_source(dvi, log)
				log.close()
				if options.delete_temp:
					os.unlink(source)
					os.unlink(dvi)
		except Exception, e:
			sys.stderr.write("Error: " + str(e) + "\n")
			raise
	main()
