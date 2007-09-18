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
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "transformations.h"

static
inline int is_node_element(xmlNodePtr node, const char * name) {
	return node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, name) == 0;
}

static
inline int is_node_element_not(xmlNodePtr node, const char * name) {
	return node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, name) != 0;
}

/*
	Cette fonction effectue transforme une séquence d'éléments quelconques
	séparé par des noeuds spécifiques en séquence de noeuds spécifiques contenant
	les élémentes quelconques.
	
	<item />
	toto
	<item />
	titi
	
	devient :
	
	<item>toto</item>
	<item>titi</item>
*/
static
void close_siblings(xmlNodePtr root, const char * name) {
	xmlNodePtr node = root->children;
	
	while (node) {
		xmlNodePtr next = node->next;
		
		if (is_node_element(node, name)) {
			xmlNodePtr inner = node->next;
			while (inner && is_node_element_not(inner, name)) {
				xmlNodePtr n = inner->next;
				xmlUnlinkNode(inner);
				xmlAddChild(node, inner);
				inner = n;
			}
			next = inner;
		}
		
		node = next;
	}
}

static
void rebuild_all_lists(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_element(node, "list")) {
			close_siblings(node, "item");
		} else {
			xmlTransformationPush(node, rebuild_all_lists, param);
		}
		node = next;
	}
}

static
void rebuild_bibliography(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_element(node, "thebibliography")) {
			close_siblings(node, "bibitem");
		} else {
			xmlTransformationPush(node, rebuild_all_lists, param);
		}
		node = next;
	}
}

static
const char * stop_section_list_names[] = {
	"thebibliography", "footnotelist", 0
};

static
const char * stop_paragraph_list_names[] = {
	"section", "parsep", 0
};

static
int is_name_in_list(const char * name, const char ** list) {
	while(*list) {
		if(strcmp(*list++, name) == 0) return 1;
	}
	return 0;
}

static
int is_par_end_node(xmlNodePtr node) {
	const char * name = (const char *) node->name;
	return
		is_name_in_list(name, stop_paragraph_list_names)
	||
		is_name_in_list(name, stop_section_list_names);	
}

static
void rebuild_paragraphs(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	int found = 0;
	
	while (node) {
		xmlNodePtr next = node->next;
		
		if (node->type == XML_ELEMENT_NODE &&
				is_name_in_list(node->name, stop_paragraph_list_names)) {
			
			xmlNodePtr par = xmlNewNode(0, BAD_CAST "par");
			xmlNodePtr content = node->next;
			
			while (content && content->type == XML_ELEMENT_NODE &&
					!is_par_end_node(content)) {
				xmlNodePtr n = content->next;
				
				xmlUnlinkNode(content);
				xmlAddChild(par, content);
				
				content = n;
			}
			next = content;
			
			if (par->children) {
				xmlAddNextSibling(node, par);
			}
			if (strcmp(node->name, "parsep") == 0) {
				xmlUnlinkNode(node);
				xmlFreeNode(node);
			}
			
			found = 1;
		}
		
		node = next;
	}
	if (!found) {
		node = root->children;
		while (node) {
			xmlTransformationPush(node, rebuild_paragraphs, param);
			node = node->next;
		}
	}
}

static
int get_section_level(xmlNodePtr section) {
	xmlNodePtr header = section->children;
	while (header && !is_node_element(header, "sectionHeader"))
		header = header->next;
	if (!header) return -1;
	const char * level = (const char *)xmlGetProp(header, BAD_CAST "level");
	return level ? atoi(level) : -1;
}

static
int is_stop_section_node(xmlNodePtr node, int level) {
	if (node->type == XML_ELEMENT_NODE &&
		is_name_in_list(node->name, stop_section_list_names))
		return 1;
	if (is_node_element(node, "section") && get_section_level(node) <= level)
		return 1;
	return 0;
}

static
void rebuild_sections(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	
	while (node) {
		xmlNodePtr next = node->next;
		
		if (is_node_element(node, "section")) {
			int level = get_section_level(node);
			xmlNodePtr n = node->next;
			
			while(n && !is_stop_section_node(n, level)) {
				xmlNodePtr nn = n->next;
				
				xmlUnlinkNode(n);
				xmlAddChild(node, n);
				
				n = nn;
			}
			next = node->next;
		}
		
		xmlTransformationPush(node, rebuild_sections, param);
		node = next;
	}
}

static
int count_number_of_colpar(xmlNodePtr tab) {
	xmlNodePtr node = tab->children;
	int result = 0;
	while (node) {
		if (is_node_element(node, "col") || is_node_element(node, "par")) {
			result++;
		}
		node = node->next;
	}
	return result;
}

static
int count_number_of_vline(xmlNodePtr tab) {
	xmlNodePtr node = tab->children;
	int result = 0;
	while (node) {
		if (is_node_element(node, "vline") || 
			is_node_element(node, "intercolumnDef")
		) {
			return result;
		} else if(node->type == XML_ELEMENT_NODE) {
			result++;
		}
		node = node->next;
	}
	return result;
}

static
void count_number_of_columns(xmlNodePtr tab, int *pncols, int *pnvlines) {
	xmlNodePtr node = tab->children;
	int ncols = 0, nvlines = 0;
	while (node) {
		if (is_node_element(node, "columnsModel")) {
			ncols += count_number_of_colpar(node);
			nvlines += count_number_of_vline(node);
		}
		node = node->next;
	}
	*pncols = ncols;
	*pnvlines = nvlines;
}

static
xmlNodePtr find_eol(xmlNodePtr base) {
	xmlNodePtr node = base->children;
	while (node) {
		if (is_node_element(node, "eol")) {
			return node;
		} else {
			xmlNodePtr eol = find_eol(node);
			if (eol) return eol;
		}
		node = node->next;
	}
	return 0;
}

static
xmlNodePtr create_line_groups(xmlNodePtr tab) {
	xmlNodePtr result = xmlCopyNode(tab, 2);
	int ncols, nvlines;
	xmlNodePtr node = tab->children;
	xmlNodePtr rowGroup = 0, row = 0;
	int tageol = 0, cvlinee = 0, c = 0;

	count_number_of_columns(tab, &ncols, &nvlines);
	
	while(node) {
		xmlNodePtr next = node->next;
		if (is_node_element(node, "columnsModel")) {
			xmlUnlinkNode(node);
			xmlAddChild(result, node);
		} else if (is_node_element(node, "hline") || is_node_element(node, "cline")) {
			if (rowGroup) {
				xmlAddChild(result, rowGroup);
				rowGroup = 0;
			}
			xmlUnlinkNode(node);
			xmlAddChild(result, node);
		} else if (is_node_element(node, "cell")) {
			xmlNodePtr eol = find_eol(node);
			if (eol) {
				tageol = 1;
				xmlUnlinkNode(eol);
				xmlFreeNode(eol);
			}
			if (!row) row = xmlNewNode(0, BAD_CAST "row");
			xmlUnlinkNode(node);
			xmlAddChild(row, node);
		} else if (is_node_element(node, "vline") || is_node_element(node, "intercolumn")) {
			if (!row) row = xmlNewNode(0, BAD_CAST "row");
			xmlUnlinkNode(node);
			xmlAddChild(row, node);
			if (c == ncols || tageol) {
				cvlinee ++;
			}
		} else if (is_node_element(node, "multicolumn")) {
			if (!row) row = xmlNewNode(0, BAD_CAST "row");
			xmlUnlinkNode(node);
			xmlAddChild(row, node);
			c += atoi((const char *) xmlGetProp(node, "span"));
		} else if (is_node_element(node, "eol")) {
			tageol = 1;
		} else if (is_node_element_not(node,"text")) {
			fprintf(stderr, "Unknown node in tabular, %s (id: %s)\n", (const char *)node->name, (const char *)xmlGetProp(node, BAD_CAST "id"));
		}
		if ( (c == ncols || tageol) &&
			(cvlinee == nvlines || is_node_element_not(node, "multicolumn"))
		) {
			if (!rowGroup) rowGroup = xmlNewNode(0, BAD_CAST "rowGroup");
			if (row) {
				xmlAddChild(rowGroup, row);
				row = 0;
			}
			c = 0;
			cvlinee = 0;
			tageol = 0;
		}
		node = next;
	}
	if (rowGroup) {
		xmlAddChild(result, rowGroup);
	}
	xmlReplaceNode(tab, result);
	xmlFreeNode(tab);
	return result;
}

static
void create_col_groupchildren(xmlNodePtr model) {
	xmlNodePtr node = model->children;
	xmlNodePtr result = xmlCopyNode(model, 2);
	xmlNodePtr group = 0;
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_element(node, "vline")) {
			if (group) {
				xmlAddChild(result, group);
				group = 0;
			}
			xmlUnlinkNode(node);
			xmlAddChild(result, node);
		} else {
			if (!group) group = xmlNewNode(0, BAD_CAST "columnsGroup");
			
			xmlUnlinkNode(node);
			xmlAddChild(group, node);
		}
		
		node = next;
	}
	if (group) xmlAddChild(result, group);
	xmlReplaceNode(model, result);
	xmlFreeNode(model);
}

static
void create_col_groups(xmlNodePtr tab) {
	xmlNodePtr node = tab->children;
	
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_element(node, "columnsModel")) 
		{	/* normalement, il n'y en a qu'un */
			create_col_groupchildren(node);
		}
		node = next;
	}
}

static
void rebuild_tabulars(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;
	
	while (node) {
		xmlNodePtr next = node->next;
		if (is_node_element(node, "tabular") || is_node_element(node, "array")) {
			create_col_groups(create_line_groups(node));
		} else {
			xmlTransformationPush(node, rebuild_tabulars, param);
		}
		node = next;
	}
}

void xmlRegisterPostXmlTransformations() {
#define DEF(x) xmlTransformationRegister("text", #x, x, 0);
	DEF(rebuild_all_lists)
	DEF(rebuild_bibliography)
	DEF(rebuild_paragraphs)
	DEF(rebuild_sections)
	DEF(rebuild_tabulars)
#undef DEF
}
