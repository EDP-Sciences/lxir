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

#include "enc2tbl.h"

#define INVALID_CHAR ((char const *) -1)
#define SPACE_CHAR " "

iconv_t iconv_state;

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
		strcpy(buffer, SPACE_CHAR);
		return -1;
	}
	*output = 0;
	return 0;
}

struct adobe2unicode_t {
        char const * adobe;
		char const * unicode;
};

#define A2U_UNIT_SIZE (sizeof(struct adobe2unicode_t))

static int a2u_cmp(void const * av, void const * bv)
{
        struct adobe2unicode_t const * a = av;
        struct adobe2unicode_t const * b = bv;

        return strcmp(a->adobe, b->adobe);
}

struct adobe2unicode_t * adobe2unicode;

static int adobe2unicode_count;

void adobe2unicode_init() {
    xmlDocPtr doc;
    xmlNodePtr node;
	int i;

	doc = xmlReadFile("characters.xml", NULL, 0);
	
	node = doc->children->children; i = 0;
	
	while(node) {
		if (node->type == XML_ELEMENT_NODE) ++i;
		node = node->next;
	}
	adobe2unicode_count = i;
	adobe2unicode = malloc(adobe2unicode_count * sizeof(struct adobe2unicode_t));
	node = doc->children->children; i = 0;
	
	while(node) {
		if (node->type == XML_ELEMENT_NODE) {
			xmlChar * attr;
			struct adobe2unicode_t * chr = adobe2unicode + i;
			
			attr = xmlGetProp(node, BAD_CAST "name");
			if (!attr) {
				fprintf(stderr, "Invalid character : it has no name !\n");
				exit(1);
			}
			chr->adobe = strdup((char const *)attr);
			
			attr = xmlGetProp(node, BAD_CAST "value");
			if (!attr) {
				unsigned int code;
				attr = xmlGetProp(node, BAD_CAST "code");
				if (!attr) {
					fprintf(stderr, "Invalid character \"%s\": it has no value and no code !\n", chr->adobe);
					exit(1);
				}
				code = strtoul((char const *)attr, 0, 0);
				char * unicode_char = malloc(32);
				if (to_utf8(code, unicode_char, 32)) {
					fprintf(stderr, "Invalid conversion to UTF-8 for character \"%s\"\n", chr->adobe);
					exit(1);
				}
				chr->unicode = unicode_char;
			} else {
				chr->unicode = strdup((char const *)attr);
			}

			++i;
		}
		node = node->next;
	}
	xmlFreeDoc(doc);
	
	qsort(adobe2unicode, adobe2unicode_count, sizeof(struct adobe2unicode_t), a2u_cmp);
}

#define A2U_COUNT adobe2unicode_count
#define A2U_INIT() adobe2unicode_init()

struct enc_list {
	struct enc_list * next;
	char * name;
	struct ident_list * idents;
};

struct enc_list * encodings = 0;

struct ident_list * make_ident_list(struct ident_list * list, const char * name) {
	struct ident_list *	ret = (struct ident_list *) malloc(sizeof(struct ident_list));

	ret->name = name;
	ret->next = 0;
	ret->last = 0;
	if (list) {
		list->last->next = ret;
		list->last = ret;
	} else {
		list = ret;
		list->next = 0;
		list->last = list;
	}

	return list;
}

void register_enc(const char * name, struct ident_list * list) {
	struct enc_list * enc = encodings;
	while (enc) {
		if (strcmp(enc->name, name) == 0) {
			fprintf(stderr, "Warning: encoding \"%s\" registered more than once !\n", name);
			break;
		}
		enc = enc->next;
	}

	enc = (struct enc_list *) malloc(sizeof(struct enc_list));
	
	enc->next = encodings;
	enc->name = strdup(name);
	enc->idents = list;
	encodings = enc;
}

static char const * lookup(char const * name)
{
        struct adobe2unicode_t key;
        struct adobe2unicode_t * datum;

        key.adobe = (char const *) name;

        datum = bsearch(&key, adobe2unicode,
                        A2U_COUNT, A2U_UNIT_SIZE, a2u_cmp);

        if (!datum) {
			return INVALID_CHAR;
        }

        return datum->unicode;
}

static char const * getunicodechar(const char * name) {
	char * temp;
	char const * ret = lookup(name);
	static char buffer[32];
	if (ret != INVALID_CHAR) return ret;
	temp = strchr(name, '.');
	if (temp) {
		*temp = 0;
		ret = lookup(name);
		*temp = '.';
		if (ret != INVALID_CHAR) return ret;
	}
	if(strncmp(name, "uni", 3) == 0) {
		unsigned int code = strtoul(name + 3, &temp, 16);
		if(code && *temp == 0) {
			to_utf8(code, buffer, 32);
			return buffer;
		}
	}
	if(*name == 'u' || *name == 'U') {
		unsigned int code = strtoul(name + 1, &temp, 16);
		if(code && *temp == 0) {
			to_utf8(code, buffer, 32);
			return buffer;
		}
	}
	fprintf(stderr, "Unknown Adobe glyph name `%s'\n", name);
	return SPACE_CHAR;
}

int export_encodings() {
    xmlDocPtr doc;
    xmlNodePtr root;
	struct enc_list * enc;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "encodings");
	xmlDocSetRootElement(doc, root);
	
	enc = encodings;
	while(enc) {
		xmlNodePtr node;
		struct ident_list * ident;

		node = xmlNewChild(root, NULL, BAD_CAST "encoding", NULL);
		xmlNewProp(node, BAD_CAST "name", BAD_CAST enc->name+1);
		ident = enc->idents;
		while(ident) {
			char const * code = getunicodechar(ident->name + 1);

			xmlNodePtr chr = xmlNewChild(node, NULL, BAD_CAST "char", NULL);
			xmlNewProp(chr, BAD_CAST "value", BAD_CAST code);
			xmlNewProp(chr, BAD_CAST "name", BAD_CAST ident->name + 1);
			ident = ident->next;
		}

		enc = enc->next;
	}
	
	
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
    xmlMemoryDump();
	return 0;
}



void yyerror(const char * error) {
	fprintf(stderr, "Parse error : %s\n", error);
}

extern FILE * yyin;
int handle_enc(const char * filename) {
	if (strstr(filename, "/kmsym.enc") || strstr(filename, "/kmex.enc")) return 0;
	fprintf(stderr, "Reading file \"%s\"\n", filename);
	yyin = fopen(filename, "rb");
	yyparse();
	fclose(yyin);
	return 0;
}

extern int find_in_path(const char *, const char *, int (*)(const char *));


int main(int argc, char * argv[]) {
    LIBXML_TEST_VERSION;

	iconv_state = iconv_open("UTF-8", "UTF-32");

	A2U_INIT();

	/* yydebug = 1; */
	yy_flex_debug = 0;
	
	if (argc < 2) {
		fprintf(stderr, "usage : %s <path>\n", argv[0]);
		return -1;
	}
	
	find_in_path(argv[1], ".enc", handle_enc);

	if (iconv_state == (iconv_t) -1) {
		perror("iconv");
	}
	export_encodings();
	iconv_close(iconv_state);
	return 0;
}
