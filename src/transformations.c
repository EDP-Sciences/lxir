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

#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxslt/transform.h>
#include <string.h>

#include <kpathsea/kpathsea.h>

#include "transformations.h"

static char * _xstrdup(const char * p) {
	return p ? strdup(p) : 0;
}

struct xmlTransformationListEntry;

typedef struct xmlTransformationList {
	struct xmlTransformationList * next;

	const char * name;
	struct xmlTransformationListEntry * first;
	struct xmlTransformationListEntry * last;
} xmlTransformationList;

typedef struct xmlTransformationListEntry {
	struct xmlTransformationListEntry * next;

	xmlTransformation transformation;
	const char * parameter;
} xmlTransformationListEntry;

#define ROOT_NODE	((xmlNodePtr) -1)
#if USE_DOUBLE_LINKED_LIST

typedef struct xmlTransformationStackEntry {
	struct xmlTransformationStackEntry * next;

	xmlNodePtr node;
	xmlTransformation transformation;
	const char * parameter;
} xmlTransformationStackEntry;

typedef struct xmlTransformationStack {
	xmlDocPtr document;
	xmlTransformationStackEntry * first;
	xmlTransformationStackEntry * last;
} xmlTransformationStack;

#else

typedef struct xmlTransformationStackEntry {
	xmlNodePtr node;
	xmlTransformation transformation;
	const char * parameter;
} xmlTransformationStackEntry;

typedef struct xmlTransformationStack {
	xmlDocPtr document;
	unsigned int count;
	unsigned int pos;
	xmlTransformationStackEntry * entries;
} xmlTransformationStack;

#endif

typedef struct xmlTransformationInfoEntry {
	struct xmlTransformationInfoEntry * next;

	const char * type;
	const char * name;
	xmlTransformation transformation;
	const char * parameter;
} xmlTransformationInfoEntry;

static int xmlTransformationReport = 0;
static xmlTransformationList * lists = 0;
static xmlTransformationInfoEntry * nametop = 0;

void xmlTransformationSetReport(int value) {
	xmlTransformationReport = value;
}


int xmlTransformationAddToList(const char * type, const char * name, const char * parameter) {
	xmlTransformationList * list = lists;
	xmlTransformationListEntry * entry;
	xmlTransformationInfoEntry * info = nametop;
	while(info) {
		if (strcmp(info->type, type) == 0 && strcmp(info->name, name) == 0)
			break;
		info = info->next;
	}
	if(!info) {
		fprintf(stderr, "Unable to find transformation \"%s:%s\"\n", type, name);
		return 0;
	}

	while(list) {
		if (strcmp(type, list->name) == 0) break;
		list = list->next;
	}
	if(!list) {
		list = malloc(sizeof(xmlTransformationList));
		list->name = _xstrdup(type);
		list->first = 0;
		list->last = 0;
		list->next = lists;
		lists = list;
	}
	entry = malloc(sizeof(xmlTransformationListEntry));
	entry->transformation = info->transformation;
	entry->parameter = parameter ? strdup(parameter) : info->parameter;
	entry->next = 0;
	if (list->last) {
		list->last->next = entry;
	} else {
		list->first = entry;
	}
	list->last = entry;

	return 1;
}

#if USE_DOUBLE_LINKED_LIST
static
void xmlTransformationPushLast(xmlNodePtr node, xmlTransformation trans, xmlTransformationEntry * p) {
	xmlTransformationStackEntry * entry = malloc(sizeof(xmlTransformationStackEntry));
	entry->next = 0;
	entry->node = node;
	entry->transformation = trans;
	entry->parameter = p->parameter;
	if(p->stack->last) {
		p->stack->last->next = entry;
	} else {
		p->stack->first = entry;
	}
	p->stack->last = entry;
}

void xmlTransformationPush(xmlNodePtr node, xmlTransformation trans, xmlTransformationEntry * p) {
	xmlTransformationStackEntry * entry = malloc(sizeof(xmlTransformationStackEntry));
	entry->next = p->stack->first;
	entry->node = node;
	entry->transformation = trans;
	entry->parameter = _xstrdup(p->parameter);
	p->stack->first = entry;
	if(!p->stack->last) {
		p->stack->last = entry;
	}
}

static
int xmlTransformationApply(xmlTransformationStack * stack) {
	xmlTransformationEntry trans;
	xmlTransformationStackEntry entry;
	entry = stack->first;
	if(!entry) return 0;
	stack->first = entry->next;
	trans.stack = stack;
	trans.parameter = entry.parameter;
	(*entry.transformation)(entry.node != ROOT_NODE ? entry.node : stack->document->children, &trans);
	free((void *)entry.parameter);
	free(entry);

	return 1;
}

static
void xmlTransformationStackClear() {
}

static
void xmlTransformationStackInit(xmlTransformationStack * s) {
	s->first = 0;
	s->last = 0;
}

#else

#define STACK_SIZE 32

static
void xmlTransformationStackClear(xmlTransformationStack * s) {
	s->count = 0;
	s->pos = 0;
	free(s->entries);
}

static
void xmlTransformationStackInit(xmlTransformationStack * s) {
	s->count = STACK_SIZE;
	s->pos = 0;
	s->entries =  (xmlTransformationStackEntry *) malloc(s->count * sizeof(xmlTransformationStackEntry));
}

static
void xmlTransformationStackGrow(xmlTransformationStack * s) {
	if (s->pos == s->count) {
		s->count *= 2;
		s->entries =  (xmlTransformationStackEntry *) realloc(s->entries, s->count * sizeof(xmlTransformationStackEntry));
	}
}

static
void xmlTransformationPushLast(xmlNodePtr node, xmlTransformation trans, xmlTransformationEntry * p) {
	int i;
	xmlTransformationStackGrow(p->stack);
	for (i = p->stack->pos - 1; i >= 0; --i) {
		p->stack->entries[i + 1] = p->stack->entries[i];
	}
	p->stack->entries[0].node = node;
	p->stack->entries[0].transformation = trans;
	p->stack->entries[0].parameter = _xstrdup(p->parameter);
	p->stack->pos++;
}

void xmlTransformationPush(xmlNodePtr node, xmlTransformation trans, xmlTransformationEntry * p) {
	xmlTransformationStackGrow(p->stack);
	p->stack->entries[p->stack->pos].node = node;
	p->stack->entries[p->stack->pos].transformation = trans;
	p->stack->entries[p->stack->pos].parameter = _xstrdup(p->parameter);
	p->stack->pos++;
}

static
int xmlTransformationApply(xmlTransformationStack * stack) {
	xmlTransformationEntry trans;
	xmlTransformationStackEntry entry;
	if(stack->pos == 0) return 0;
	entry = stack->entries[--stack->pos];

	trans.stack = stack;
	trans.parameter = entry.parameter;

	(*entry.transformation)(entry.node != ROOT_NODE ? entry.node : stack->document->children, &trans);
	free((void *)entry.parameter);

	return 1;
}

#endif

int xmlTransformationApplyList(const char * type, xmlDocPtr * pdoc) {
	xmlTransformationStack stack;
	xmlTransformationEntry trans;
	xmlTransformationList * list = lists;
	xmlTransformationListEntry * entry;

	xmlTransformationStackInit(&stack);
	stack.document = *pdoc;

	while(list) {
		if (strcmp(type, list->name) == 0) break;
		list = list->next;
	}
	if(!list) {
		fprintf(stderr, "Unable to find transformation list \"%s\"\n", type);
		return 0;
	}
	trans.stack = &stack;
	entry = list->first;
	while (entry) {
		trans.parameter = _xstrdup(entry->parameter);
		xmlTransformationPushLast(ROOT_NODE, entry->transformation, &trans);
		entry = entry->next;
	}
	while (xmlTransformationApply(&stack)) ;

	xmlTransformationStackClear(&stack);

	*pdoc = stack.document;

	return 1;
}


void xmlTransformationRegister(const char * type, const char * name, xmlTransformation trans, const char * parameter) {
	xmlTransformationInfoEntry * next = (xmlTransformationInfoEntry *) malloc(sizeof(xmlTransformationInfoEntry));
	next->name = _xstrdup(name);
	next->type = _xstrdup(type);
	next->transformation = trans;
	next->parameter = _xstrdup(parameter);
	next->next = nametop;
	nametop = next;
}

static
void dump_tree(xmlNodePtr root, xmlTransformationEntry * param) {
	if (param->parameter) {
		fprintf(stderr, "Dumping tree into file \"%s\"\n", param->parameter);
		xmlSaveFormatFileEnc(param->parameter, root->doc, "UTF-8", 1);
	}
}

static
void xslt_proc(xmlNodePtr root, xmlTransformationEntry * param) {
	assert(root->doc->children == root && root->doc == param->stack->document);
	if (param->parameter) {
		xmlDocPtr old, result;
		xsltStylesheetPtr ss;
		char * path;
		old = param->stack->document;

		path = kpse_find_file(param->parameter, kpse_tex_format, true);
		if (path) {
			ss = xsltParseStylesheetFile((const xmlChar *)path);
		} else {
			path = malloc(strlen(TEXMF_XSLT_DIR) + strlen(param->parameter) + 2);
			sprintf(path, TEXMF_XSLT_DIR "/%s", param->parameter);
			ss = xsltParseStylesheetFile((const xmlChar *)path);
			free(path);
		}
		result = xsltApplyStylesheet(ss, old, 0);
		xsltFreeStylesheet(ss);

		if (result) {
			xmlFreeDoc(old);
			param->stack->document = result;
		} else {
			fprintf(stderr, "Unable to apply stylesheet, result has been discarded\n");
		}
	}
}

static
void xmlRegisterBaseTransformations() {
	xmlTransformationRegister("text", "dump_tree", dump_tree, 0);
	xmlTransformationRegister("math", "dump_tree", dump_tree, 0);
	xmlTransformationRegister("text", "xslt_proc", xslt_proc, 0);
	xmlTransformationRegister("math", "xslt_proc", xslt_proc, 0);
}

static
xmlNodePtr find_first_element_node(xmlDocPtr doc) {
	xmlNodePtr node = doc->children;
	while (node && node->type != XML_ELEMENT_NODE)
		node = node->next;
	if (node) return node->children;
	else return 0;
}

static int should_report(xmlChar * report_all, xmlChar * report) {
	if (xmlTransformationReport) return 1;
	if (report_all && strcmp(report_all, "yes") == 0) return 1;
	if (report && strcmp(report, "yes") == 0) return 1;
	return 0;
}

void xmlTransformationInit(const char * filename) {
	xmlDocPtr trans;
	xmlNodePtr node;
	int i = 1;


	const char * path = kpse_find_file(filename, kpse_tex_format, true);
	if (path) {
		trans = xmlReadFile(path, NULL, 0);
	} else {
		char * path = malloc(strlen(TEXMF_XML_DIR) + strlen(filename) + 2);
		sprintf(path, TEXMF_XML_DIR "/%s", filename);
		trans = xmlReadFile(path, NULL, 0);
		free(path);
	}
	if (!trans) {
		fprintf(stderr, "Unable to find \"%s\", exiting...\n", filename);
		exit(-1);
	}
	node = find_first_element_node(trans);

	xmlRegisterBaseTransformations();

	while(node) {
		if (node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, "stack") == 0) {
			xmlChar * type = xmlGetProp(node, BAD_CAST "type");
			xmlChar * report_all = xmlGetProp(node, BAD_CAST "report-all");
			xmlNodePtr child = node->children;
			while (child) {
				if (child->type == XML_ELEMENT_NODE && strcmp((const char *) child->name, "transformation") == 0) {
					xmlChar * name = xmlGetProp(child, BAD_CAST "name");
					xmlChar * param = xmlGetProp(child, BAD_CAST "param");
					xmlTransformationAddToList((const char *)type, (const char *)name, (const char *)param);
					xmlChar * report = xmlGetProp(child, BAD_CAST "report");
					if (should_report(report_all, report)) {
						char filename[128];
						sprintf(filename, "temp-%d-%s.xml", i++, name);
						xmlTransformationAddToList((const char *)type, "dump_tree", filename);
					}
					xmlFree(name);
					xmlFree(param);
					xmlFree(report);
				}
				child = child->next;
			}
			xmlFree(type);
		}
		node = node->next;
	}
	xmlFreeDoc(trans);
}
