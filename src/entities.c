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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <iconv.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

static int count = 0;
static const char RE[] = "\\[entity![^!]*!\\]";
static regex_t re;
static iconv_t iconv_state;

static
void exit_regerror(int err) {
	size_t s = regerror(err, &re, 0, 0);
	char * buffer = malloc(s);
	regerror(err, &re, buffer, s);
	fprintf(stderr, "Regular expression error : %s\n", buffer);
	free(buffer);
	regfree(&re);
	exit(-1);
}

void start_entities() {
	if(count++ == 0) {
		iconv_state = iconv_open("UTF-8", "UTF-32");
		int err = regcomp(&re, RE, 0);
		if (err) exit_regerror(err);
	}
}

void stop_entities() {
	if(--count == 0) {
		iconv_close(iconv_state);
		regfree(&re);
	}
}

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

static
char * new_entity(const char * src, size_t len) {
	char * temp;
	int offset = 8;
	temp = malloc(len - offset);

	if (src[offset] == '\\') ++offset;
	if (src[offset] != '#') {
		strcpy(temp, "&");
		strncat(temp, src + offset, len - offset);
		strcat(temp, ";");
	} else {
		int c, base = 10, uchar = 0;
		c = src[++offset];
		if (c == 'x') {
			base = 16;
			c = src[++offset];
		}
		while(1) {
			if (c >= '0' && c <= '9') {
				uchar = uchar*base + c - '0' + 0;
			} else {
				if (base == 16) {
					if (c >= 'a' && c <= 'f') {
						uchar = uchar*base + c - 'a' + 10;
					} else if (c >= 'A' && c <= 'F') {
						uchar = uchar*base + c - 'A' + 10;
					} else break;
				} else {
					break;
				}
			}
			if (++offset == len) break;
			c = src[offset];
		}
		to_utf8(uchar, temp, 5);
	}
	return temp;
}

extern
char * replace_entities(const char * content) {
	int offset = 0;
	char * result = malloc(strlen(content) + 1);
	char * temp;
	*result = 0;
	while (1) {
		regmatch_t match;
		int status = regexec(&re, content + offset, 1, &match, 0);
		if (status == 0) {
			if (match.rm_so != 0) {
				strncat(result, content + offset, match.rm_so);
			}
			temp = new_entity(content + offset + match.rm_so, match.rm_eo - match.rm_so);
			strcat(result, temp);
			free(temp);
			offset += match.rm_eo;
		} else if (status == REG_NOMATCH) {
			strcat(result, content + offset);
			break;
		} else
			exit_regerror(status);
	}
	return result;
}

static
xmlNodePtr xml_NewEntity(xmlDocPtr doc, xmlChar * src, size_t len) {
	xmlEntityPtr result;
	int offset = 8;
	if (src[offset] == '\\') ++offset;

	if (src[offset] != '#') {
		xmlChar * temp = xmlMalloc(1 + len - (offset + 2));
		memcpy(temp, src + offset, len - (offset + 2));
		temp[len - (offset + 1)] = 0;
		result = xmlGetDocEntity(doc, temp);
		xmlFree(temp);
	} else {
		xmlChar * temp;
		int c, base = 10, uchar = 0;
		c = src[++offset];
		if (c == 'x') {
			base = 16;
			c = src[++offset];
		}
		while(1) {
			if (c >= '0' && c <= '9') {
				uchar = uchar*base + c - '0' + 0;
			} else {
				if (base == 16) {
					if (c >= 'a' && c <= 'f') {
						uchar = uchar*base + c - 'a' + 10;
					} else if (c >= 'A' && c <= 'F') {
						uchar = uchar*base + c - 'A' + 10;
					} else break;
				} else {
					break;
				}
			}
			if (++offset == len) break;
			c = src[offset];
		}
		temp = xmlMalloc(5);
		to_utf8(uchar, temp, 5);
		result = (xmlEntityPtr) xmlNewText(temp);
		xmlFree(temp);
	}
	return (xmlNodePtr) result;
}

int xmlReplaceEntities(xmlNodePtr node) {
	xmlNodePtr marker;
	xmlChar * content = xmlNodeGetContent(node);
	int offset = 0;

	if (!content) return 0;

	marker = xmlNewNode(0, "tmp");
	xmlAddPrevSibling(node, marker);
	xmlUnlinkNode(node);
	xmlFreeNode(node);

	if (count <= 0) {
		fprintf(stderr, "Error: entities not started before calling replace_entities()\n");
		return 0;
	}

	while(1) {
		regmatch_t match;
		int status = regexec(&re, content + offset, 1, &match, 0);
		if (status == 0) {
			if (match.rm_so != 0) {
				xmlAddPrevSibling(marker, xmlNewTextLen(content + offset, match.rm_so));
			}
			xmlAddPrevSibling(marker, xml_NewEntity(marker->doc, content + offset + match.rm_so, match.rm_eo - match.rm_so));
			offset += match.rm_eo;
		} else if (status == REG_NOMATCH) {
			xmlAddPrevSibling(marker, xmlNewText(content + offset));
			break;
		} else
			exit_regerror(status);
	}
	xmlFree(content);
	xmlUnlinkNode(marker);
	xmlFreeNode(marker);
	return 1;
}
