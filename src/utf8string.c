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

#include <unicode/utf8.h>
#include <unicode/unorm2.h>


int utf8_is_accented_char(const char * src) {
	UChar32 c;
	UChar decomposition[16];
	int32_t length, i;
	UErrorCode error = U_ZERO_ERROR;
	const UNormalizer2 * nfd;

	U8_GET((const uint8_t *) src, 0, 0, 16, c);
	if (c < 0) {
		fprintf(stderr, "Unable to extract codepoint from src (%s).\n", src);
		return -1;
	}
	//~ fprintf(stderr, "Testing if U-%04X (%s) is accented...", c, src);

	nfd = unorm2_getInstance(NULL, "nfc", UNORM2_DECOMPOSE, &error);
	if (U_FAILURE(error)) {
		fprintf(stderr, "Unable to initialize NFD normalizer\n");
		return -1;
	}
	length = unorm2_getDecomposition(nfd, (UChar32) c, decomposition, 16, &error);
	if (U_FAILURE(error)) {
		fprintf(stderr, "Unable to get NFD decomposition for codepoint U%04X\n", c);
		return -1;
	}
	if (length <= 1) {
		//~ fprintf(stderr, " nope, no decomposition\n");
		return 0;
	}
	for (i = 0; i < length; ++i) {
		if (u_hasBinaryProperty((UChar32) decomposition[i], UCHAR_DIACRITIC)) {
			//~ fprintf(stderr, " YES (U-%04X %d/%d)\n", decomposition[i], i, length);
			return 1;
		}
	}
	//~ fprintf(stderr, " nope, no char in the decomposition is a diacritic\n");
	return 0;
}

int utf8_char_len(const char * src) {
	int i = 0;
	U8_FWD_1(src, i, -1);
	return i;
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
			xmlNodePtr rest = xmlNewText(text->content + lchar);
			if (text->next) {
				xmlNodePtr next = text->next;
				xmlUnlinkNode(text);
				xmlAddPrevSibling(next, rest);
			} else {
				xmlNodePtr parent = text->parent;
				xmlUnlinkNode(text);
				xmlAddChild(parent, rest);
			}
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
