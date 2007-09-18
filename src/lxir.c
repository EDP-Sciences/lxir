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
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libdvi.h>
#include <libfontmap.h>

#if USE_KPSE
#include <kpathsea/kpathsea.h>
#endif

#include "transformations.h"
#include "utf8string.h"
#include "log2xml.h"

#define REPORT_ALL 0

int do_not_add_space = 0;

static char pbuffer[128];

void print_font(xmlNodePtr root, dvifont_t * font) {
	xmlNodePtr node, size;
	
	node = xmlNewChild(root, NULL, BAD_CAST "font", NULL);
	
	sprintf(pbuffer, "%d", font->index);	xmlNewProp(node, BAD_CAST "index", BAD_CAST pbuffer);
	sprintf(pbuffer, "%8.8X", font->checksum);	xmlNewProp(node, BAD_CAST "checksum", BAD_CAST pbuffer);
	
	size = xmlNewChild(node, NULL, BAD_CAST "size", NULL);

	sprintf(pbuffer, "%d", font->scale);	xmlNewProp(size, BAD_CAST "scale", BAD_CAST pbuffer);
	sprintf(pbuffer, "%d", font->design_size);	xmlNewProp(size, BAD_CAST "design_size", BAD_CAST pbuffer);
	
	if (font->area && *font->area) {
		xmlNewTextChild(node, NULL, BAD_CAST "area", BAD_CAST font->area);
	}
	if (font->name && *font->name) {
		xmlNewTextChild(node, NULL, BAD_CAST "name", BAD_CAST font->name);
	}
}

void make_header(xmlNodePtr root, dvifile_t * dvi) {
	int i;
	xmlNodePtr header, size;
	
	header = xmlNewChild(root, NULL, BAD_CAST "header", NULL);
	
	sprintf(pbuffer, "%d", dvi->version); xmlNewTextChild(header, NULL, BAD_CAST "version", BAD_CAST pbuffer);
	
	size = xmlNewChild(header, NULL, BAD_CAST "size", NULL);
	
	sprintf(pbuffer, "%d", dvi->num);	xmlNewProp(size, BAD_CAST "num", BAD_CAST pbuffer);
	sprintf(pbuffer, "%d", dvi->den);	xmlNewProp(size, BAD_CAST "den", BAD_CAST pbuffer);
	sprintf(pbuffer, "%d", dvi->mag);	xmlNewProp(size, BAD_CAST "mag", BAD_CAST pbuffer);
	
	xmlNewTextChild(header, NULL, BAD_CAST "comment", BAD_CAST dvi->comment);
	
	for(i = 0; i < dvi->nbfonts; ++i)
			print_font(header, &dvi->fonts[i]);
}

void make_node(xmlNodePtr root, dvinode_header_t * node) {
	switch(node->type) {
		case DVINODE_BOP: {
				int i;
				dvinode_bop_t * bop = (dvinode_bop_t *) node;
				xmlNodePtr node = xmlNewChild(root, NULL, BAD_CAST "bop", NULL);
				
				for(i = 0; i < 10; ++i) {
					sprintf(pbuffer, "n%d", i);
					sprintf(pbuffer+64, "%d", bop->counts[i]);
					xmlNewProp(node, BAD_CAST pbuffer, BAD_CAST pbuffer+64);
				}
			} break;
		case DVINODE_TEXT: {
				dvinode_text_t * text = (dvinode_text_t *) node;
				char * content;
				xmlNodePtr node;
			
				content = malloc(text->size + 1);
				memcpy(content, text->content, text->size);
				content[text->size] = 0;

				node = xmlNewTextChild(root, NULL, BAD_CAST "text", BAD_CAST content);
			
				free(content);
				sprintf(pbuffer, "%d", text->font); xmlNewProp(node, BAD_CAST "font", BAD_CAST pbuffer);
				sprintf(pbuffer, "%d", text->h); xmlNewProp(node, BAD_CAST "h", BAD_CAST pbuffer);
				sprintf(pbuffer, "%d", text->v); xmlNewProp(node, BAD_CAST "v", BAD_CAST pbuffer);
			} break;
		case DVINODE_XXX: {
				dvinode_xxx_t * xxx = (dvinode_xxx_t *) node;
				char * content;

				content = malloc(xxx->size + 1);
				memcpy(content, xxx->content, xxx->size);
				content[xxx->size] = 0;

				xmlNewTextChild(root, NULL, BAD_CAST "xxx", BAD_CAST content);
				free(content);
			} break;
		case DVINODE_CONTROL: {
				dvinode_control_t * control = (dvinode_control_t *) node;
				char * content;
				switch(control->type) {
					case DVICONTROL_PUSH: content = "push"; break;
					case DVICONTROL_POP: content = "pop"; break;
					case DVICONTROL_DOWN: content = "down"; break;
					case DVICONTROL_RIGHT: content = "right"; break;
					case DVICONTROL_W: content = "w"; break;
					case DVICONTROL_X: content = "x"; break;
					case DVICONTROL_Y: content = "y"; break;
					case DVICONTROL_Z: content = "z"; break;
					default: content = "invalid"; break;
				}
				xmlNodePtr node = xmlNewChild(root, NULL, BAD_CAST "control", NULL);
				xmlNewProp(node, BAD_CAST "type", BAD_CAST content);
				sprintf(pbuffer, "%d", control->param); xmlNewProp(node, BAD_CAST "param", BAD_CAST pbuffer);
			} break;
		case DVINODE_RULE: {
				dvinode_rule_t * rule = (dvinode_rule_t *) node;
				xmlNodePtr node = xmlNewChild(root, NULL, BAD_CAST "rule", NULL);
				sprintf(pbuffer, "%d", rule->h); xmlNewProp(node, BAD_CAST "h", BAD_CAST pbuffer);
				sprintf(pbuffer, "%d", rule->v); xmlNewProp(node, BAD_CAST "v", BAD_CAST pbuffer);
				sprintf(pbuffer, "%d", rule->a); xmlNewProp(node, BAD_CAST "a", BAD_CAST pbuffer);
				sprintf(pbuffer, "%d", rule->b); xmlNewProp(node, BAD_CAST "b", BAD_CAST pbuffer);
			} break;
	}
}

void make_content(xmlNodePtr root, dvifile_t * dvi) {
	int i;
	dvinode_header_t * node;
	for(i = 0; i < dvi->nbpages; ++i) {
		xmlNodePtr page = xmlNewChild(root, NULL, BAD_CAST "page", NULL);
		node = dvi->pages[i];
		while(node) {
			make_node(page, node);
			node = node->next;
		}
	}
}

static
int is_valid_node(xmlNodePtr node, const char * name) {
	return node && node->type == XML_ELEMENT_NODE && 
		strcmp((const char *)node->name, name) == 0;
}

static is_valid_control_node(xmlNodePtr node, const char * type) {
	return is_valid_node(node, "control") && 
		strcmp((const char *)xmlGetProp(node, "type"), type) == 0;
}

void remove_page_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	/* this function takes page nodes out of the document */
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_valid_node(node, "page")) {
			xmlNodePtr child = node->children;
			xmlUnlinkNode(node);
			while(child) {
				xmlNodePtr nchild = child->next;
				xmlUnlinkNode(child);
				if (next) xmlAddPrevSibling(next, child);
				else xmlAddChild(root, child);
				child = nchild;
			}
			xmlFreeNode(node);
		}
		node = next;
	}
}

void remove_bop_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	/* this function takes bop nodes out of the document */
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_valid_node(node, "bop")) {
			xmlUnlinkNode(node);
			xmlFreeNode(node);
		}
		node = next;
	}
}

static
xmlChar * is_valid_accent(xmlNodePtr text1, xmlNodePtr text2) {
	xmlChar * c1 = xmlNodeGetContent(text1), * c2 = xmlNodeGetContent(text2);
	char * accent, * p1 = (char *) c1, * p2 = (char *) c2;
	int r;
	while (*p1 == ' ') ++p1;
	while (*p2 == ' ') ++p2;
	
	accent = xmlMalloc(32);
	
	r = lfm_get_accent(p1, p2, accent) || lfm_get_accent(p2, p1, accent);
	
	xmlFree(c1);
	xmlFree(c2);
	
	if (!r) {
		xmlFree(accent);
		return 0;
	}
	return accent;
}

void transform_large_accent_pattern(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next, text1, text2, last, tmp;
		xmlChar * accent;
		
		if (
			is_valid_control_node(node, "push") &&
			(tmp = node->next) && is_valid_control_node(tmp, "push") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "push") &&
			(tmp = tmp->next) && ((
				is_valid_control_node(tmp, "right") &&
				(text1 = tmp->next) && is_valid_node(text1, "text")
			) || (
				(text1 = tmp) && is_valid_node(text1, "text")
			)) &&
			(tmp = text1->next) && is_valid_control_node(tmp, "pop") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "pop") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "push") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "push") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "right") &&
			(text2 = tmp->next) && is_valid_node(text2, "text") &&
			(tmp = text2->next) && is_valid_control_node(tmp, "pop") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "pop") &&
			(tmp = tmp->next) && is_valid_control_node(tmp, "pop") &&
			(last = tmp->next) && is_valid_control_node(last, "right") &&
			(accent = is_valid_accent(text1, text2))
		) {
			/* insert the accent */
			xmlUnlinkNode(text1);
			xmlAddPrevSibling(node, text1);
			xmlNodeSetContentLen(text1, 0, 0);
			xmlNodeAddContent(text1, accent);
			xmlFree(accent);
			
			next = last->next;
			do {
				xmlNodePtr n = node->next;
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				node = n;
			} while (node != last);
		} else {
			xmlTransformationPush(node, transform_large_accent_pattern, param);
		}
		
		node = next;
	}
}

void remove_control_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	/* this function takes control nodes out of the document */
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_valid_node(node, "control")) {
			xmlNodePtr prev, last;
			
			prev = node->prev;
			
			last = node;
			while (is_valid_node(last->next, "control"))
				last = last->next;
			
			next = last->next;
			
			if (is_valid_node(prev, "text") && is_valid_node(next, "text")) {
				/* 
				We have some text nodes separated by a bunch of control nodes.
				
				We may need to insert a space depending on the type of control nodes.
				*/
				int has_space = 0;
				xmlNodePtr current = node, n;
				do {
					xmlChar * type = xmlGetProp(current, BAD_CAST "type");
					xmlChar * param = xmlGetProp(current, BAD_CAST "param");
					
					if (
						strcmp((const char *)type, "push") == 0 ||
						strcmp((const char *)type, "pop") == 0 ||
						strcmp((const char *)type, "y") == 0 ||
						strcmp((const char *)type, "down") == 0 ||
						strcmp((const char *)type, "z") == 0
					) {
						has_space = 1;
					} else if (
						strcmp((const char *) type, "w") == 0 && 
						strcmp((const char *) param, "1") == 0
					) {
						has_space = 1;
					}
					
					n = current->next;
					xmlUnlinkNode(current);
					xmlFreeNode(current);
					current = n;
				} while (current != next);
				/* merge the two text nodes if no spacing control node has been detected */
				if (!has_space) {
					xmlChar * content = xmlNodeGetContent(current);
					xmlNodeAddContent(prev, content);
					xmlFree(content);
					next = current->next;
					xmlUnlinkNode(current);
					xmlFreeNode(current);
				}
			} else {
				while(node) {
					xmlNodePtr n = node != last ? node->next : 0;
					xmlUnlinkNode(node);
					xmlFreeNode(node);
					node = n;
				}
			}
		} else {
			xmlTransformationPush(node, remove_control_nodes, param);
		}
		node = next;
	}
}

#define NODE_TYPE_EMPTY 0
#define NODE_TYPE_OPEN 1
#define NODE_TYPE_CLOSE 2
#define NODE_TYPE_SPECIAL 3

static
int is_valid_tag_name(const char * content, xmlChar ** pname, int * popen) {
	char * s, * e;
	char * name;
	int len;
	
	if(strncmp(content, "::tag lxir ", 11) != 0) return 0;
	content += 11;
	s = strchr(content, '(') + 1;
	e = strrchr(content, ')');
	len = e - s;
	name = malloc(len + 1);
	memcpy(name, s, len);
	name[len] = 0;
	*pname = (xmlChar *)name;
	
	if(strncmp(content, "begin", 5) == 0) {
		*popen = NODE_TYPE_OPEN;
		return 1;
	} else if(strncmp(content, "end", 3) == 0) {
		*popen = NODE_TYPE_CLOSE;
		return 1;
	} else if(strncmp(content, "empty", 5) == 0) {
		*popen = NODE_TYPE_EMPTY;
		return 1;
	}
	fprintf(stderr, "Unknown tag token : %s\n", content);
	return 0;
}

void fill_attributes(const char * content, xmlNodePtr attr) {
	const char * popen, *pequal, * pclose;

	while(NULL != (popen = strchr(content, '{'))) {
		char * name, * value;
		int len;
		if(!(pequal = strchr(popen, '='))) break;
		if(!(pclose = strchr(pequal, '}'))) break;
		content = pclose + 1;
		
		++popen; --pclose;
		len = (pequal - popen);
		name = malloc(len + 1);
		if(!name) break;
		memcpy(name, popen, len);
		name[len] = 0;
		len = (pclose - pequal);
		value = malloc(len + 1);
		if(!value) break;
		memcpy(value, pequal + 1, len);
		value[len] = 0;
		xmlNewProp(attr, BAD_CAST name, BAD_CAST value);
		free(name);
		free(value);
	}
}

static
int is_xxx_valid_name(xmlNodePtr node, xmlChar ** pname, int * popen, xmlNodePtr attr) {
	int len;
	char * content, * name;
	
	if(node->type != XML_ELEMENT_NODE) return 0;
	
	if(!node->children || node->children != node->last) return 0;
	
	if(!xmlNodeIsText(node->children)) return 0;
		
	content = (char*)node->children->content;
	
	if (!content) return 0;

	len = strlen(content);
	
	if(len < 4) return 0;
	
	if (content[0] == ' ') { content++; len -= 1; }
	if (content[len - 1] == ' ') { len -= 1; content[len] = 0; }

	if(content[0] == ':' && content[1] == ':') {
		int res = is_valid_tag_name(content, pname, popen);
		if (res) {
			fill_attributes(content, attr);
		}
		return res;
	} else {
		*popen = NODE_TYPE_SPECIAL;
		name = malloc(len + 1);
		memcpy(name, content, len);
		name[len] = 0;
		*pname = (xmlChar *)name;
		return 1;
	}
}

static
int is_node_env_name(xmlNodePtr node, xmlChar ** ptype, int * popen) {
	int len;
	char * name, * content = (char *)xmlGetProp(node, BAD_CAST "type");
	
	if(!content) return 0;
	
	if(strncmp(content, "Env:@", 5) == 0) {
		content += 5;
		len = strlen(content);
		char * p = strrchr(content, '@');
		if (strcmp(p, "@:End") == 0) {
			len -= 5;
			*popen = NODE_TYPE_CLOSE;
			name = malloc(len + 1);
			memcpy(name, content, len);
			name[len] = 0;
			*ptype = (xmlChar *)name;
			return 1;
		} else if (strcmp(p, "@:Begin") == 0) {
			len -= 7;
			*popen = NODE_TYPE_OPEN;
			name = malloc(len + 1);
			memcpy(name, content, len);
			name[len] = 0;
			*ptype = (xmlChar *)name;
			return 1;
		}
	}
	return 0;
}

static
int is_node_special(xmlNodePtr node, xmlChar ** ptype, xmlChar ** pvalue) {
	int len, has_at;
	char * ps, * pe;
	char * name, * content = (char *)xmlGetProp(node, BAD_CAST "type");
	
	if(!content) return 0;
	
	ps = strchr(content, ':');
	pe = strrchr(content, ':');
	if(!ps || !pe || ps == pe) return 0;
		
	has_at = (ps[1] == '@' && pe[-1] == '@');
	if (has_at) {
		ps += 1; pe -= 1;
		
		len = ps - content;
		if(len < 2 || ps[-1] != ':' || pe[1] != ':') return 0;
		len -= 1;
		if (strncmp(pe + 2, content, len) != 0) return 0;
		name = malloc(len + 1);
		memcpy(name, content, len);
		name[len] = 0;
		*ptype = (xmlChar *)name;
		
		ps += 1;
		len = pe - ps;
		name = malloc(len + 1);
		memcpy(name, ps, len);
		name[len] = 0;
		*pvalue = (xmlChar *)name;
		
		return 1;
	} else {
		len = ps - content;
		if(len < 1) return 0;
		if (strncmp(pe + 1, content, len) != 0) return 0;
		name = malloc(len + 1);
		memcpy(name, content, len);
		name[len] = 0;
		*ptype = (xmlChar *)name;
		
		ps += 1;
		len = pe - ps;
		name = malloc(len + 1);
		memcpy(name, ps, len);
		name[len] = 0;
		*pvalue = (xmlChar *)name;
		
		return 1;
	}
}

static
void relink_nodes(xmlNodePtr new_parent, xmlNodePtr node) {
	while (node) {
		xmlNodePtr next = node->next;
		xmlUnlinkNode(node);
		xmlAddChild(new_parent, node);
		node = next;
	}
}

struct node_stack_entry {
	xmlNodePtr node;
	xmlNodePtr attr;
	int type;
	xmlChar * name;
	
	struct node_stack_entry * next;
	struct node_stack_entry * prev;
};

struct node_stack {
	struct node_stack_entry * first;
	struct node_stack_entry * last;
};

static
void copy_props(xmlNodePtr node, xmlNodePtr attr, int close) {
	if (attr->properties) {
		xmlAttrPtr a = attr->properties;
		while(a) {
			if (!close) {
				xmlNewProp(node, a->name, a->children->content);
			} else {
				xmlChar * name = xmlMalloc(strlen((const char *)a->name) + 8);
				sprintf((char *)name, "close-%s", BAD_CAST a->name);
				xmlNewProp(node, name, a->children->content);
				xmlFree(name);
			}
			a = a->next;
		}
	}
}

static
struct node_stack_entry *
new_node_stack_entry(xmlNodePtr node) {
	struct node_stack_entry * entry;
	
	if (!is_valid_node(node, "xxx")) return 0;
		
	entry = (struct node_stack_entry *) malloc(sizeof(struct node_stack_entry));
	entry->node = node;
	entry->attr = xmlNewNode(NULL, BAD_CAST "temp-attribute");
	if (!is_xxx_valid_name(node, &entry->name, &entry->type, entry->attr)) {
		fprintf(stderr, "Invalid xxx node structure ! it has no name ???\n");
		exit(-1);
	}
	
	if (entry->type == NODE_TYPE_EMPTY) {
		xmlNodePtr next = node->next;
		xmlUnlinkNode(node);
		xmlFreeNode(node);
		node = xmlNewNode(NULL, BAD_CAST "node");
		xmlNewProp(node, BAD_CAST "type", entry->name);
		xmlAddPrevSibling(next, node);
		copy_props(node, entry->attr, 0);
		xmlFreeNode(entry->attr);
		free(entry);
		return 0;
	} else if (entry->type == NODE_TYPE_SPECIAL) {
		xmlNodePtr next = node->next;
		xmlUnlinkNode(node);
		xmlFreeNode(node);
		node = xmlNewNode(NULL, BAD_CAST "special");
		xmlNewProp(node, BAD_CAST "content", entry->name);
		xmlAddPrevSibling(next, node);
		copy_props(node, entry->attr, 0);
		xmlFreeNode(entry->attr);
		free(entry);
		return 0;
	}
	return entry;
}

static
void add_node_to_stack(struct node_stack * stack, struct node_stack_entry * entry) {
	entry->prev = stack->last;
	if(stack->last) {
		stack->last->next = entry;
	} else {
		stack->first = entry;
	}
	stack->last = entry;
	entry->next = 0;
}

static
void fill_node_stack(struct node_stack * stack, xmlNodePtr root) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		struct node_stack_entry * entry = new_node_stack_entry(node);
		if (entry) add_node_to_stack(stack, entry);
		node = next;
	}
}

static
void unlink_node_stack(struct node_stack * stack, struct node_stack_entry * entry) {
	if(entry->next) {
		entry->next->prev = entry->prev;
	} else {
		stack->last = entry->prev;
	}
	if(entry->prev) {
		entry->prev->next = entry->next;
	} else {
		stack->first = entry->next;
	}
	entry->prev = 0;
	entry->next = 0;
}

static
int build_hierarchy_step(struct node_stack * stack) {
	struct node_stack_entry * entry, * match;
	xmlNodePtr next, node;
	
	entry = stack->first; 
	if(!entry) return 0;
	
	/* find the first CLOSE node */
	while(entry && entry->type != NODE_TYPE_CLOSE) entry = entry->next;
	if(!entry) {
		fprintf(stderr, "Invalid xxx hierarchy, open node \"%s\" without closing nodes found\n", stack->first->name);
		exit(-1);
	}
	/* find the first OPEN node before */
	while(entry && entry->type != NODE_TYPE_OPEN) entry = entry->prev;
	if(!entry) {
		entry = stack->first;
		unlink_node_stack(stack, entry);
		
		fprintf(stderr, "Invalid xxx hierarchy, closing node \"%s\" found without matching open\nReplacing by an empty node...\n", entry->name);
		next = entry->node->next;
		xmlUnlinkNode(entry->node);
		xmlFreeNode(entry->node);
		node = xmlNewNode(NULL, BAD_CAST "node");
		xmlNewProp(node, BAD_CAST "type", entry->name);
		copy_props(node, entry->attr, 0); xmlFreeNode(entry->attr);
		xmlAddPrevSibling(next, node);
		return 1;
	}
	match = entry->next;
	/* find the matching CLOSE node after */
	while(match && match->type == NODE_TYPE_CLOSE && strcmp((const char *)match->name, (const char *)entry->name)) match = match->next;
	if(!match || match->type != NODE_TYPE_CLOSE) {
		fprintf(stderr, "Invalid xxx hierarchy, unable to find a matching close node for \"%s\"\n", entry->name);
		exit(-1);
	}
	/* place the "match" node just before the "entry->next" node */
	if (match != entry->next) {
		xmlUnlinkNode(match->node);
		xmlAddPrevSibling(entry->next->node, match->node);
	}
	
	unlink_node_stack(stack, entry);
	unlink_node_stack(stack, match);
	
	next = entry->node->next;
	xmlUnlinkNode(entry->node);
	xmlFreeNode(entry->node);
	node = xmlNewNode(NULL, BAD_CAST "node");
	xmlNewProp(node, BAD_CAST "type", entry->name);
	copy_props(node, entry->attr, 0); xmlFreeNode(entry->attr);
	copy_props(node, match->attr, 1); xmlFreeNode(match->attr);
	
	xmlAddPrevSibling(next, node);
	while(next && next != match->node) {
		xmlNodePtr nextnext = next->next;
		xmlUnlinkNode(next);
		xmlAddChild(node, next);
		next = nextnext;
	}
	if (next) {
		xmlUnlinkNode(next);
		xmlFreeNode(next);
	}
	free(entry);
	free(match);
	return 1;
}

void replace_xxx_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	struct node_stack stack = { 0, 0 };
	fill_node_stack(&stack, root);
	while(build_hierarchy_step(&stack));
}

void replace_env_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr parent = root;
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (is_valid_node(node, "node")) {
			int open;
			xmlChar * name;
			if (is_node_env_name(node, &name, &open)) {
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				if (open == NODE_TYPE_OPEN) {
					node = xmlNewChild(parent, NULL, BAD_CAST "node", NULL);
					xmlNewProp(node, BAD_CAST "type", BAD_CAST "Env");
					xmlNewProp(node, BAD_CAST "value", BAD_CAST name);
					relink_nodes(node, next);
					xmlAddChild(parent, node);
					parent = node;
				} else if(open == NODE_TYPE_CLOSE) {
					if (!is_valid_node(parent, "node")) {
						fprintf(stderr, "Invalid xxx hierarchy !, got a closing node without opening node\n");
						exit(-1);
					}
					if (strcmp((const char *)xmlGetProp(parent, BAD_CAST "type"), "Env") != 0) {
						fprintf(stderr, "Invalid xxx hierarchy !, parent is not an env node\n");
						exit(-1);
					}
					if (strcmp((const char *)xmlGetProp(parent, BAD_CAST "value"), (const char *)name) != 0) {
						fprintf(stderr, "Invalid xxx hierarchy !, got \"%s\" and expected \"%s\"\n", node->content, xmlGetProp(parent, BAD_CAST "type"));
						exit(-1);
					}
					node = parent;
					parent = node->parent;
					relink_nodes(parent, next);
				} else {
					fprintf(stderr, "Invalid xxx hierarchy, Env is neither at begin nor end !\n");
					exit(-1);
				}
				free(name);
			}
		} else
			xmlTransformationPush(node, replace_env_nodes, param);
		node = next;
	}
}

void fix_special_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (node->type == XML_ELEMENT_NODE) {
			if (is_valid_node(node, "node")) {
				xmlChar *type, * value;
				if (is_node_special(node, &type, &value)) {
					xmlUnsetProp(node, BAD_CAST "type");
					xmlNewProp(node, BAD_CAST "type", type);
					xmlNewProp(node, BAD_CAST "value", value);
					free(type);
					free(value);
				}
			} else {
				xmlTransformationPush(node, fix_special_nodes, param);
			}
		}
		node = next;
	}
}

int merge_adjacent_text(xmlNodePtr root) {
	int count = 0;
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (node->type == XML_ELEMENT_NODE) {
			count += merge_adjacent_text(node);
			
			if (next && is_valid_node(node, "text") && is_valid_node(next, "text")) {
				if(!next->children) {
					xmlUnlinkNode(next);
					xmlFreeNode(next);
					next = node;
				} else {
					int nodeF, nextF;
					int nodeH, nodeV, nextH, nextV;
					nodeH = atoi((const char *)xmlGetProp(node, BAD_CAST "h"));
					nodeV = atoi((const char *)xmlGetProp(node, BAD_CAST "v"));

					nextH = atoi((const char *)xmlGetProp(next, BAD_CAST "h"));
					nextV = atoi((const char *)xmlGetProp(next, BAD_CAST "v"));

					nodeF = atoi((const char *)xmlGetProp(node, BAD_CAST "font"));
					nextF = atoi((const char *)xmlGetProp(next, BAD_CAST "font"));
				
					if ( (nodeF == nextF) && (nodeV == nextV || nodeH == nextH ||
						/* the next one is more experimental. 786432 is the "normal" spacing between lines (this should be detected ?) */
						( (nextV - nodeV) <= 786432 && (nextH < nodeH) ) ) ) {
						xmlNodePtr text;
						/* if (nextV - nodeV) { fprintf(stderr, "merging %d and %d (%d)\n", nodeV, nextV, (nextV - nodeV)); } */
						xmlUnlinkNode(node);
						if(!do_not_add_space && xmlNodeIsText(next->children) && next->children->content[0] != ' ')
							xmlAddPrevSibling(next->children, xmlNewText(BAD_CAST " "));
						while(node->last) {
							text = node->last;
							xmlUnlinkNode(text);
							xmlAddPrevSibling(next->children, text);
						}
						xmlFreeNode(node);
						count += 1;
					}
				}
			}
		}
		node = next;
	}
	return count;
}

void merge_adjacent_text_nodes(xmlNodePtr root, xmlTransformationEntry * param) {
	int count = merge_adjacent_text(root);
	if (count) {
		fprintf(stderr, "merged %d text nodes\n", count);
	}
}

static
int is_accent_node_name(xmlChar * name) {
	return name && (
		strcmp((const char *)name, "textaccent") == 0 ||
		strcmp((const char *)name, "mathaccent") == 0
	);
}

static
int is_mathmode_node_name(xmlChar * name) {
	return name && (
		strcmp((const char *)name, "imath") == 0 ||
		strcmp((const char *)name, "equation") == 0
	);
}

void mathmode_drop_text_real(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if(node->type == XML_ELEMENT_NODE) {
			if (is_valid_node(node, "text")) {
				while (node->children) {
					xmlNodePtr child = node->children;
					xmlUnlinkNode(child);
					xmlAddPrevSibling(node, child);
				}
				xmlUnlinkNode(node);
				xmlFreeNode(node);
			} else {
				xmlTransformationPush(node, mathmode_drop_text_real, param);
			}
		}
		node = next;
	}
}

void mathmode_drop_text(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		if(node->type == XML_ELEMENT_NODE) {
			int search_children = 1;
			if (is_valid_node(node, "node")) {
				xmlChar * type = xmlGetProp(node, BAD_CAST "type");
				
				if (type && is_mathmode_node_name(type)) {
					xmlTransformationPush(node, mathmode_drop_text_real, param);
					search_children = 0;
				}
			}
			
			if (search_children) {
				xmlTransformationPush(node, mathmode_drop_text, param);
			}
		}
		
		node = node->next;
	}
}

/*
static void transform_math_expression_real(xmlNodePtr root) {
	xmlNsPtr ns;
	xmlNodePtr result, mrow, node;
	
	result = xmlNewNode(NULL, "math");
	ns = xmlNewNs(result, "http://www.w3.org/1998/Math/MathML", NULL);
	mrow = xmlNewChild(ns, result, "mrow", NULL);
	node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		
		if (xmlNodeIsText(node) {
			xmlChar * content;
			
			content = node->content;
			while(content && *content) {
				int family, len;
				xmlChar * element;
				len = utf8_get_next_family_char(content, &family, &element);
				if (len == 0) break;
				content += len;
				switch(family) {
					case UTF8_CHAR_FAMILY_NUMBER: xmlNewTextChild(mrow, ns, "mn", element); break;
					case UTF8_CHAR_FAMILY_OPERATOR: xmlNewTextChild(mrow, ns, "mo", element); break;
					case UTF8_CHAR_FAMILY_OTHER: xmlNewTextChild(mrow, ns, "mi", element); break;
					default: xmlNewTextChild(mrow, ns, "error", element); break;
				}
				xmlMemFree(element);
			}
			
		} else {
			xmlUnlinkNode(node);
			xmlAddChild(mrow, node);
		}
		
		node = next;
	}
	xmlAddPrevSibling(root, result);
	xmlUnlinkNode(root);
	xmlFreeNode(root);
}

void transform_math_expression(xmlNodePtr root) {
	xmlNodePtr node = root->children;
	while(node) {
		if(node->type == XML_ELEMENT_NODE) {
			int search_children = 1;
			if (strcmp((const char *)node->name, "node") == 0) {
				xmlChar * type = (const char *)xmlGetProp(node, BAD_CAST "type");
				
				if (type && is_mathmode_node_name(type)) {
					transform_math_expression_real(node);
					search_children = 0;
				}
			}
			
			if (search_children) {
				transform_math_expression(node);
			}
		}
		
		node = node->next;
	}
}
*/

void merge_accent_tags1(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (node->type == XML_ELEMENT_NODE) {
			
			if(next && is_valid_node(node, "text") && is_valid_node(next, "node")) {
				//  Transform : 
				//		<text>{...}</text><node type="textaccent"/><text>{...}</text>
				//	into :
				//		<text>{...}<node type="textaccent" />{...}</text>
				xmlChar * type = xmlGetProp(next, BAD_CAST "type");
				xmlNodePtr last = next->next;
				if (last && is_accent_node_name(type) && is_valid_node(last, "text")) {
					xmlNodePtr child;
					xmlUnlinkNode(next);
					xmlAddNextSibling(node->last, next);
					while ((child = last->children)) {
						xmlUnlinkNode(child);
						xmlAddNextSibling(node->last, child);
					}
					xmlUnlinkNode(last);
					xmlFreeNode(last);
					next = node;
				}
			}
			xmlTransformationPush(node, merge_accent_tags1, param);
		}
		node = next;
	}
}

void merge_accent_tags2(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (node->type == XML_ELEMENT_NODE) {
			if(next && is_valid_node(node, "node") && is_valid_node(next, "text")) {
				
				xmlChar * type = xmlGetProp(node, BAD_CAST "type");
				if (is_accent_node_name(type)) {
					//	Transform :
					//		<node type="textaccent"/><text>{...}</text>
					//	into :
					//		<text><node type="textaccent" />{...}</text>
					xmlUnlinkNode(node);
					if (next->children) {
						xmlAddPrevSibling(next->children, node);
					} else {
						xmlAddChild(next, node);
					}
					next = next->next;
				}
			}
			xmlTransformationPush(node, merge_accent_tags2, param);
		}
		node = next;
	}
}

static
int get_char_from_next_text(xmlNodePtr node, xmlChar * text) {
	if (node->next && xmlNodeIsText(node->next)) {
		return utf8_get_next_char(node->next, text);
	}
/* we have :
[]
	[t:NODE, type=text]
		...
		[t:NODE, type=accent]
	[t:NODE, type=text]
		[t:TEXT] (*)

	we want to take one UTF8 char from (*).
*/
	if (node->parent && 
			node->parent->next && 
			is_valid_node(node->parent->next, "text") &&
			node->parent->next->children &&
			xmlNodeIsText(node->parent->next->children)) {
		return utf8_get_next_char(node->parent->next->children, text);
	}
	return -1;
}

void transform_accent_tags(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (node->type == XML_ELEMENT_NODE) {
			
			if(is_valid_node(node, "node") && 
					is_accent_node_name(xmlGetProp(node, BAD_CAST "type"))
					) {
				xmlChar base[10];
				xmlChar accent[10];
				xmlChar conv[10];
				int err;
				
				err = get_char_from_next_text(node, accent);
				if(err) {
//					fprintf(stderr, "Invalid accent ! unable to find text for accent\n");
					xmlNewTextChild(node, NULL, BAD_CAST "letter", BAD_CAST "*error*");
					xmlNewTextChild(node, NULL, BAD_CAST "accent", BAD_CAST "*error*");
					node = next; continue ;
				}
				err = get_char_from_next_text(node, base);
				if(err) {
//					fprintf(stderr, "Invalid accent ! unable to find text for base\n");
					xmlNewTextChild(node, NULL, BAD_CAST "letter", BAD_CAST "*error*");
					xmlNewTextChild(node, NULL, BAD_CAST "accent", accent);
					node = next; continue ;
				}
				err = lfm_get_accent((const char *)base,(const char *) accent,(char *) conv);
				if (err) {
//					fprintf(stderr, "Unknown accent found ! letter = \"%s\", accent = \"%s\"\n", base, accent);
					xmlNewTextChild(node, NULL, BAD_CAST "letter", base);
					xmlNewTextChild(node, NULL, BAD_CAST "accent", accent);
				} else {
					if (node->next) {
						xmlAddPrevSibling(node->next, xmlNewText(conv));
						next = node->next;
					} else {
						xmlAddChild(node->parent, xmlNewText(conv));
						next = node->prev;
					}
					xmlUnlinkNode(node);
					xmlFreeNode(node);
				}
			} else {
				xmlTransformationPush(node, transform_accent_tags, param);
			}
		}
		node = next;
	}
}

static xmlDocPtr mathdoc;
static
xmlNodePtr get_math_node(xmlDocPtr doc, int begin_id, int end_id) {
	return mathlog_copy_expr(mathdoc, doc, begin_id, end_id);
}

void replace_tabular_math_entities(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlChar * type;
		xmlNodePtr child, next = node->next;
		if (is_valid_node(node, "node") &&
			(type = xmlGetProp(node, BAD_CAST "type")) &&
			(
				(
					strcmp((const char *)type, "tabular") == 0 &&
					(child = node->children)
				) || (
					strcmp((const char *)type, "minipage") == 0 &&
					(child = node->children->next)
				)
			) &&
			is_valid_node(child, "node") &&
			(type = xmlGetProp(child, BAD_CAST "type")) &&
			strcmp((const char *)type, "math") == 0 &&
			!child->next
		) {
			xmlNodePtr content;
			while((content = child->children)) {
				xmlUnlinkNode(content);
				xmlAddPrevSibling(child, content);
			}
			xmlUnlinkNode(child);
			xmlFreeNode(child);
		}
		xmlTransformationPush(node, replace_tabular_math_entities, param);
		node = next;
	}
}

static
void relink_label(xmlNodePtr src, xmlNodePtr dst) {
	xmlNodePtr node = src->children;
	while (node) {
		xmlChar * type;
		if (is_valid_node(node, "node") &&
			(type = xmlGetProp(node, BAD_CAST "type")) &&
			strcmp((const char *)type, "label") == 0
		) {
			xmlUnlinkNode(node);
			xmlAddPrevSibling(dst, node);
			return ;
		}
		node = node->next;
	}
}

void replace_math_entities(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	while(node) {
		xmlChar * type;
		xmlNodePtr next = node->next;
		if (is_valid_node(node, "node") &&
			(type = xmlGetProp(node, BAD_CAST "type")) &&
			(
				strcmp((const char *)type, "display") == 0 ||
				strcmp((const char *)type, "math") == 0
			)
		) {
			int begin_id, end_id;
			xmlChar * param;
			xmlNodePtr math;
			
			param = xmlGetProp(node, BAD_CAST "id");
			begin_id = param ? atoi((const char *)param) : -1;
			param = xmlGetProp(node, BAD_CAST "close-id");
			end_id = param ? atoi((const char *)param) : -1;
			
			math = get_math_node(root->doc, begin_id, end_id);
			if (math) {
				xmlAddPrevSibling(node, math);
				relink_label(node, math);
				xmlUnlinkNode(node);
				xmlFreeNode(node);
				next = math->next;
			} else {
				fprintf(stderr, "Unable to find replacement for formula : %d-%d\n", begin_id, end_id);
			}
		} else {
			xmlTransformationPush(node, replace_math_entities, param);
		}
		node = next;
	}
}

extern void start_entities();
extern void stop_entities();
extern char * replace_entities(const char *);

void replace_entities_in_text(xmlNodePtr root, xmlTransformationEntry * param) {
	/* find "[entity!(.*)!]" in text and replace by "&#(.*);" */
	xmlNodePtr node = root->children;
	while(node) {
		xmlNodePtr next = node->next;
		if (node->type == XML_ELEMENT_NODE) {
			xmlTransformationPush(node, replace_entities_in_text, param);
		} else if (xmlNodeIsText(node) && node->content) {
			char * content = replace_entities((const char *) node->content);
			if (content) {
				xmlNodePtr p = xmlNewChild(node->parent, NULL, BAD_CAST "temp", BAD_CAST content);
				xmlReplaceNode(node, p->children);
				free(content);
				xmlUnlinkNode(p);
				xmlFreeNode(p);
				xmlFreeNode(node);
			}
		}
		node = next;
	}
}

void xmlRegisterTextTransformations() {
#define DEF(x) xmlTransformationRegister("text", #x, x, 0);
	DEF(remove_page_nodes)
	DEF(remove_bop_nodes)
	DEF(transform_large_accent_pattern)
	DEF(remove_control_nodes)
	DEF(replace_xxx_nodes)
	DEF(replace_env_nodes)
	DEF(fix_special_nodes)
	DEF(mathmode_drop_text)
	DEF(merge_accent_tags1)
	DEF(merge_accent_tags2)
	DEF(transform_accent_tags)
	DEF(merge_adjacent_text_nodes)
	DEF(replace_entities_in_text)
	DEF(replace_tabular_math_entities)
	DEF(replace_math_entities)
#undef DEF
#define DEF(x) xmlTransformationRegister("math", #x, x, 0);
	DEF(replace_entities_in_text)
#undef DEF
}

extern void xmlRegisterPostXmlTransformations();

void xmlRegisterTransformations() {
	xmlRegisterTextTransformations();
	xmlRegisterMathTransformations();
	xmlRegisterPostXmlTransformations();
}

static
const char * get_log_filename(const char * filename) {
static char buffer[1024];
	const char * fname = strrchr(filename, '/') ? strrchr(filename, '/') : filename;
	const char * extname = strrchr(fname, '.');
	int len = extname ? extname - filename : strlen(filename);
	memcpy(buffer, filename, len);
	strcpy(buffer + len, ".log");
	return buffer;
}

static
void init_transformations() {
	xmlRegisterTransformations();
	xmlTransformationInit("transformations.xml");
}

int main(int argc, char * argv[]) {
	int err;
	int flags;
	const char * dvifile;

	dvifile_t * dvi;
    xmlDocPtr doc;
    xmlNodePtr root;

#if USE_KPSE
	kpse_set_program_name(argv[0], "lxir");
#endif

    LIBXML_TEST_VERSION;
	xmlSubstituteEntitiesDefault(1);
	xmlLoadExtDtdDefaultValue = 1;
	
	lfm_init();
	
	if (argc < 2) {
		fprintf(stderr, "Usage : %s [-skip {1|sp|all}] <dvi>\n", argv[0]);
		return -1;
	}
	if(argc >= 4) {
		if (strcmp(argv[1], "-skip")) {
			fprintf(stderr, "invalid parameter : %s\n", argv[1]);
			return -1;
		}
		if (strcmp(argv[2], "1") == 0) {
			flags = DVI_SKIP_SPACE_SMALL;
			do_not_add_space = 1;
		} else if(strcmp(argv[2], "sp") == 0) {
			flags = DVI_SKIP_SPACE_NORMAL;
		} else if(strcmp(argv[2], "all") == 0) {
			flags = DVI_SKIP_SPACE_SMALL | DVI_SKIP_SPACE_NORMAL;
		} else {
			fprintf(stderr, "unknown value for skip parameter : %s\n", argv[2]);
			return -1;
		}
		dvifile = argv[3];
	} else {
		flags = DVI_SKIP_SPACE_SMALL;
		dvifile = argv[1];
	}
		
	err = dvi_read(&dvi, dvifile, flags);
	if(err) {
		fprintf(stderr, "Error (%d) reading DVI file.\n", err);
		return -1;
	}
	init_transformations();
	start_entities();
	
	mathdoc = mathlog_read_file(get_log_filename(dvifile));
	xmlSaveFormatFileEnc("temp-mathlog.xml", mathdoc, "UTF-8", 1);
	
	doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "document");
	xmlDocSetRootElement(doc, root);
	

	make_header(root, dvi);
	make_content(root, dvi);
	
	dvi_destroy(dvi);
	
	xmlSaveFormatFileEnc("temp-0-rawdvi.xml", doc, "UTF-8", 1);
	
	xmlTransformationApplyList("text", &doc);
	
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
    xmlMemoryDump();
	lfm_close();
#ifndef _WIN32
	stop_entities();
#endif
	return 0;
}

