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

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <libfontmap.h>

#include "transformations.h"

#include <kpathsea/kpathsea.h>

#define NODE_VBOX 1	//
#define NODE_HBOX 2	//
#define NODE_SPECIAL 3	// *name* id
#define NODE_GLUE 4 //
#define NODE_PENALTY 5
#define NODE_RULE 6 // param
#define NODE_KERN 7
#define NODE_MATHON 8
#define NODE_MATHOFF 9
#define NODE_WRITE 10
#define NODE_DISCRETIONARY 11
#define NODE_LEADERS 12

#define NODE_OTHER 13 // *content*

struct {
	int type;
	const char * name;
	int len;
} types[] = {
	{ NODE_VBOX, "vbox", 4 },
	{ NODE_HBOX, "hbox", 4 },
	{ NODE_SPECIAL, "special", 7 },
	{ NODE_GLUE, "glue", 4 },
	{ NODE_PENALTY, "penalty", 7 },
	{ NODE_RULE, "rule", 4 },
	{ NODE_KERN, "kern", 4 },
	{ NODE_MATHON, "mathon", 6 },
	{ NODE_MATHOFF, "mathoff", 7 },
	{ NODE_WRITE, "write", 5 },
	{ NODE_DISCRETIONARY, "discretionary", 13 },
	{ NODE_LEADERS, "leaders", 7 },
};

#define NBTYPES (sizeof(types) / sizeof(types[0]))

static
int utf8(const char * c) {
	const unsigned char * codes = (const unsigned char *) c;
	int first = *codes++;
	int result = -1;
	if (first < 0x80) {
		result = first;
	} else if (first >= 0xC0 && first <= 0xDF) {
		int second = *codes++;
		if ((second & 0xC0) == 0x80) {
			result = (first & 0x1F) << 6 | (second & 0x3F);
			if (result < (1 << 7)) result = -1;
		}
	} else if (first >= 0xE0 && first <= 0xEF) {
		int second = *codes++;
		int third = *codes++;
		if ((second & 0xC0) == 0x80 && (third & 0xC0) == 0x80) {
			result = (first & 0xF) << 12 | (second & 0x3F) << 6 | (third & 0x3F);
			if (result < (1 << 11)) result = -1;
		}
	} else if (first >= 0xF0 && first <= 0xF7) {
		int second = *codes++;
		int third = *codes++;
		int fourth = *codes++;
		if ((second & 0xC0) == 0x80 && (third & 0xC0) == 0x80&& (fourth & 0xC0) == 0x80) {
			result = (first & 0x7) << 18 | (second & 0x3F) << 12 | (third & 0x3F) << 6 | (fourth & 0x3F);
			if (result < (1 << 15)) result = -1;
		}
	}

	return result;
}

static
char * to_utf8(int c, char * b) {
	if (c < 0) {
		*b++ = '<';
		*b++ = '-';
		*b++ = '1';
		*b++ = '>';
	} else if (c < (1 << 7)) {
		*b++ = c;
	} else if (c < (1 << 11)) {
		*b++ = ((c >> 6) & 0x1F) | 0xC0;
		*b++ = (c & 0x3F) | 0x80;
	} else if (c < (1 << 16)) {
		*b++ = ((c >> 12) & 0xF) | 0xE0;
		*b++ = ((c >> 6) & 0x3F) | 0x80;
		*b++ = (c & 0x3F) | 0x80;
	} else if (c < (1 << 21)) {
		*b++ = ((c >> 18) & 0x7) | 0xF0;
		*b++ = ((c >> 12) & 0x3F) | 0x80;
		*b++ = ((c >> 6) & 0x3F) | 0x80;
		*b++ = (c & 0x3F) | 0x80;
	} else {
		*b++ = '<';
		*b++ = 'i';
		*b++ = 'n';
		*b++ = '>';
	}
	*b = 0;
	return b;
}

static
xmlNodePtr new_node(const char * line) {
	xmlNodePtr node;
	int i, type = NODE_OTHER;
	const char * name, * param;

	if (*line++ != '\\') {
		fprintf(stderr, "Invalid line : \"%s\", expected a '\\'\n", line - 1);
		exit(-1);
	}

	for(i = 0; i < NBTYPES; ++i) {
		if(strncmp(line, types[i].name, types[i].len) == 0) {
			name = types[i].name;
			type = types[i].type;
			param = line + types[i].len;
			break;
		}
	}
	if (type == NODE_OTHER) {
		name = "other";
		param = line;
	}
	node = xmlNewNode(0, BAD_CAST name);
	xmlNewTextChild(node, 0, BAD_CAST "param", BAD_CAST param);

	return node;
}

int read_log_file(xmlNodePtr root, const char * filename) {
	char line[1024];
	int state = 0;
	int err, depth;
	regex_t page_regex, box_regex;

	xmlNodePtr current = 0;
	FILE * f = fopen(filename, "r");
	if (!f) return -1;

	err = regcomp(&page_regex, "\\*\\*\\*T@GS START OUTPUTING BOX TO LOG\\*\\*\\*", REG_EXTENDED);
	if(err) {
		char * errbuff;
		size_t s = regerror(err, &page_regex, 0, 0);
		errbuff = malloc(s);
		regerror(err, &page_regex, errbuff, s);
		fprintf(stderr, "Error compiling page_regex : %s\n", errbuff);
		free(errbuff);
		exit(-1);
	}

	err = regcomp(&box_regex, "Completed box being shipped out \\[([0-9]+)\\]", REG_EXTENDED);
	if(err) {
		char * errbuff;
		size_t s = regerror(err, &box_regex, 0, 0);
		errbuff = malloc(s);
		regerror(err, &box_regex, errbuff, s);
		fprintf(stderr, "Error compiling box_regex : %s\n", errbuff);
		free(errbuff);
		exit(-1);
	}

	while(!feof(f)) {
		if(!fgets(line, 1024, f)) {
			if (ferror(f)) {
				perror("There was an error reading input file");
				exit(-1);
			}
			break;
		} else {
			int l = strlen(line) - 1;
			if (line[l] == '\n') line[l] = 0;
		}
		if (state == 0) {
			int ret = regexec(&page_regex, line, 0, 0, 0);
			if(ret == 0) {
				state = 1;
				current = xmlNewNode(0, BAD_CAST "page");
				xmlAddChild(root, current);
			}
		} else if (state == 1) {
			regmatch_t matches[2];
			int ret = regexec(&box_regex, line, 2, matches, 0);
			if(ret == 0) {
				state = 2;
				depth = 0;
				if (matches[1].rm_so >= 0) {
					int len = matches[1].rm_eo - matches[1].rm_so;
					char * buffer = malloc(len + 1);
					memcpy(buffer, line + matches[1].rm_so, len);
					buffer[len] = 0;
					xmlNewProp(current, BAD_CAST "id", BAD_CAST buffer);
					free(buffer);
				}
			}
		} else if (state == 2 && line[0] == 0) {
			state = 3;
		} else if (state == 2 && line[0] != '.' && depth > 0) {
			xmlNodeAddContent(current->children, BAD_CAST line);
		} else if (state == 2) {
			int d = 0;
			xmlNodePtr node;

			while (line[d] == '.') d++;
			if (line[d] == '|') d++;
			if(line[d] == '\\') {

				node = new_node(line + d);

				if (d == 0 && depth == 0) {
					xmlAddChild(current, node);
				} else if (d == depth) {
					// append as next
					xmlAddNextSibling(current, node);
				} else if(d == depth + 1) {
					// append as children
					xmlAddChild(current, node);
				} else if(d < depth) {
					// move to parent and append as next
					while(depth > d) {
						if (!current->parent) {
							fprintf(stderr, "INVALID NODE tree\n");
							exit(-1);
						}
						current = current->parent;
						depth -= 1;
					}
					xmlAddNextSibling(current, node);
				} else {
					fprintf(stderr, "INVALID node structure\n");
					exit(-1);
				}
				current = node;
				depth = d;
			} else {
				xmlNodeAddContent(current->children, BAD_CAST line);
			}
		} else if (state == 3) {
			if (line[0] != '.') {
				state = 0;
			} else {
				int d = 0;
				xmlNodePtr node;

				xmlNodeAddContent(current->children, BAD_CAST "\n");
				state = 2;
				while (line[d] == '.') d++;
				node = new_node(line + d);

				if (d == depth) {
					// append as next
					xmlAddNextSibling(current, node);
				} else if(d == depth + 1) {
					// append as children
					xmlAddChild(current, node);
				} else if(d < depth) {
					// move to parent and append as next
					while(depth > d) {
						if (!current->parent) {
							fprintf(stderr, "INVALID NODE tree\n");
							exit(-1);
						}
						current = current->parent;
						depth -= 1;
					}
					xmlAddNextSibling(current, node);
				} else {
					fprintf(stderr, "INVALID node structure\n");
					exit(-1);
				}
				current = node;
				depth = d;
			}
		}
	}

	fclose(f);
	regfree(&page_regex);
	regfree(&box_regex);
	return 0;
}

static
int is_node_valid(xmlNodePtr node, char const * type, char const * content, size_t len) {
	int result =
		node &&
		node->type == XML_ELEMENT_NODE &&
		strcmp((const char *)node->name, type) == 0;
	if (!result || !content) return result;


	result =
			node->children &&
			node->children->type == XML_ELEMENT_NODE &&
			strcmp((const char *)node->children->name, "param") == 0 &&
			node->children->children &&
			node->children->children->type == XML_TEXT_NODE;
	if (!result) return result;

	if(len == -1) len = strlen(content);

	if (len == 0) {
		return strcmp((const char *)node->children->children->content, content) == 0;
	} else {
		return strncmp((const char *)node->children->children->content, content, len) == 0;
	}
}

static
int is_node_valid_mathbox(xmlNodePtr node) {
	return
		is_node_valid(node, "hbox", 0, 0) ||
		is_node_valid(node, "vbox", 0, 0) ||
		is_node_valid(node, "other", 0, 0) ||
		is_node_valid(node, "mrow", 0, 0) ||
		is_node_valid(node, "msub", 0, 0) ||
		is_node_valid(node, "msup", 0, 0) ||
		is_node_valid(node, "msubsup", 0, 0) ||
		is_node_valid(node, "mfenced", 0, 0) ||
		is_node_valid(node, "menclose", 0, 0) ||
		is_node_valid(node, "math-above", 0, 0)
		;
}

static
int real_xml_extract_math_id(xmlNodePtr node, const char * content, int len, char * buffer) {
	while(node) {
		int ret;

		if (is_node_valid(node, "special", content, len)) {
			const char * str = (const char *) node->children->children->content + len;
			while(isdigit(*str)) {
				*buffer++ = *str++;
			}
			*buffer = 0;
			xmlUnlinkNode(node);
			xmlFreeNode(node);
			return 0;
		}

		ret = real_xml_extract_math_id(node->children, content, len, buffer);
		if (!ret) return ret;
		node = node->next;
	}
	return 1;
}

static
int xml_extract_math_id(xmlNodePtr base, const char * type, const char * p, char * e) {
	char content[128];

	int len = sprintf(content, "{::tag lxir %s(%s){id=", type, p);

	return real_xml_extract_math_id(base, content, len, e);
}

static
xmlNodePtr extract_math_expr(xmlNodePtr start, xmlNodePtr end, const char * type) {
	xmlNodePtr math = xmlNewNode(0, BAD_CAST "math");
	// xmlNewProp(math, BAD_CAST "type", type);
	char id[32];

	while (start->next != end) {
		xmlNodePtr node = start->next;
		xmlUnlinkNode(node);
		xmlAddChild(math, node);
	}
	if(xml_extract_math_id(math, "begin", type, id) == 0)
		xmlNewProp(math, BAD_CAST "begin-id", BAD_CAST id);
	if(xml_extract_math_id(end, "end", type, id) == 0)
		xmlNewProp(math, BAD_CAST "end-id", BAD_CAST id);

	return math;
}

static
const char * get_charcode(const xmlChar * content, struct translation_info * trans) {
	int charcode;
	charcode = content[0];
	if (charcode == '^' && content[1] == '^') {
		charcode = content[2];
		if(charcode < 64) {
			charcode += 64;
		} else {
			charcode -= 64;
		}
	}
	if(charcode >= trans->size) {
			fprintf(stderr, "Invalid charcode : \"%s\" : %d >= %d\n", content, charcode, trans->size);
			exit(-1);
	}
	return trans->map[charcode];
}

static
const char * get_other_content(xmlNodePtr node, char const **pvariant) {
	struct translation_info trans;
	char * type;
	const char * ret;
	int len;

	if(!is_node_valid(node, "other", 0, 0)) return 0;

	type = strdup((const char *)node->children->children->content);
	*strchr(type, ' ') = 0;
	len = strlen(type);
	if(lfm_get_math_encoding_map(type, &trans)) {
		fprintf(stderr, "Unable to find encoding for math font \"%s\"\n", type);
		exit(-1);
	}
	if (pvariant) {
		*pvariant = lfm_get_math_encoding_variant(type);
	}
	ret = get_charcode(node->children->children->content + len + 1, &trans);
	free(type);
	return ret;
}

/*
pattern for inline math :
	<mathon />
	-- (NODE_HBOX|NODE_OTHER)*
	<mathoff />
	<special><content>{::tag lxir end(math){id=%d}}</content></special>
*/

static
void extract_inline_math(xmlNodePtr root, xmlNodePtr basemath) {
	xmlNodePtr node = root->children;
	while (node) {
		if (is_node_valid(node, "mathon", 0, 0)) {
			xmlNodePtr math, end;
			end = node->next;
			while (end && !is_node_valid(end, "mathoff", 0, 0)) end = end->next;
			if (!end) {
				char id[32];
				if(xml_extract_math_id(node, "begin", "math", id)) strcpy(id, "*unknown*");
				fprintf(stderr, "lxir: Unable to find mathoff after mathon begining at ID:%s !\n", id);
				exit(-1);
			}
			math = extract_math_expr(node, end, "math");
			xmlNewProp(math, BAD_CAST "type", BAD_CAST "inline");
			xmlAddChild(basemath, math);
		} else {
			extract_inline_math(node, basemath);
		}
		node = node->next;
	}
}

static
void extract_special_inline_math(xmlNodePtr root, xmlNodePtr basemath) {
	xmlNodePtr node = root->children;
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "mathoff", 0, 0)) {
			xmlNodePtr start = node->prev;
			while (start && !is_node_valid(start, "special", "{::tag lxir begin(math){id=", -1)) start = start->prev;
			if (start) {
				xmlNodePtr mathon = xmlNewNode(0, BAD_CAST "mathon");
				xmlAddPrevSibling(start, mathon);
				xmlNodePtr before = mathon->prev;
				xmlNodePtr math = extract_math_expr(start->prev, node, "math");
				xmlNewProp(math, BAD_CAST "type", BAD_CAST "inline");
				xmlAddChild(basemath, math);
				next = before ? before->next : root->children;
			}
		} else {
			extract_special_inline_math(node, basemath);
		}
		node = next;
	}
}

/*
pattern for display math :
	penalty 10000
	glue (\abovedisplayshortskip)
	glue (\baselineskip) || glue (\lineskip)
		(NODE_HBOX)*
	penalty 0
	glue (\belowdisplayshortskip)
	glue (\baselineskip) || glue(\lineskip)
	special "{::tag lxir end(display){id=%d}}"
*/
static
void extract_display_math(xmlNodePtr root, xmlNodePtr basemath) {
	xmlNodePtr node = root->children;
	int state = 0;
	while (node) {
		if (state == 2) {
			if (is_node_valid(node, "glue", "(\\baselineskip)", 15) ||
				is_node_valid(node, "glue", "(\\lineskip)", 11)) {

				xmlNodePtr end = node->next;
				int estate = 0;
				while (end) {
					if (estate == 2) {
						if (is_node_valid(end, "glue", "(\\baselineskip)", 15) ||
							is_node_valid(end, "glue", "(\\lineskip)", 11)) {
							break;
						} else {
							estate = 0;
						}
					} else if (estate == 1) {
						if (
							is_node_valid(end, "glue", "(\\belowdisplayshortskip)", 24) ||
							is_node_valid(end, "glue", "(\\belowdisplayskip)", 19)
						) {
							estate = 2;
						} else {
							estate = 0;
						}
					}
					if (estate == 0) {
						if (is_node_valid(end, "penalty", " 0", 0)) {
							estate = 1;
						}
					}
					end = end->next;
				}
				if (!end) {
					char id[32];
					if(xml_extract_math_id(node, "begin", "display", id)) strcpy(id, "*unknown*");
					fprintf(stderr, "lxir: Unable to find the end of display section begining at ID:%s !\n", id);
					exit(-1);
				} else {
					xmlNodePtr math;
					end = end->prev->prev;
					math = extract_math_expr(node, end, "display");
					xmlNewProp(math, BAD_CAST "type", BAD_CAST "display");
					xmlAddChild(basemath, math);
				}
			} else {
				state = 0;
			}
		} else if (state == 1) {
			if (
				is_node_valid(node, "glue", "(\\abovedisplayshortskip)", 24) ||
				is_node_valid(node, "glue", "(\\abovedisplayskip)", 19)
			) {

				state = 2;
			} else {
				state = 0;
			}
		}
		if (state == 0) {
			if (is_node_valid(node, "penalty", " 10000", 0)) {
				state = 1;
			} else {
				extract_display_math(node, basemath);
			}
		}
		node = node->next;
	}
}

static
void transform_inline_math(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;

	while (node) {
		xmlNodePtr next = node->next;

		if (is_node_valid(node, "mathon", 0, 0)) {
			xmlNodePtr math, end;
			end = node->next;
			while (end && !is_node_valid(end, "mathoff", 0, 0)) { end = end->next; }
			if (!end) {
				char id[32];
				if(xml_extract_math_id(node, "begin", "math", id)) strcpy(id, "*unknown*");
				fprintf(stderr, "lxir: Unable to find mathoff after mathon begining at ID:%s !\n", id);
				exit(-1);
			}
			math = extract_math_expr(node, end, "math");
			xmlNewProp(math, BAD_CAST "type", BAD_CAST "inline");
			xmlNodeSetName(math, BAD_CAST "mrow");
			xmlAddPrevSibling(node, math);
			xmlUnlinkNode(node);
			xmlFreeNode(node);
			next = end->next;
			xmlUnlinkNode(end);
			xmlFreeNode(end);

		} else {
			xmlTransformationPush(node, transform_inline_math, param);
		}
		node = next;
	}
}

static
xmlNodePtr extract_all_math(xmlNodePtr root) {
	xmlNodePtr math = xmlNewNode(0, BAD_CAST "math-log");
	extract_display_math(root, math);
	extract_inline_math(root, math);
	extract_special_inline_math(root, math);

	return math;
}

/*
	this is the pattern we'll try to match :
	<vbox>
		<param />
		<hbox> {...} </hbox> / <mfenced>
		<kern /> ?
		<rule />
		<kern /> ?
		<hbox> {...} </hbox>
	</vbox>

	we will produce :

	<mfrac>
		<hbox> {...} </hbox>
		<hbox> {...} </hbox>
	</mfrac>
*/

static
void transform_over_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr row1 = 0, row2 = 0, rule = 0, tmp = 0;

		if(is_node_valid(node, "vbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(row1 = tmp->next) &&
			is_node_valid_mathbox(row1) &&
			(tmp = row1->next) &&
			(	(
					is_node_valid(tmp, "kern", 0, 0) &&
					(rule = tmp->next) &&
					is_node_valid(rule, "rule", 0, 0)
				) || (
					is_node_valid(tmp, "rule", 0, 0) &&
					(rule = tmp)
				)
			) &&
			(tmp = rule->next) &&
			(
				(
					is_node_valid(tmp, "kern", 0, 0) &&
					(row2 = tmp->next) &&
					is_node_valid_mathbox(row2)
				) || (
					is_node_valid_mathbox(tmp) &&
					(row2 = tmp)
				)
			) &&
				!row2->next)
				{

			xmlNodePtr frac;
			frac = xmlNewNode(0, BAD_CAST "mfrac");
			xmlUnlinkNode(row1);
			xmlAddChild(frac, row1);
			xmlUnlinkNode(row2);
			xmlAddChild(frac, row2);
			xmlAddPrevSibling(node, frac);
			xmlUnlinkNode(node);
			xmlFreeNode(node);
			xmlTransformationPush(row1, transform_over_pattern, param);
			xmlTransformationPush(row2, transform_over_pattern, param);
		} else {
			xmlTransformationPush(node, transform_over_pattern, param);
		}

		node = next;
	}
}

/*
	This is the pattern we'll try to match :
	<mi>*UNICODE SQRT CHAR*</mi>
	<menclose notation="top">
		<mrow /> {1}
	</menclose>

	and replace by :

	<msqrt>
		<mrow /> {1}
	</msqrt>
*/

static
int is_sqrt_symbol_node(xmlNodePtr node) {
	const char * content;
	int code;
	if (is_node_valid(node, "mrow", 0, 0) &&
			node->children && !node->children->next)
		node = node->children;

	if(
		is_node_valid(node, "mi", 0, 0) && node->children &&
		(content = (const char *)node->children->content) &&
		(code = utf8(content)) &&
		(
			(code >= 0x10FF70 && code <= 0x0010ff76) ||
			code == 0x221A
		)
	) {
		return 1;
	}
	return 0;
}

static
int is_sqrt_symbol_list(xmlNodePtr node) {
	while(node) {
		if (!is_sqrt_symbol_node(node)) return 0;
		node = node->next;
	}
	return 1;
}

static
void transform_sqrt_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlChar * prop;
		xmlNodePtr next = node->next;
		xmlNodePtr tmp = 0, content = 0;
		if(
			(
				is_sqrt_symbol_node(node) ||
				(
					is_node_valid(node, "msup", 0, 0) &&
				/*	(prop = xmlGetProp(node, BAD_CAST "single")) &&
					(strcmp((const char *)prop, "1") == 0) &&
					(xmlFree(prop), 1) &&
				*/	(tmp = node->children) &&
					is_sqrt_symbol_node(tmp)
				) ||
				(
					is_node_valid(node, "vbox", 0, 0) &&
					is_sqrt_symbol_list(node->children->next)
				)
			) &&
			(tmp = node->next) &&
			is_node_valid(tmp, "menclose", 0, 0) &&
			(prop = xmlGetProp(tmp, BAD_CAST "notation")) &&
			(strcmp((const char *)prop, "top") == 0) &&
			(xmlFree(prop), 1) &&
			(content = tmp->children)
		) {
			xmlNodePtr sqrt = xmlNewNode(0, BAD_CAST "msqrt");
			xmlUnlinkNode(content);
			xmlAddChild(sqrt, content);
			xmlAddPrevSibling(node, sqrt);
			xmlUnlinkNode(node);
			xmlFreeNode(node);
			xmlUnlinkNode(tmp);
			xmlFreeNode(tmp);
			next = sqrt->next;

			xmlTransformationPush(content, transform_sqrt_pattern, param);

		} else {
			xmlTransformationPush(node, transform_sqrt_pattern, param);
		}

		node = next;
	}
}

/*
	This is the pattern we'll try to match :
	<vbox>
		<param />
		<hbox /> {1}
		<kern />
		<rule />
	</vbox>

	and replace by :

	<menclose notation="bottom">
		<hbox /> {1}
	</menclose>
*/

static
void transform_underline_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;

		xmlNodePtr content = 0, tmp = 0;
		if(
			is_node_valid(node, "vbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(content = tmp->next) &&
			is_node_valid_mathbox(content) &&
			(tmp = content->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "rule", 0, 0) &&
			(!tmp->next)
		) {
			xmlNodePtr under = xmlNewNode(0, BAD_CAST "menclose");
			xmlNewProp(under, BAD_CAST "notation", BAD_CAST "bottom");
			xmlUnlinkNode(content);
			xmlAddChild(under, content);
			xmlAddPrevSibling(node, under);
			xmlUnlinkNode(node);
			xmlFreeNode(node);

			next = under->next;

			xmlTransformationPush(under, transform_underline_pattern, param);

		} else {
			xmlTransformationPush(node, transform_underline_pattern, param);
		}

		node = next;
	}
}

/*
	This is the pattern we'll try to match :
	<vbox>
		<param />
		<kern />
		<rule />
		<kern />
		<hbox /> {1}
	</vbox>

	and replace by :

	<menclose notation="top">
		<hbox /> {1}
	</menclose>

	NOTE: do this before looking for sqrt
*/

static
void transform_overline_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;

		xmlNodePtr content = 0, tmp = 0;
		if(
			is_node_valid(node, "vbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "rule", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(content = tmp->next) &&
			is_node_valid_mathbox(content) &&
			(!content->next)
		) {
			xmlNodePtr over = xmlNewNode(0, BAD_CAST "menclose");
			xmlNewProp(over, BAD_CAST "notation", BAD_CAST "top");
			xmlUnlinkNode(content);
			xmlAddChild(over, content);
			xmlAddPrevSibling(node, over);
			xmlUnlinkNode(node);
			xmlFreeNode(node);

			next = over->next;

			xmlTransformationPush(over, transform_overline_pattern, param);

		} else {
			xmlTransformationPush(node, transform_overline_pattern, param);
		}

		node = next;
	}
}

/*
	this is the pattern we'll try to match :

	<hbox /> || <other />
	<vbox>
		<param />
		<hbox />
		<kern />
		<hbox />
	</vbox>
*/

static
void transform_subsup_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr content = 0, subscript = 0, superscript = 0, tmp = 0, box = 0;

		if ((content = node) &&
			(
				is_node_valid(content, "other", 0, 0) ||
				is_node_valid(content, "hbox", 0, 0) ||
				(
					is_node_valid(content, "glue", 0, 0) &&
					(content = content->next) &&
					(
						is_node_valid(content, "other", 0, 0) ||
						is_node_valid(content, "hbox", 0, 0)
					)
				)
			) &&
			(box = content->next) &&
			is_node_valid(box, "vbox", 0, 0) &&
			/* (!box->next) && */
			(tmp = box->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(superscript = tmp->next) &&
			is_node_valid(superscript, "hbox", 0, 0) &&
			(tmp = superscript->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(subscript = tmp->next) &&
			is_node_valid(subscript, "hbox", 0, 0) &&
			!subscript->next

		) {
			next = box->next;

			xmlNodePtr subsup = xmlNewNode(0, BAD_CAST "msubsup");

			xmlAddPrevSibling(content, subsup);
			xmlUnlinkNode(content);
			xmlAddChild(subsup, content);
			xmlUnlinkNode(subscript);
			xmlAddChild(subsup, subscript);
			xmlUnlinkNode(superscript);
			xmlAddChild(subsup, superscript);

			transform_subsup_pattern(content, param);
			transform_subsup_pattern(subscript, param);
			transform_subsup_pattern(superscript, param);

			xmlUnlinkNode(box);
			xmlFreeNode(box);

			if(is_node_valid(subscript->children, "param", 0, 0)) {
				xmlNodePtr p = subscript->children;
				xmlUnlinkNode(p);
				xmlFreeNode(p);
			}
			if(is_node_valid(superscript->children, "param", 0, 0)) {
				xmlNodePtr p = superscript->children;
				xmlUnlinkNode(p);
				xmlFreeNode(p);
			}
		} else {
			xmlTransformationPush(node, transform_subsup_pattern, param);
		}
		node = next;
	}
}

static
const char * trans_fenced_char(const char * chr) {
	int code = utf8(chr);

	if((code & 0xffffff00) == 0x0010ff00) {
		code &= 0xff;
		if (code == 000 || code == 020 || code == 022 || code == 040) return "(";
		if (code == 001 || code == 021 || code == 023 || code == 041) return ")";
		if (code == 002 || code == 024 || code == 042 || code == 0150) return "[";
		if (code == 003 || code == 025 || code == 043 || code == 0151) return "]";
		if (code == 004 || code == 026 || code == 044 || code == 0152) return "⌊";
		if (code == 005 || code == 027 || code == 045 || code == 0153) return "⌋";
		if (code == 006 || code == 030 || code == 046 || code == 0154) return "⌈";
		if (code == 007 || code == 031 || code == 047 || code == 0155) return "⌉";
		if (code == 010 || code == 032 || code == 050 || code == 0156) return "{";
		if (code == 011 || code == 033 || code == 051 || code == 0157) return "}";
		if (code == 012 || code == 034 || code == 052 || code == 0104) return "⟨";
		if (code == 013 || code == 035 || code == 053 || code == 0105) return "⟩";
		if (code == 016 || code == 036 || code == 054) return "/";
		if (code == 017 || code == 037 || code == 055) return "\\";
		if (code == 014) return "|";
		if (code == 015) return "∥";
		if (code == 0132 || code == 0122) return "∫";
		fprintf(stderr, "lxir: unknown fenced char %d\n", code);
	}
	return chr;
}

static
const char * get_fenced_char(xmlNodePtr node) {
	return trans_fenced_char(get_other_content(node, 0));
}

static regex_t box_param_regex;
static int regex_use_count;

static
xmlChar * extract_match(const xmlChar * content, const regmatch_t * match) {
	int len;
	xmlChar * buffer;
	len = match->rm_eo - match->rm_so;
	if (len == 0) return NULL;
	buffer = (xmlChar *) xmlMalloc(len + 1);
	memcpy(buffer, content + match->rm_so, len);
	buffer[len] = 0;
	return buffer;
}

static
void real_decode_box_parameters(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while (node) {
		xmlNodePtr next = node->next;
		xmlNodePtr param_node, content_node;

		if(
			(
				is_node_valid(node, "hbox", 0, 0) || is_node_valid(node, "vbox", 0, 0)
			) &&
			(param_node = node->children) &&
			is_node_valid(param_node, "param", 0, 0) &&
			(content_node = param_node->children) &&
			content_node->type == XML_TEXT_NODE
		) {
			regmatch_t matches[6];
			xmlChar * content = content_node->content;
			int ret = regexec(&box_param_regex, content, 6, matches, 0);
			if(ret == 0) {
				xmlChar * sub;

				sub = extract_match(content, &matches[1]);
				if (sub) { xmlSetProp(node, BAD_CAST "height", sub); xmlFree(sub); }
				sub = extract_match(content, &matches[2]);
				if (sub) { xmlSetProp(node, BAD_CAST "depth", sub); xmlFree(sub); }
				sub = extract_match(content, &matches[3]);
				if (sub) { xmlSetProp(node, BAD_CAST "width", sub); xmlFree(sub); }
				sub = extract_match(content, &matches[5]);
				if (sub) { xmlSetProp(node, BAD_CAST "shift", sub); xmlFree(sub); }
			}
		}
		++regex_use_count;
		xmlTransformationPush(node, real_decode_box_parameters, param);
		node = next;
	}
	if (--regex_use_count == 0) {
		regfree(&box_param_regex);
	}
}

static
void decode_box_parameters(xmlNodePtr root, xmlTransformationEntry * param) {
	int err = regcomp(&box_param_regex, "\\(([+-]?[0-9]+.[0-9]+)\\+([+-]?[0-9]+.[0-9]+)\\)x([+-]?[0-9]+.[0-9]+)(, shifted ([+-]?[0-9]+.[0-9]+))?", REG_EXTENDED);
	if(err) {
		char * errbuff;
		size_t s = regerror(err, &box_param_regex, 0, 0);
		errbuff = malloc(s);
		regerror(err, &box_param_regex, errbuff, s);
		fprintf(stderr, "lxir: Error compiling box_param_regex : %s\n", errbuff);
		free(errbuff);
		exit(-1);
	}

	regex_use_count = 1;
	real_decode_box_parameters(root, param);
}

#if 0
/*
	<mrow>
		<hbox>
			<hbox /> {1}
			<vbox />
			<hbox />
		</hbox>
	</mrow>
	<*>* {2} ####FIXME : needs to be greedy here !
	<mrow>
		<hbox>
			<hbox /> {3}
			<vbox />
			<hbox />
		</hbox>
	</mrow>
=>
	<mfenced open="{1}" close = "{3}">
	{2}
	</mfenced>
*/

static
void transform_left_and_right_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr begin, left, end, right;

	xmlNodePtr node = root->children;
	int state = 0;

	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr box, row, sub, tmp;

		if (
			is_node_valid(node, "hbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "hbox", 0, 0) &&
			(tmp = tmp->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "special", "{::tag lxir begin(hbox){id=", 27) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "glue", 0, 0) &&
			(row = tmp->next) &&
			is_node_valid(row, "mrow", 0, 0) &&
			(tmp = row->children) &&
			is_node_valid(tmp, "hbox", 0, 0) &&
			!tmp->next &&
			(tmp = tmp->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(box = tmp->next) &&
			is_node_valid(box, "hbox", 0, 0) &&
			(tmp = box->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(sub = tmp->next) &&
			is_node_valid(sub, "other", 0, 0) &&
			!sub->next &&
			(tmp = box->next) &&
			is_node_valid(tmp, "vbox", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "hbox", 0, 0) &&
			!tmp->next
		) {
			if (state == 0) {
				begin = node;
				left = sub;
				state = 1;
			} else {
				end = node;
				right = sub;
				state = 2;
			}
		} else {
			xmlTransformationPush(node, transform_left_and_right_pattern, param);
		}

		if (next == 0 && state == 2) {
			xmlNodePtr fenced = xmlNewNode(0, BAD_CAST "mfenced");
			xmlNodePtr content = xmlNewNode(0, BAD_CAST "mrow");
			xmlAddChild(fenced, content);

			xmlNewProp(fenced, BAD_CAST "open", BAD_CAST get_fenced_char(left));
			xmlNewProp(fenced, BAD_CAST "close", BAD_CAST get_fenced_char(right));

			tmp = begin->next;
			do {
				xmlNodePtr next = tmp->next;
				xmlUnlinkNode(tmp);
				xmlAddChild(content, tmp);
				tmp = next;
			} while(tmp && tmp != end);

			xmlAddPrevSibling(begin, fenced);

			xmlUnlinkNode(begin);
			xmlFreeNode(begin);
			xmlUnlinkNode(end);
			xmlFreeNode(end);

			transform_left_and_right_pattern(content, param);

			next = fenced->next;
			state = 0;
		}
		node = next;
	}
}
#else
/*
	<hbox>
		<param />
		<hbox>
			<other /> {1}
		<hbox>
		... {2}
		<hbox>
			<other /> {3}
		</hbox>
	<hbox>
	(*) the three hboxes must have the same (height + depth).

	<mfenced open="{1}" close = "{3}">
		{2}
	</mfenced>
*/

static
double get_box_total_height(xmlNodePtr node) {
	double h, d;
	xmlChar * attr;
	attr = xmlGetProp(node, BAD_CAST "height");
	h = strtod(attr, 0);
	xmlFree(attr);
	attr = xmlGetProp(node, BAD_CAST "depth");
	d = strtod(attr, 0);
	xmlFree(attr);

	return h + d;
}

#define SIZE_DELTA 1e-5
static
xmlNodePtr is_basic_fence_node(xmlNodePtr node) {
	xmlNodePtr other_node;
	if (
		is_node_valid(node, "hbox", 0, 0) &&
		(is_node_valid(node->children, "param", 0, 0)) &&
		(other_node = node->children->next) &&
		is_node_valid(other_node, "other", 0, 0) &&
		(is_node_valid(other_node->children, "param", 0, 0)) &&
		!other_node->next
	) {
		return other_node;
	}
	return 0;
}

static
const char * get_composite_fence_char(const char ** fences, int len) {
	const char * first = fences[0];
	const char * last = fences[len-1];
	const char * middle = (len % 2) ? fences[len>>1] : 0;

	if (strcmp(first, "⎧") == 0 && strcmp(middle, "⎨") == 0 && strcmp(last, "⎩") == 0) return "{";
	if (strcmp(first, "⎛") == 0 && (!middle || strcmp(middle, "⎜") == 0) && strcmp(last, "⎝") == 0) return "(";
	if (strcmp(first, "⎞") == 0 && (!middle || strcmp(middle, "⎟") == 0) && strcmp(last, "⎠") == 0) return ")";
	if (strcmp(first, "⎡") == 0 && (!middle || strcmp(middle, "⎢") == 0) && strcmp(last, "⎣") == 0) return "[";
	if (strcmp(first, "⎤") == 0 && (!middle || strcmp(middle, "⎥") == 0) && strcmp(last, "⎦") == 0) return "]";
	if (strcmp(first, "|") == 0 && (!middle || strcmp(middle, "|") == 0) && strcmp(last, "|") == 0) return "|";

	fprintf(stderr, "lxir: unknown composite fence char <%s %s %s>\n", first, middle ? middle : "", last);
	return 0;
}

static
const char * is_composite_fence_node(xmlNodePtr node, double height) {
	if (
		is_node_valid(node, "vbox", 0, 0) &&
		(get_box_total_height(node) - height) < SIZE_DELTA
	) {
		xmlNodePtr sub = node->children->next; /* skip <param> */
		const char * fences[32];
		int len = 0;
		while (sub) {
			xmlNodePtr o = is_basic_fence_node(sub);
			if (!o) return 0;
			assert(len < 32);
			fences[len++] = get_fenced_char(o);
			sub = sub->next;
		}
		return get_composite_fence_char(fences, len);
	}
	return 0;
}

static
const char * is_matching_fence_node(xmlNodePtr node, double height) {
	xmlNodePtr other_node;

	if (
		(other_node = is_basic_fence_node(node)) &&
		(height > 0 && (get_box_total_height(node) - height) < SIZE_DELTA)
	) {
		return get_fenced_char(other_node);
	}
	if ( /* empty hbox are allowed for \right. */
		is_node_valid(node, "hbox", 0, 0) &&
		(is_node_valid(node->children, "param", 0, 0)) &&
		(!node->children->next) &&
		get_box_total_height(node) == 0
	) {
		return "";
	}
	return is_composite_fence_node(node, height);
}

static
void transform_left_and_right_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node, first_child, last_child;

	node = root->children;

	while (node) {
		xmlNodePtr next = node->next;
		if (
			is_node_valid(node, "hbox", 0, 0) &&
			(node->children) &&
			(first_child = node->children->next) &&
			(last_child = node->last) &&
			(first_child != last_child)
		) {
			const char * open_char, * close_char;
			double height = get_box_total_height(node);

			if (
				height > 0 &&
				(open_char = is_matching_fence_node(first_child, height)) &&
				(close_char = is_matching_fence_node(last_child, height)) &&
				(strlen(open_char) + strlen(close_char) > 0)
			) {
				xmlNodePtr fenced, content, tmp;

				fenced = xmlNewNode(0, BAD_CAST "mfenced");
				content = xmlNewNode(0, BAD_CAST "mrow");
				xmlAddChild(fenced, content);

				xmlNewProp(fenced, BAD_CAST "open", BAD_CAST open_char);
				xmlNewProp(fenced, BAD_CAST "close", BAD_CAST close_char);

				tmp = first_child->next;
				do {
					xmlNodePtr next = tmp->next;
					xmlUnlinkNode(tmp);
					xmlAddChild(content, tmp);
					tmp = next;
				} while(tmp && tmp != last_child);

				xmlAddPrevSibling(node, fenced);

				xmlUnlinkNode(first_child);
				xmlFreeNode(first_child);
				xmlUnlinkNode(last_child);
				xmlFreeNode(last_child);
				xmlUnlinkNode(node);
				xmlFreeNode(node);

				xmlTransformationPush(content, transform_left_and_right_pattern, param);
			} else {
				xmlTransformationPush(node, transform_left_and_right_pattern, param);
			}
		} else {
			xmlTransformationPush(node, transform_left_and_right_pattern, param);
		}

		node = next;
	}

}

#endif

/*
	pattern :
	<vbox>
		<hbox /> {1}
		<kern />
		<hbox /> {2}
		<kern />
	</vbox>

==>
	<munder>
		{1}
		{2}
	</munder>
*/
static
void transform_underscript_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr tmp, up, down;
		if (
			is_node_valid(node, "vbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(up = tmp->next) &&
			is_node_valid(up, "hbox", 0, 0) &&
			(tmp = up->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(down = tmp->next) &&
			is_node_valid(down, "hbox", 0, 0) &&
			(tmp = down->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			!tmp->next
		) {
			xmlNodePtr s = xmlNewNode(0, BAD_CAST "munder");
			xmlUnlinkNode(up);
			xmlAddChild(s, up);
			xmlUnlinkNode(down);
			xmlAddChild(s, down);

			xmlAddPrevSibling(node, s);
			xmlUnlinkNode(node);
			xmlFreeNode(node);

			transform_underscript_pattern(up, param);
			transform_underscript_pattern(down, param);

			next = s->next;
		} else {
			xmlTransformationPush(node, transform_underscript_pattern, param);
		}

		node = next;
	}
}

/*
	pattern :
	<vbox>
		<kern />
		<hbox /> {1}
		<kern />
		<hbox /> {2}
	</vbox>

==>
	<mover>
		{2}
		{1}
	</mover>
*/
static
void transform_overscript_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr tmp, up, down;
		if (
			is_node_valid(node, "vbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(up = tmp->next) &&
			is_node_valid(up, "hbox", 0, 0) &&
			(tmp = up->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(down = tmp->next) &&
			is_node_valid(down, "hbox", 0, 0) &&
			!down->next
		) {
			xmlNodePtr s = xmlNewNode(0, BAD_CAST "mover");
			xmlUnlinkNode(down);
			xmlAddChild(s, down);
			xmlUnlinkNode(up);
			xmlAddChild(s, up);

			xmlAddPrevSibling(node, s);
			xmlUnlinkNode(node);
			xmlFreeNode(node);

			transform_overscript_pattern(up, param);
			transform_overscript_pattern(down, param);

			next = s->next;
		} else {
			xmlTransformationPush(node, transform_overscript_pattern, param);
		}

		node = next;
	}
}

/*
	pattern :
	<vbox>
		<kern />
		<hbox /> {1}
		<kern />
		<hbox /> {2}
		<kern />
		<hbox /> {3}
		<kern />
	</vbox>

==>
	<mover>
		{2}
		{3}
		{1}
	</mover>
*/
static
void transform_underoverscript_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr tmp, up, base, down;
		if (
			is_node_valid(node, "vbox", 0, 0) &&
			(tmp = node->children) &&
			is_node_valid(tmp, "param", 0, 0) &&
			(tmp = tmp->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(up = tmp->next) &&
			is_node_valid(up, "hbox", 0, 0) &&
			(tmp = up->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(base = tmp->next) &&
			is_node_valid(base, "hbox", 0, 0) &&
			(tmp = base->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			(down = tmp->next) &&
			is_node_valid(down, "hbox", 0, 0) &&
			(tmp = down->next) &&
			is_node_valid(tmp, "kern", 0, 0) &&
			!tmp->next
		) {
			xmlNodePtr s = xmlNewNode(0, BAD_CAST "munderover");
			xmlUnlinkNode(base);
			xmlAddChild(s, base);
			xmlUnlinkNode(down);
			xmlAddChild(s, down);
			xmlUnlinkNode(up);
			xmlAddChild(s, up);

			xmlAddPrevSibling(node, s);
			xmlUnlinkNode(node);
			xmlFreeNode(node);

			transform_underoverscript_pattern(base, param);
			transform_underoverscript_pattern(up, param);
			transform_underoverscript_pattern(down, param);

			next = s->next;
		} else {
			xmlTransformationPush(node, transform_underoverscript_pattern, param);
		}

		node = next;
	}
}

/*
	this is the pattern we'll try to match :

	<hbox /> | <other />
	<hbox>
		<param /> ~= shifted > 0
		...
	</hbox>
*/
static
void transform_sub_or_sup_pattern(xmlNodePtr root, xmlTransformationEntry * tparam) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr script = 0, tmp = 0, kern = 0;
		const char * param = 0, * shift = 0;

		if (
			is_node_valid_mathbox(node) &&
			(script = node->next) &&
			(
				is_node_valid(script, "hbox", 0, 0) ||
				(
					(kern = script) &&
					is_node_valid(kern, "kern", 0, 0) &&
					(script = kern->next) &&
					is_node_valid(script, "hbox", 0, 0)
				)
			) &&
			(tmp = script->children) &&
			(is_node_valid(tmp, "param", 0, 0)) &&
			(param = (const char *)tmp->children->content) &&
			(shift = strstr(param, ", shifted "))
		) {
			double v;
			xmlNodePtr subsup, row, relm;
			xmlChar * type;

			v = atof(shift + 10);

			if (v > 0) {
				type = BAD_CAST "msub";
			} else {
				type = BAD_CAST "msup";
			}

			subsup = xmlNewNode(0, type);
			xmlAddPrevSibling(node, subsup);

			xmlUnlinkNode(script);
			row = xmlNewNode(0, BAD_CAST "mrow");
			relm = script->children->next;
			while(relm) {
				xmlNodePtr next = relm->next;
				xmlUnlinkNode(relm);
				xmlAddChild(row, relm);
				relm = next;
			}
			xmlFreeNode(script);
			if (kern) {
				xmlUnlinkNode(kern);
				xmlFreeNode(kern);
			}

			xmlUnlinkNode(node);
			xmlAddChild(subsup, node);
			xmlAddChild(subsup, row);
			transform_sub_or_sup_pattern(node, tparam);
			transform_sub_or_sup_pattern(row, tparam);
			next = subsup->next;
		} else if (is_node_valid(node, "munderover", 0, 0)) {
			xmlNodePtr child = node->children;
			while (child) {
				xmlTransformationPush(child, transform_sub_or_sup_pattern, tparam);
				child = child->next;
			}
		} else {
			xmlTransformationPush(node, transform_sub_or_sup_pattern, tparam);
		}
		node = next;
	}
}

static
void transform_simple_sub_or_sup_pattern(xmlNodePtr root, xmlTransformationEntry * tparam) {
	xmlNodePtr node = root->children;
	xmlChar * type = xmlGetProp(root, BAD_CAST "type");
	if (type && strcmp((const char *)type, "display") == 0) {
		xmlFree(type);
		return;
	}
	xmlFree(type);

	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr script = 0, tmp = 0;
		const char * param = 0, * shift = 0;

		if (
			(script = node) &&
			is_node_valid(script, "hbox", 0, 0) &&
			(tmp = script->children) &&
			(is_node_valid(tmp, "param", 0, 0)) &&
			tmp->next &&
			(param = (const char *)tmp->children->content) &&
			(shift = strstr(param, ", shifted "))
		) {
			double v;
			xmlNodePtr subsup, row, relm;
			xmlChar * type;

			v = atof(shift + 10);

			if (v > 0) {
				type = BAD_CAST "msub";
			} else {
				type = BAD_CAST "msup";
			}

			subsup = xmlNewNode(0, type);
			xmlAddPrevSibling(node, subsup);
			xmlNewProp(subsup, BAD_CAST "single", BAD_CAST "1");

			xmlUnlinkNode(script);
			row = xmlNewNode(0, BAD_CAST "mrow");
			relm = script->children->next;
			while(relm) {
				xmlNodePtr next = relm->next;
				xmlUnlinkNode(relm);
				xmlAddChild(row, relm);
				relm = next;
			}
			xmlFreeNode(script);

			xmlAddChild(subsup, row);
			transform_simple_sub_or_sup_pattern(script, tparam);
			next = subsup->next;
		} else {
			xmlTransformationPush(node, transform_simple_sub_or_sup_pattern, tparam);
		}
		node = next;
	}
}

/*
	<special array start />
	<special param />
	<vbox>
		<hbox>
		<glue>
		<hbox>
		<glue>
		...
		<glue>
		<hbox>
	</vbox>
	<special array end />
*/

static
void transform_array_cell(xmlNodePtr root, xmlNodePtr cell) {
	xmlNodePtr node = root->children;
	while (node) {
		if(is_node_valid(node, "mrow", 0, 0)) {
			xmlUnlinkNode(node);
			xmlAddChild(cell, node);
			return ;
		}
		node = node->next;
	}
}

static
void transform_array_line(xmlNodePtr root, xmlNodePtr line) {
	xmlNodePtr node = root->children;
	while (node) {
		xmlNodePtr next = node->next;

		if (is_node_valid(node, "hbox", 0, 0)) {
			xmlNodePtr cell = xmlNewNode(0, BAD_CAST "mtd");
			transform_array_cell(node, cell);
			xmlAddChild(line, cell);
		}
		node = next;
	}
}

static
void transform_array_content(xmlNodePtr root, xmlNodePtr array) {
	xmlNodePtr node = root->children;
	while (node) {
		xmlNodePtr next = node->next;

		if (is_node_valid(node, "hbox", 0, 0)) {
			xmlNodePtr row = xmlNewNode(0, BAD_CAST "mtr");
			xmlAddChild(array, row);
			transform_array_line(node, row);
		}

		node = next;
	}
}

static
void transform_array_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;

	while(node) {
		xmlNodePtr next = node->next;
		xmlNodePtr vbox, tmp, end;

		if (is_node_valid(node, "special", "{::tag lxir begin(array){id=", -1) &&
			(tmp = node->next) &&
			is_node_valid(tmp, "special", "{::tag lxir empty(params){id=", -1) &&
			(vbox = tmp->next) &&
			is_node_valid(vbox, "vbox", 0, 0) &&
			(end = vbox->next) &&
			is_node_valid(end, "special", "{::tag lxir end(array){id=", -1)
		) {
			xmlNodePtr array = xmlNewNode(0, BAD_CAST "mtable");
			xmlAddPrevSibling(node, array);
			transform_array_content(vbox, array);
			do {
				tmp = array->next;
				xmlUnlinkNode(tmp);
				xmlFreeNode(tmp);
			} while(tmp != end);
			next = array->next;
		} else {
			xmlTransformationPush(node, transform_array_pattern, param);
		}
		node = next;
	}
}


/*
	this is the pattern we'll try to match :
	<hbox>
		<param />
		{...}
	</hbox>

	we will produce :

	<mrow>
		{...}
	</mrow>
*/
static
void transform_mrow_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if(is_node_valid(node, "hbox", 0, 0)) {
			xmlNodePtr param = node->children;
			if (is_node_valid(param, "param", 0, 0)) {
				xmlUnlinkNode(param);
				xmlFreeNode(param);
			}
			xmlNodeSetName(node, BAD_CAST "mrow");
		}

		xmlTransformationPush(node, transform_mrow_pattern, param);

		node = next;
	}
}

/*
	this is the pattern we'll try to match :
	<other>
		<param />
	</other>
	<other>
		<param />
	</other>
	...

	we will produce :
	<mn>, <mi> or <mo> depending on content
*/

static
xmlNodePtr insert_character_node(xmlNodePtr node, const char * font, const char * type, const char * content, int force) {
	const char * mathvariant = lfm_get_math_encoding_variant(font);
	if(!force && is_node_valid(node, type, 0, 0)) {
		xmlChar * v = xmlGetProp(node, BAD_CAST "mathvariant");
		if(!v) {
			if (mathvariant) {
				xmlSetProp(node, BAD_CAST "mathvariant", BAD_CAST mathvariant);
			}
		} else {
			if (strcmp((const char *)v, mathvariant)) {
				fprintf(stderr, "lxir: mathvariant mismatch !\n");
			}
			xmlFree(v);
		}
		xmlNodeAddContent(node, BAD_CAST content);
		return node;
	} else {
		xmlNodePtr next = xmlNewNode(NULL, BAD_CAST type);
		if (mathvariant) {
			xmlSetProp(next, BAD_CAST "mathvariant", BAD_CAST mathvariant);
		}
		xmlNodeAddContent(next, BAD_CAST content);
		xmlAddNextSibling(node, next);
		return next;
	}
}

static
xmlNodePtr insert_character_mathml_type(xmlNodePtr node, const char * chr, const char * font) {
	int c = utf8(chr);
	// fprintf(stderr, "CHAR : %s => 0x%8.8x\n", chr, c);
	if ((c >= '0' && c <= '9') || c == '.') {
		return insert_character_node(node, font, "mn", chr, 0);
	}
	if (c >= 0xF730 && c <= 0xF739) { // oldstyle
		char tmp[2];
		tmp[0] = c - 0xF730 + '0';
		tmp[1] = 0;
		xmlNodePtr cnode = insert_character_node(node, font, "mn", tmp, 0);
		xmlChar * v = xmlGetProp(node, BAD_CAST "mathvariant");
		if(!v) {
			xmlSetProp(cnode, BAD_CAST "mathvariant", BAD_CAST "script");
			xmlSetProp(cnode, BAD_CAST "tex-style", BAD_CAST "oldstyle");
		} else {
			xmlFree(v);
			fprintf(stderr, "lxir: Old style script characters error !\n");
			exit(-1);
		}
		return cnode;
	}
	if (
		c == '+' || c == '*' || c == '-' || c == '/' ||
		c == '!' || c == '<' || c == '>' || c == '(' || c == ')' ||
		c == '[' || c == ']' || c == '=' || c == '{' || c == '}' ||
		c == 0x230a || c == 0x2308 || c == 0x27e8 || c == 0x27e9 ||
		c == 0x2309 || c == 0x230b || c == 0x00b1 || c == 0x2213 ||
		c == 0x005c || c == 0x00b7 || c == 0x00d7 || c == 0x2217 ||
		c == 0x22c6 || c == 0x22c4 || c == 0x25E6 || c == 0x2022 ||
		c == 0x00F7 || c == 0x2229 || c == 0x222a || c == 0x228e ||
		c == 0x2293 || c == 0x2294 || c == 0x22b2 || c == 0x22b3 ||
		c == 0x2240 || c == 0x25ef || c == 0x25b3 || c == 0x25bd ||
		c == 0x2228 || c == 0x2227 || c == 0x2295 || c == 0x2296 ||
		c == 0x2298 || c == 0x2299 || c == 0x2020 || c == 0x2021 ||
		c == 0x2210 || c == 0x2264 || c == 0x227a || c == 0x227c ||
		c == 0x226a || c == 0x2282 || c == 0x2286 || c == 0x2291 ||
		c == 0x2208 || c == 0x22a2 || c == 0x2323 || c == 0x2322 ||
		c == 0x2265 || c == 0x227b || c == 0x227d || c == 0x226b ||
		c == 0x2283 || c == 0x2287 || c == 0x2292 || c == 0x220b ||
		c == 0x22a3 || c == 0x007c || c == 0x2225 || c == 0x2261 ||
		c == 0x223c || c == 0x2243 || c == 0x224d || c == 0x2248 ||
		c == 0x221d || c == 0x22a5 || c == 0x2297 || c == 0x2190 ||
		c == 0x2191 || c == 0x21d0 || c == 0x21d1 || c == 0x2192 ||
		c == 0x2193 || c == 0x21d2 || c == 0x21d3 || c == 0x2194 ||
		c == 0x2195 || c == 0x21d4 || c == 0x21d5 || c == 0x2197 ||
		c == 0x2198 || c == 0x21bc || c == 0x21c0 || c == 0x2199 ||
		c == 0x21bd || c == 0x21c1 || c == 0x2196 || c == 0x2212
	) {
		return insert_character_node(node, font, "mo", chr, 1);
	}

	return insert_character_node(node, font, "mi", chr, 1);
}

static
char * make_temp_copy(char const * mathchar, int stopchar) {
	char * buffer;
	int len;
	char * pos = strchr(mathchar, stopchar);
	if (!pos) {
		return 0;
	}
	len = pos - mathchar;
	buffer = malloc(len + 1);
	memcpy(buffer, mathchar, len);
	buffer[len] = 0;
	return buffer;
}

static
char * content_from_mathchar(char const * mchar) {
	char * buffer;
	char * token;
	char * result;

	buffer = make_temp_copy(mchar, '}');
	if (!buffer) return 0;
	result = malloc(strlen(buffer) + 1);
	if (!result) {
		fprintf(stderr, "lxir: Allocation error while coding mathchar\n");
		exit(-1);
	}
	token = strtok(buffer, ",");
	char * pos = result;
	while (token) {
		int value = strtol(token, 0, 16);
		pos = to_utf8(value, pos);
		token = strtok(0, ",");
	}
	free(buffer);
	buffer = strdup(result);
	free(result);
	return buffer;
}

extern
char * replace_entities(const char *);

static
void cleanup_backspaces(char * data) {
	while (true) {
		char * p = strchr(data, '\\');
		if (!p) return;
		memmove(p, p + 1, strlen(p));
		data = p + 1;
	}
}

static
char * content_from_mathcontent(char const * mcontent) {
	char * content = make_temp_copy(mcontent, '}');
	char * rcontent = replace_entities(content);
	free(content);
	cleanup_backspaces(rcontent);
	return rcontent;
}

static
xmlNodePtr add_content_to_node(xmlNodePtr node, xmlChar const * type, xmlChar * content) {
	xmlNodePtr list = 0;
	xmlNodePtr chr;
	int result = xmlParseBalancedChunkMemory(node->doc, NULL, NULL, 0, content, &list);
	if (result == 0) {
		if (list->next || list->children || list->type == XML_ELEMENT_NODE)
			type = BAD_CAST "mrow";
		chr = xmlNewNode(NULL, type);
		xmlAddChildList(chr, list);
		// xmlFreeNodeList(list);
	} else {
		chr = xmlNewNode(NULL, type);
		xmlNodeAddContent(chr, content);
	}
	free(content);
	xmlAddPrevSibling(node, chr);
	xmlUnlinkNode(node);
	xmlFreeNode(node);
	return chr;
}

static
void transform_mathsym_patterns(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;

		if(is_node_valid(node, "special", "{::tag lxir empty(", 18)) {
			const char * type, * mathtype, * mathchar, * mathcontent, *no_html;

			type = (const char *) node->children->children->content + 20;
			mathtype = strstr(type, "{mathtype=");
			mathchar = strstr(type, "{mathchar=");
			mathcontent = strstr(type, "{mathcontent=");
			no_html = strstr(type, "{no-html=true}");
			if(mathtype) {
				mathtype += 10;
				type = "mi";
				if(strncmp(mathtype, "ord}", 4) == 0)
					type = "mi";
				else if(strncmp(mathtype, "op}", 3) == 0)
					type = "mo";
				else if(strncmp(mathtype, "rel}", 4) == 0)
					type = "mo";
				else if(strncmp(mathtype, "punct}", 6) == 0)
					type = "mo";
				if(type) {
					char * content = 0;
					if (mathcontent) {
						content = content_from_mathcontent(mathcontent + 13);
					} else if(mathchar) {
						content = content_from_mathchar(mathchar + 10);
					}
					if (content) {
						xmlNodePtr chr = add_content_to_node(node, BAD_CAST type, BAD_CAST content);
						if (no_html)
							xmlSetProp(chr, BAD_CAST "no-html", BAD_CAST "true");
						next = chr->next;
					}
				}
			}
		} else {
			xmlTransformationPush(node, transform_mathsym_patterns, param);
		}
		node = next;
	}
}

static
void transform_string_patterns(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "other", 0, 0) && node->children && node->children->children && node->children->children->content) {
			struct translation_info trans;
			char * type/*, * slash*/;
			int len;
			xmlNodePtr prev, tmp, end = node->next;
			type = strdup((const char *)node->children->children->content);
			*strchr(type, ' ') = 0;
			len = strlen(type);
/*
			slash = strchr(type, '/');
			if (slash) *slash = 0;
*/
			if(lfm_get_math_encoding_map(type, &trans)) {
				fprintf(stderr, "lxir: Unable to find encoding for math font \"%s\"\n", type);
				exit(-1);
			}
			tmp = xmlNewNode(0, BAD_CAST "tmp");
			xmlAddPrevSibling(node, tmp);
			prev = insert_character_mathml_type(tmp,
				trans_fenced_char(get_charcode(node->children->children->content + len + 1, &trans)), type);

			while (end && is_node_valid(end, "other", type, -1)) {
				prev = insert_character_mathml_type(prev,
					trans_fenced_char(get_charcode(end->children->children->content + len + 1, &trans)), type);
				end = end->next;
			}
			next = end;

			while(node != end) {
				xmlNodePtr next = node->next;
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				node = next;
			}

			xmlUnlinkNode(tmp);
			xmlFreeNode(tmp);
		} else {
			xmlTransformationPush(node, transform_string_patterns, param);
		}
		node = next;
	}
}

/*
	<special> ::tag lxir begin math-above-{1}
	<*>
	<special> ::tag lxir end math-above-{1}

	->

	<math-above type={1}>
		<*>
	</math-above>
*/
static
void transform_math_above1(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "special", "{::tag lxir begin(math-above-", 29)) {
			xmlNodePtr last, tmp;
			const char *type = (const char *) node->children->children->content + 29;
			char * real_type = strdup(type);
			char * endpos = strchr(real_type, ')');
			assert(endpos);
			*endpos = 0;
			char * endtag = malloc(29 + strlen(type));
			int endlen;
			strcpy(endtag, "{::tag lxir end(math-above-");
			strcat(endtag, real_type);
			strcat(endtag, ")");
			endlen = strlen(endtag);
			tmp = xmlNewNode(NULL, BAD_CAST "math-above");
			xmlSetProp(tmp, BAD_CAST "type", BAD_CAST real_type);
			xmlAddPrevSibling(node, tmp);
			free(real_type);
			last = next;
			while (last) {
				next = last->next;
				xmlUnlinkNode(last);
				if (is_node_valid(last, "special", endtag, endlen)) {
					break;
				} else {
					xmlAddChild(tmp, last);
				}
				last = next;
			}
			if (last) {
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				xmlFreeNode(last);
			} else {
				fprintf(stderr, "lxir: math-above without endtag !\n");
				exit(-1);
			}
			free(endtag);
		} else {
			xmlTransformationPush(node, transform_math_above1, param);
		}
		node = next;
	}
}

/*
	<math-above type="{1}>
		<m{ion} />
	</math-above>

	->

	<m{ion} /> + *diacritic
*/
static
void transform_math_above2(xmlNodePtr root, xmlTransformationEntry * param) {

	xmlNodePtr node = root->children;

	while (node) {
		xmlNodePtr child, next;
		next = node->next;
		if (
			is_node_valid(node, "math-above", 0, 0) &&
			(child = node->children) &&
			(
				is_node_valid(child, "mo", 0, 0) ||
				is_node_valid(child, "mi", 0, 0) ||
				is_node_valid(child, "mn", 0, 0)
			) &&
			(!child->next)
		) {
			const xmlChar * diacritic;
			xmlChar * type = xmlGetProp(node, BAD_CAST "type");
			xmlUnlinkNode(child);
			xmlAddPrevSibling(node, child);
			if (strcmp(type, "bar") == 0) {
				diacritic = "̅";
			} else if (strcmp(type, "dot") == 0) {
				diacritic = "̇";
			} else if (strcmp(type, "hat") == 0) {
				diacritic = "̂";
			} else {
				fprintf(stderr, "lxir: unknown math-above type `%s'\n", type);
				diacritic = "";
			}
			xmlFree(type);
			xmlTextConcat(child->children, diacritic, strlen(diacritic));
			xmlUnlinkNode(node);
			xmlFreeNode(node);
		} else if (is_node_valid(node, "math-above", 0, 0)) {
			const xmlChar * symbol;
			xmlChar * type = xmlGetProp(node, BAD_CAST "type");
			xmlNodePtr mover = xmlNewNode(NULL, BAD_CAST "mover");
			xmlSetProp(mover, BAD_CAST "accent", BAD_CAST "true");
			xmlNodePtr contener = mover;
			if (node->children->next) {
				contener = xmlNewNode(NULL, BAD_CAST "mrow");
				xmlAddChild(mover, contener);
			}
			while (node->children) {
				xmlNodePtr child = node->children;
				xmlUnlinkNode(child);
				xmlAddChild(contener, child);
			}
			if (strcmp(type, "bar") == 0) {
				symbol = "¯";
			} else if (strcmp(type, "dot") == 0) {
				symbol = "˙";
			} else if (strcmp(type, "hat") == 0) {
				symbol = "ˆ";
			} else {
				fprintf(stderr, "lxir: unknown math-above type `%s'\n", type);
				symbol = "";
			}
			xmlFree(type);
			xmlNewTextChild(mover, NULL, BAD_CAST "mo", symbol);
			xmlAddPrevSibling(node, mover);
			xmlUnlinkNode(node);
			xmlFreeNode(node);
		} else {
			xmlTransformationPush(node, transform_math_above2, param);
		}
		node = next;
	}
}

static
void drop_remaining_tex_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if(
			is_node_valid(node, "write", 0, 0) ||
			is_node_valid(node, "penalty", 0, 0) ||
			is_node_valid(node, "glue", 0, 0) ||
			is_node_valid(node, "special", 0, 0) ||
			is_node_valid(node, "discretionary", 0, 0) ||
			is_node_valid(node, "leaders", 0, 0) ||
			is_node_valid(node, "kern", 0, 0)
		) {
			xmlUnlinkNode(node);
			xmlFreeNode(node);
		} else {
			xmlTransformationPush(node, drop_remaining_tex_nodes, param);
		}
		node = next;
	}
}

/*
static
void drop_extra_sub_or_sup(xmlNodePtr root) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if(is_node_valid(node, "msub", 0, 0) ||
			is_node_valid(node, "msup", 0, 0) ||
			is_node_valid(node, "mfrac", 0, 0) &&
			(	!node->children ||
				!node->children->next)) {

			while(node->children) {
				xmlNodePtr c = node->children;
				drop_extra_sub_or_sup(c);
				if (c == node->children) {
					xmlUnlinkNode(c);
					xmlAddPrevSibling(node, c);
				}
			}
			xmlUnlinkNode(node);
			xmlFreeNode(node);
		}
		node = next;
	}
}
*/

static
void drop_empty_mrows(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "mrow", 0, 0)) {
			xmlChar * p = xmlGetProp(node, BAD_CAST "begin-id");
			if (p) { xmlFree(p); node = next; continue; }
			p = xmlGetProp(node, BAD_CAST "end-id");
			if (p) { xmlFree(p); node = next; continue; }

			if (!node->children) {
				xmlUnlinkNode(node);
				xmlFreeNode(node);
			} else if (!node->children->next) {
				xmlNodePtr c = node->children;
				xmlUnlinkNode(c);
				xmlAddPrevSibling(node, c);
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				next = c;
			} else if (!node->next && node->parent->children == node && is_node_valid(node->parent, "mrow", 0, 0)) {

				while (node->children) {
					xmlNodePtr c = node->children;
					xmlUnlinkNode(c);
					xmlAddPrevSibling(node, c);
				}
				next = node->parent;
				xmlUnlinkNode(node);
				xmlFreeNode(node);
			} else {
				xmlTransformationPush(node, drop_empty_mrows, param);
			}
		} else {
			xmlTransformationPush(node, drop_empty_mrows, param);
		}
		node = next;
	}
}

static
void drop_empty_vbox(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "vbox", 0, 0) &&
			node->children &&
/*			is_node_valid(node->children, "param", 0, 0)
		) {
			if (!node->children->next) {
				xmlUnlinkNode(node);
				xmlFreeNode(node);
			} else if (!node->children->next->next) {
				// single, remove
				xmlNodePtr content = node->children->next;
				xmlUnlinkNode(content);
				xmlAddPrevSibling(node, content);
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				xmlTransformationPush(content, drop_empty_vbox, param);
			} else {
				xmlTransformationPush(node, drop_empty_vbox, param);
			}
*/
			is_node_valid(node->children, "param", 0, 0) &&
			!node->children->next
		) {
			xmlUnlinkNode(node);
			xmlFreeNode(node);
		} else {
			xmlTransformationPush(node, drop_empty_vbox, param);
		}
		node = next;
	}
}

static
void transform_mtable_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "vbox", 0, 0) &&
			node->children &&
			is_node_valid(node->children, "param", 0, 0)
		) {
			xmlNodePtr children, mtable = xmlNewNode(0, BAD_CAST "mtable");
			xmlAddPrevSibling(node, mtable);

			children = node->children->next;
			while(children) {
				xmlNodePtr next = children->next;
				xmlNodePtr mtr = xmlNewNode(0, BAD_CAST "mtr");
				xmlAddChild(mtable, mtr);
				xmlNodePtr mtd = xmlNewNode(0, BAD_CAST "mtd");
				xmlAddChild(mtr, mtd);
				xmlUnlinkNode(children);
				xmlAddChild(mtd, children);
				xmlTransformationPush(children, transform_mtable_pattern, param);
				children = next;
			}

			xmlUnlinkNode(node);
			xmlFreeNode(node);
		} else {
			xmlTransformationPush(node, transform_mtable_pattern, param);
		}
		node = next;
	}
}

static
int xmlChildElementCount_(xmlNodePtr node) {
	int count = 0;
	xmlNodePtr child = node->children;
	while (child) {
		if(child->type == XML_ELEMENT_NODE) ++count;
		child = child->next;
	}
	return count;
}

static
int is_dual_math_node(xmlNodePtr node) {
	if (	is_node_valid(node, "msub", 0, 0) ||
			is_node_valid(node, "msup", 0, 0) ||
			is_node_valid(node, "mover", 0, 0) ||
			is_node_valid(node, "munder", 0, 0))
		return xmlChildElementCount_(node) <= 2;
	if (	is_node_valid(node, "msubsup", 0, 0) ||
			is_node_valid(node, "munderover", 0, 0))
		return xmlChildElementCount_(node) <= 3;
	return 0;
}

static
void merge_mn_sequence(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		if (is_node_valid(node, "mn", 0, 0) &&
			node->next && is_node_valid(node->next, "mn", 0, 0) &&
			!is_dual_math_node(node->parent)
		) {
			xmlChar * content = xmlNodeGetContent(node->next);
			xmlNodeAddContent(node, content);
			xmlFree(content);
			xmlUnlinkNode(node->next);
		} else {
			xmlTransformationPush(node, merge_mn_sequence, param);
			node = node->next;
		}
	}
}

static
int mathvariant_equal(xmlNodePtr n1, xmlNodePtr n2) {
	int result = 0;
	xmlChar * mv1 = xmlGetProp(n1, BAD_CAST "mathvariant");
	xmlChar * mv2 = xmlGetProp(n2, BAD_CAST "mathvariant");
	if (!mv1 && !mv2) return 1;
	if (mv1 && mv2) result = !strcmp(mv1, mv2);
	xmlFree(mv1);
	xmlFree(mv2);
	return result;
}

static
void merge_mi_sequence(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		if (is_node_valid(node, "mi", 0, 0) &&
			node->next && is_node_valid(node->next, "mi", 0, 0) &&
			mathvariant_equal(node, node->next)
		) {
			xmlChar * content = xmlNodeGetContent(node->next);
			xmlNodeAddContent(node, content);
			xmlFree(content);
			xmlUnlinkNode(node->next);
		} else {
			xmlTransformationPush(node, merge_mi_sequence, param);
			node = node->next;
		}
	}
}

static
void merge_menclose_sequence(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "menclose", 0, 0) &&
			node->children &&
			is_node_valid(node->children, "menclose", 0, 0) &&
			!node->children->next
		) {
			xmlNodePtr inner = node->children;
			xmlNodePtr content = inner->children;
			char * arg1 = (char *)xmlGetProp(node, BAD_CAST "notation");
			char * arg2 = (char *)xmlGetProp(inner, BAD_CAST "notation");
			if (strcmp(arg1, arg2)) {
				int len = strlen(arg1) + strlen(arg2) + 2;
				char * buffer = malloc(len);
				strcpy(buffer, arg1);
				strcat(buffer, " ");
				strcat(buffer, arg2);
				xmlSetProp(node, BAD_CAST "notation", buffer);
				free(buffer);
				xmlUnlinkNode(content);
				xmlUnlinkNode(inner);
				xmlAddChild(node, content);
				xmlFreeNode(inner);
				xmlTransformationPush(node, merge_menclose_sequence, param);
			} else {
				xmlTransformationPush(inner, merge_menclose_sequence, param);
			}
			xmlFree(arg1);
			xmlFree(arg2);
		} else {
			xmlTransformationPush(node, merge_menclose_sequence, param);
		}
		node = next;
	}
}

/*
    <mo mathvariant="normal">[</mo>
    <mi mathvariant="italic">entity</mi>
    <mo mathvariant="normal">!</mo>
    <mi mathvariant="normal">#</mi>
	...
    <mo mathvariant="normal">!</mo>
    <mo mathvariant="normal">]</mo>
    <mi mathvariant="normal">c</mi>
*/

static
int is_math_text_valid(xmlNodePtr node, char const * type, char const * content) {
	int result =
		node &&
		node->type == XML_ELEMENT_NODE &&
		strcmp((const char *)node->name, type) == 0;
	if (!result || !content) return result;

	return node->children &&
		node->children->type == XML_TEXT_NODE &&
		node->children->content &&
		strcmp((const char *)node->children->content, content) == 0;
}

static
void math_strtol_node(xmlNodePtr node, xmlChar const * buffer) {
	xmlChar chr[10];
	int value;
	if (*buffer++ != '#') {
		fprintf(stderr, "lxir: Invalid entity code '%s' found\n", buffer);
		xmlAddChild(node, xmlNewComment(buffer));
		return;
	}
	if (*buffer == 'x')
		value = strtol(buffer + 1, NULL, 16);
	else
		value = strtol(buffer, NULL, 0);
	to_utf8(value, chr);
	// fprintf(stderr, "math_strtol_node '%s' %d '%s'\n", buffer, value, chr);
	xmlNodeAddContent(node, chr);
}

static
void replace_entities_in_math(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
    xmlNodePtr temp, start, end, cont, child;
	while(node) {
		if (is_math_text_valid(node, "mo", "[") &&
			(temp = node->next) && is_math_text_valid(temp, "mi", "entity") &&
			(temp = temp->next) && is_math_text_valid(temp, "mo", "!") &&
			(temp = temp->next) && is_math_text_valid(temp, "mi", 0)
		) {
			start = temp;
			end = temp;
			while (end) {
				if (is_math_text_valid(end, "mo", "!")) { break; }
				end = end->next;
			}
            if (start && end && (cont = end->next) && is_math_text_valid(cont, "mo", "]")) {
                xmlNodePtr n = start;
                xmlBufferPtr buffer = xmlBufferCreate();
                while (n != end) {
                    xmlChar * content = xmlNodeGetContent(n);
                    xmlBufferCat(buffer, content);
                    xmlFree(content);
                    n = n->next;
                }
                n = xmlNewNode(0, BAD_CAST "mo");
                xmlSetProp(n, BAD_CAST "entity", BAD_CAST "1");
                math_strtol_node(n, xmlBufferContent(buffer));
                xmlBufferFree(buffer);
                xmlAddPrevSibling(node, n);

                temp = cont->next;
                while (node != temp) {
                    xmlNodePtr next = node->next;
                    xmlUnlinkNode(node);
                    node = next;
                }
            } else if (start && end &&
                       (cont = end->next) && is_dual_math_node(cont) &&
                       (child = cont->children) && is_math_text_valid(child, "mo", "]")) {
                xmlNodePtr n = start;
                xmlBufferPtr buffer = xmlBufferCreate();
                while (n != end) {
                    xmlChar * content = xmlNodeGetContent(n);
                    xmlBufferCat(buffer, content);
                    xmlFree(content);
                    n = n->next;
                }
                n = xmlNewNode(0, BAD_CAST "mo");
                xmlSetProp(n, BAD_CAST "entity", BAD_CAST "1");
                math_strtol_node(n, xmlBufferContent(buffer));
                xmlBufferFree(buffer);
                xmlAddPrevSibling(child, n);

                while (node != cont) {
                    xmlNodePtr next = node->next;
                    xmlUnlinkNode(node);
                    node = next;
                }
                xmlUnlinkNode(child);
                node = cont;
            } else {
				node = temp;
			}
		} else {
			xmlTransformationPush(node, replace_entities_in_math, param);
			node = node->next;
		}
	}
}

void xmlRegisterMathTransformations() {
#define DEF(x) xmlTransformationRegister("math", #x, x, 0);
	DEF(transform_inline_math)
	DEF(transform_left_and_right_pattern)
	DEF(transform_underscript_pattern)
	DEF(transform_overscript_pattern)
	DEF(transform_underoverscript_pattern)
	DEF(transform_over_pattern)
	DEF(transform_subsup_pattern)
	DEF(transform_sub_or_sup_pattern)
	DEF(transform_simple_sub_or_sup_pattern)
	DEF(transform_sqrt_pattern)
	DEF(transform_underline_pattern)
	DEF(transform_overline_pattern)
	DEF(transform_mathsym_patterns)
	DEF(transform_math_above1)
	DEF(transform_math_above2)
	DEF(transform_array_pattern)
	DEF(drop_remaining_tex_nodes)
	DEF(transform_string_patterns)
	DEF(transform_mrow_pattern)
	DEF(drop_empty_mrows)
	DEF(drop_empty_vbox)
	DEF(transform_mtable_pattern)
	DEF(merge_mn_sequence)
	DEF(merge_mi_sequence)
	DEF(merge_menclose_sequence)
	DEF(replace_entities_in_math)
	DEF(decode_box_parameters)
#undef DEF
}

xmlDocPtr mathlog_read_file(const char * logname, int save_raw_log) {
	xmlNodePtr root, math;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

	root = xmlNewNode(0, BAD_CAST "tex-log");
	xmlDocSetRootElement(doc, root);
	read_log_file(root, logname);
	if (save_raw_log)
		xmlSaveFormatFileEnc("temp-rawlog.xml", doc, "UTF-8", 1);
	math = extract_all_math(root);
	xmlDocSetRootElement(doc, math);
	xmlFreeNode(root);
	xmlTransformationApplyList("math", &doc);
	if (save_raw_log)
		xmlSaveFormatFileEnc("temp-mathlog.xml", doc, "UTF-8", 1);
	return doc;
}

static
xmlNodePtr get_math_expr(xmlNodePtr root, int begin_id, int end_id) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr r;
		xmlChar * prop;
		if (
			(is_node_valid(node, "math", 0, 0) || is_node_valid(node, "mrow", 0, 0)) &&
			(prop = xmlGetProp(node, BAD_CAST "begin-id")) &&
			atoi((const char *)prop) == begin_id &&
			(xmlFree(prop), 1) &&
			(prop = xmlGetProp(node, BAD_CAST "end-id")) &&
			atoi((const char *)prop) == end_id && (xmlFree(prop), 1)
		) {
			return node;
		}

		r = get_math_expr(node, begin_id, end_id);
		if (r) return r;

		node = node->next;
	}
	return 0;
}

xmlNodePtr mathlog_copy_expr(xmlDocPtr src, xmlDocPtr doc, int begin_id, int end_id) {
	xmlNodePtr root = src->children;
	xmlNodePtr copy, node = get_math_expr(root, begin_id, end_id);
	if (!node) return 0;

	copy = xmlDocCopyNode(node, doc, 1);
	if (copy) {
		xmlNodeSetName(copy, BAD_CAST "math"); /* it could be mrow */
	}
	return copy;
}


#ifdef TEST
static
void save_math_nodes(xmlNodePtr root) {
	xmlNodePtr node = root->children;
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_valid(node, "math", 0, 0)) {
			char filename[128];
			xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
			xmlUnlinkNode(node);
			xmlDocSetRootElement(doc, node);
			xmlNewProp(node, BAD_CAST "xmlns", BAD_CAST "http://www.w3.org/1998/Math/MathML");
			sprintf(filename, "equation-%s.xml", xmlGetProp(node, BAD_CAST "begin-id"));
			xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
			xmlFreeDoc(doc);
		}
		node = next;
	}
}

static
void init_transformations() {
	xmlRegisterMathTransformations();
	xmlTransformationInit("transformations.xml");
}

int main(int argc, char * argv[]) {
	xmlDocPtr doc;

    LIBXML_TEST_VERSION;
	xmlSubstituteEntitiesDefault(1);
	xmlLoadExtDtdDefaultValue = 1;
	kpse_set_program_name(argv[0], "lxir");

	init_transformations();

	lfm_init();

	if (argc < 2) {
		fprintf(stderr, "Usage : %s <log>\n", argv[0]);
		exit(-1);
	}

	doc = mathlog_read_file(argv[1]);
	save_math_nodes(doc->children);
	xmlFreeDoc(doc);
	return 0;
}
#endif
