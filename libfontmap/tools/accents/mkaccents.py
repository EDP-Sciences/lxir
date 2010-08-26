#!/usr/bin/env python
# -*- encoding: utf-8 -*-
## lxir - a tool to convert latex documents to xml.
## Copyright (C) 2007  EDP-Sciences
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
import unicodedata
import sys

def tochar(number):
	s = chr(int(number[2:], 16))+chr(int(number[:2], 16))
	return s.decode('UTF-16')

def escape(code):
	if code == u">":
		return "&gt;"
	elif code == u"<":
		return "&lt;"
	else:
		return code.encode("UTF-8")

def find_chars(char):
	res = [char]
	if char == u"ı":
		res.append(u"i")
	elif char == u"i":
		res.append(u"ı")
	return res

EXTRA_ACCENT_NAMES = {
	'ACUTE ACCENT': ('APOSTROPHE', 'MODIFIER LETTER PRIME', 'GREEK TONOS'),
	'TILDE': ( 'SMALL TILDE',),
	'RING ABOVE': ('DEGREE SIGN',),
	'DOUBLE ACUTE ACCENT': ('QUOTATION MARK', 'MODIFIER LETTER DOUBLE PRIME',),
	'VERTICAL LINE ABOVE': ('MODIFIER LETTER VERTICAL LINE',),
	# CANDRABINDU
	#~ 'TURNED COMMA ABOVE': ('MODIFIER LETTER TURNED COMMA',),
	#~ 'COMMA ABOVE': ('MODIFIER LETTER APOSTROPHE', 'ARMENIAN APOSTROPHE',),
	#~ 'REVERSED COMMA ABOVE': ('MODIFIER LETTER REVERSED COMMA', 'ARMENIAN MODIFIER LETTER HALF RING'),
}

def find_accents(accent):
	res = [accent]
	comb, name = unicodedata.name(accent).split(' ', 1)
	if comb == "COMBINING":
		try:
			res.append(unicodedata.lookup("MODIFIER LETTER "+name))
		except:
			pass
		try:
			res.append(unicodedata.lookup(name))
		except:
			pass
		if EXTRA_ACCENT_NAMES.has_key(name):
			for extra in EXTRA_ACCENT_NAMES[name]:
				try:
					res.append(unicodedata.lookup(extra))
				except:
					pass
	return res

def write_accent(f, s):
	try:
		char = s.split(';')[0]
		if len(char) > 4:
			return
		try:
			char = tochar(char)
		except UnicodeError:
			return
		accent = unicodedata.decomposition(char)
		if len(accent) != 9:
			return
		base, accent = accent.split(' ')

		bases = find_chars(tochar(base))
		accents = find_accents(tochar(accent))

		for base in bases:
			for accent in accents:
				f.write("\t<accent base=\"%s\" accent=\"%s\" char=\"%s\" />\n" %
					(escape(base), escape(accent), escape(char)))

	except Exception, d:
		print "Error writing accent", s, d


f = open(sys.argv[1], "r")
lines = f.readlines()
f.close()

o = open("accents.xml", "w")
o.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
o.write("<encodings>\n")
for line in lines:
	write_accent(o, line[:-1])
o.write("</encodings>\n")

