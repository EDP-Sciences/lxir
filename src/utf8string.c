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
#include <string.h>
#include "utf8string.h"

int utf8_char_len(const char * src) {
	int count = 1;
	int p = *(unsigned char *)src;
	if (p >= 128) {
		int n = 64;
		while(p & n) {
			++count;
			n >>= 1;
		}
	}
	return count;
}

int utf8_copy_char(const char * src, char * dst) {
	int len = utf8_char_len(src);
	memcpy(dst, src, len);
	return len;
}

int utf8_string_len(const char * str) {
	int len = 0;
	while(*str) {
		int l = utf8_char_len(str);
		len += 1;
		str += l;
	}
	return len;
}

int utf8_get_next_char(xmlNodePtr text, xmlChar * output) {
	int len, lchar;
	if (text->content != 0 && (len = strlen((const char *)text->content)) > 0) {
		lchar = utf8_copy_char((const char *)text->content, (char *)output);
		output[lchar] = 0;
		if (len > lchar) {
			xmlNodePtr sibling = text->prev;
			xmlNodePtr rest = xmlNewText(text->content + lchar);
			xmlUnlinkNode(text);
			xmlAddNextSibling(sibling, rest);
		} else {
			xmlUnlinkNode(text);
		}
		xmlFreeNode(text);
		return 0;
	}
	return -1;
}
/*
static int get_family(xmlChar * chr) {
	int ch;
	if(ch >= '0' && ch <= '9') return UTF8_CHAR_FAMILY_NUMBER;
}

int utf8_get_next_family_char(xmlChar * base, int * pfamily, xmlChar ** pelement) {
	int family = 0;
	
}
*/
