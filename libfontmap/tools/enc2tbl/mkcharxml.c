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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

iconv_t iconv_state;

struct adobe2unicode_t {
        char const * adobe;
        unsigned int unicode;
};

#include "adobetbl.h"

#define A2U_UNIT_SIZE (sizeof(struct adobe2unicode_t))
#define A2U_SIZE (sizeof(adobe2unicode))
#define A2U_COUNT (A2U_SIZE / A2U_UNIT_SIZE)

static
int to_utf8(unsigned int code, char * buffer, int size) {
	union {
		unsigned int code;
		char buffer[4];
	} u;
	char * input, * output;
	size_t src, dst, err;
	
	u.code = code;
	src = 4;
	dst = size;
	input = u.buffer;
	output = buffer;
	
	err = iconv(iconv_state, &input, &src, &output, &dst);
	if (err == (size_t)-1) {
		perror("iconv");
	}
	*output = 0;
	return 0;
}

int main(int argc, char * argv[]) {
    xmlDocPtr doc;
    xmlNodePtr root;
	int i;

    LIBXML_TEST_VERSION;

	iconv_state = iconv_open("UTF-8", "UTF-32");

	doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "characters");
	xmlDocSetRootElement(doc, root);
	xmlAddChild(root, xmlNewComment(BAD_CAST 
"\nYou can use the `code' parameter to declare a character based on his unicode\n"
"number, for instance : <char name=\"test\" code=\"0x1234\" />\n"));
	
	for (i = 0; i < A2U_COUNT; ++i) {
		char buffer[32];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "char");
		if (adobe2unicode[i].unicode < 0x100000) {
			if (to_utf8(adobe2unicode[i].unicode, buffer, 32) != 0) {
				fprintf(stderr, "Invalid character conversion for \"%s\"\n", adobe2unicode[i].adobe);
			}
			xmlNewProp(node, BAD_CAST "value", BAD_CAST buffer);
		} else {
			sprintf(buffer, "0x%X", adobe2unicode[i].unicode);
			xmlNewProp(node, BAD_CAST "code", BAD_CAST buffer);
		}
		xmlNewProp(node, BAD_CAST "name", BAD_CAST adobe2unicode[i].adobe);
		xmlAddChild(root, node);
	}
	
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	
	iconv_close(iconv_state);

	return 0;
}
