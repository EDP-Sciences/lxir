#!/bin/env python
import os, sys, re

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
		raise Exception("Temporary file \"%s\" already exists" % filename)
	return filename

def make_lxir_source(source):
	source = find_source(source)
	temp = temp_source_filename(source)
	s = open(source, "r")
	d = open(temp, "w")
	has_requirepackage = False
	has_documentclass = False
	math_type = 0
	rp = re.compile("\\\\RequirePackage(\[.*\])?{lxir}")
	dc = re.compile("\\\\documentclass(\[.*\])?{.*}")
	for line in s:
		if not has_documentclass:
			if not has_requirepackage and rp.match(line):
				has_requirepackage = True
			if dc.match(line):
				has_documentclass = True
				if not has_requirepackage:
					d.write("\\RequirePackage[verbatimmath]{lxir}\n")
					#~ d.write("\\RequirePackage{lxir}\n")
			d.write(line)
		else:
			index = 0
			while index >= 0:
				p1 = line.find("$", index)
				p2 = line.find("$$", index)
				if p2 >= 0 and (p1 < 0 or p2 <= p1):
					if p2 > index:
						d.write(line[index:p2])
					if math_type == 0:
						d.write("\n\\begin{formule}\n$$")
						math_type = 2
					elif math_type == 2:
						d.write("$$\n\\end{formule}\n")
						math_type = 0
					else:
						raise Exception("Invalid math transition")
					index = p2 + 2
				elif p1 >= 0:
					if p1 > index:
						d.write(line[index:p1])
					if math_type == 0:
						d.write("\n\\begin{formule}\n$")
						math_type = 1
					elif math_type == 1:
						d.write("$\n\\end{formule}\n")
						math_type = 0
					else:
						raise Exception("Invalid math transition")
					index = p1 + 1
				else:
					d.write(line[index:])
					index = -1
	s.close()
	d.close()
	return temp

def compile_latex_source(filename):
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

def main():
	try:
		if len(sys.argv) < 2:
			raise Exception("Usage : %s <source>\n" % sys.argv[0])
		source = make_lxir_source(sys.argv[1])
		dvi = compile_latex_source(source)
		xml = compile_lxir_source(dvi)
		os.unlink(source)
	except Exception, e:
		sys.stderr.write(str(e) + "\n")
		sys.exit(1)

if __name__ == '__main__':
	main()
