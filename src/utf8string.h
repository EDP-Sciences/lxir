/*
lxir - a tool to convert latex documents to xml.
Copyright (C) 2007  EDP-Sciences

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef UTF8_STRING_H
#define UTF8_STRING_H 1

#include <libxml/tree.h>

int utf8_is_accented_char(const char *);
int utf8_char_len(const char *);
int utf8_copy_char(const char *, char *);
int utf8_string_len(const char *);
int utf8_get_next_char(xmlNodePtr, xmlChar *);

#endif
