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

#if USE_KPSE
#include <kpathsea/kpathsea.h>
#endif

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
void to_utf8(int c, char * b) {
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
		fgets(line, 1024, f);
		{
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
		is_node_valid(node, "other", 0, 0) ||
		is_node_valid(node, "mrow", 0, 0) ||
		is_node_valid(node, "mfenced", 0, 0)
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
const char * get_other_content(xmlNodePtr node) {
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
				fprintf(stderr, "Unable to find mathoff after mathon begining at ID:%s !\n", id);
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
					fprintf(stderr, "Unable to find the end of display section begining at ID:%s !\n", id);
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
				fprintf(stderr, "Unable to find mathoff after mathon begining at ID:%s !\n", id);
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
		<hbox> {...} </hbox>
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
			is_node_valid(row1, "hbox", 0, 0) &&
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
					is_node_valid(row2, "hbox", 0, 0)
				) || (
					is_node_valid(tmp, "hbox", 0, 0) &&
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
	if(
		is_node_valid(node, "mi", 0, 0) &&
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
		const char * prop;
		xmlNodePtr next = node->next;
		xmlNodePtr tmp = 0, content = 0;
		if(
			(
				is_sqrt_symbol_node(node) ||
				(
					is_node_valid(node, "msup", 0, 0) &&
					(prop = (const char *) xmlGetProp(node, BAD_CAST "single")) &&
					(strcmp(prop, "1") == 0) &&
					(tmp = node->children) &&
					is_sqrt_symbol_node(tmp)
				) ||
				(
					is_node_valid(node, "vbox", 0, 0) &&
					is_sqrt_symbol_list(node->children->next)
				)
			) &&
			(tmp = node->next) &&
			is_node_valid(tmp, "menclose", 0, 0) &&
			(prop = (const char *) xmlGetProp(tmp, BAD_CAST "notation")) &&
			(strcmp(prop, "top") == 0) &&
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
			is_node_valid(content, "hbox", 0, 0) &&
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

			xmlTransformationPush(content, transform_underline_pattern, param);

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

	NOTE : AFTER sqrt !!!
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
			is_node_valid(content, "hbox", 0, 0) &&
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

			xmlTransformationPush(content, transform_overline_pattern, param);

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
		if (code == 012 || code == 034 || code == 052) return "⟨";
		if (code == 013 || code == 035 || code == 053) return "⟩";
		if (code == 016 || code == 036 || code == 054) return "/";
		if (code == 017 || code == 037 || code == 055) return "\\";
		if (code == 014) return "|";
		if (code == 015) return "∥";
		fprintf(stderr, "lxir : unknown fenced char %d\n", code);
	}
	return chr;
}

static
const char * get_fenced_char(xmlNodePtr node) {
	return trans_fenced_char(get_other_content(node));
}

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
		} else {
			xmlTransformationPush(node, transform_sub_or_sup_pattern, tparam);
		}
		node = next;
	}
}

static
void transform_simple_sub_or_sup_pattern(xmlNodePtr root, xmlTransformationEntry * tparam) {
	xmlNodePtr node = root->children;
	const char * type = (const char *)xmlGetProp(root, BAD_CAST "type");
	if (type && strcmp(type, "display") == 0) return;

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
xmlNodePtr insert_character_node(xmlNodePtr node, const char * type, const char * content, int force) {
	if(!force && is_node_valid(node, type, 0, 0)) {
		xmlNodeAddContent(node, BAD_CAST content);
		return node;
	} else {
		xmlNodePtr next = xmlNewNode(NULL, BAD_CAST type);
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
		return insert_character_node(node, "mn", chr, 0);
	}
	if (c >= 0xF730 && c <= 0xF739) { // oldstyle
		char tmp[2];
		tmp[0] = c - 0xF730 + '0';
		tmp[1] = 0;
		xmlNodePtr cnode = insert_character_node(node, "mn", tmp, 0);
		if(!xmlGetProp(cnode, BAD_CAST "mathvariant")) {
			xmlSetProp(cnode, BAD_CAST "mathvariant", BAD_CAST "script");
			xmlSetProp(cnode, BAD_CAST "tex-style", BAD_CAST "oldstyle");
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
		return insert_character_node(node, "mo", chr, 1);
	}

	return insert_character_node(node, "mi", chr, 1);
}

static
void transform_mathsym_patterns(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;

		if(is_node_valid(node, "special", "{::tag lxir empty(", 18)) {
			const char * type, * mathtype, * mathchar, * mathcontent;

			type = (const char *) node->children->children->content + 20;
			mathtype = strstr(type, "{mathtype=");
			mathchar = strstr(type, "{mathchar=");
			mathcontent = strstr(type, "{mathcontent=");
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
						content = strdup(mathcontent + 13);
						*strchr(content, '}') = 0;
					} else if(mathchar) {
						int value = strtol(mathchar + 10, 0, 16);
						content = malloc(5);
						to_utf8(value, content);
					}
					if (content) {
						xmlNodePtr chr = xmlNewNode(NULL, BAD_CAST type);
						xmlNodeAddContent(chr, BAD_CAST content);
						free(content);
						xmlAddPrevSibling(node, chr);
						xmlUnlinkNode(node);
						xmlFreeNode(node);
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
			char * type, * slash;
			int len;
			xmlNodePtr prev, tmp, end = node->next;
			type = strdup((const char *)node->children->children->content);
			*strchr(type, ' ') = 0;
			len = strlen(type);
			slash = strchr(type, '/');
			if (slash) *slash = 0;
			if(lfm_get_math_encoding_map(type, &trans)) {
				fprintf(stderr, "Unable to find encoding for math font \"%s\"\n", type);
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
*/

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
		if (is_node_valid(node, "mrow", 0, 0) && !xmlGetProp(node, BAD_CAST "begin-id") && !xmlGetProp(node, BAD_CAST "end-id")) {
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
				xmlUnlinkNode(children);
				xmlAddChild(mtr, children);
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
	DEF(transform_array_pattern)
	DEF(drop_remaining_tex_nodes)
	DEF(transform_string_patterns)
	DEF(transform_mrow_pattern)
	DEF(drop_empty_mrows)
	DEF(drop_empty_vbox)
	DEF(transform_mtable_pattern)
#undef DEF
}

xmlDocPtr mathlog_read_file(const char * logname) {
	xmlNodePtr root, math;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

	root = xmlNewNode(0, BAD_CAST "tex-log");
	xmlDocSetRootElement(doc, root);
	read_log_file(root, logname);
#if TEST
	xmlSaveFormatFileEnc("temp-log.xml", doc, "UTF-8", 1);
#endif
	math = extract_all_math(root);
	xmlDocSetRootElement(doc, math);
	xmlFreeNode(root);
	xmlTransformationApplyList("math", &doc);
#if TEST
	xmlSaveFormatFileEnc("temp-math.xml", doc, "UTF-8", 1);
#endif
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
			(prop = xmlGetProp(node, BAD_CAST "end-id")) &&
			atoi((const char *)prop) == end_id
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
#if USE_KPSE
	kpse_set_program_name(argv[0], "lxir");
#endif

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
