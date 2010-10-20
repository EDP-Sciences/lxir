#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml/tree.h>
#include <string.h>

#include "transformations.h"

extern void clean_rowspan(xmlNodePtr, xmlTransformationEntry *);

struct cell_t {
	xmlNodePtr node;
	int row_span;
	int col_span;
	int has_content;
};

struct row_t {
	xmlNodePtr node;
	struct cell_t ** cells;
	int num_cells;
};

struct col_t {
	xmlNodePtr node;
	xmlChar * align;
};

struct table_t {
	xmlNodePtr node;
	xmlNodePtr colgroup;

	struct col_t ** cols;
	int num_cols;

	struct row_t ** rows;
	int num_rows;
};


static
void free_cell(struct cell_t * cell) {
	free(cell);
}

static
void free_row(struct row_t * row) {
	int i;
	for (i = 0; i < row->num_cells; ++i)
		free_cell(row->cells[i]);
	free(row->cells);
	free(row);
}

static
void free_col(struct col_t * col) {
	xmlFree(col->align);
	free(col);
}

static
void free_table(struct table_t * table) {
	int i;
	for (i = 0; i < table->num_rows; ++i)
		free_row(table->rows[i]);
	free(table->rows);
	for (i = 0; i < table->num_cols; ++i)
		free_col(table->cols[i]);
	free(table->cols);
	free(table);
}

static
int is_cell_node(xmlNodePtr node) {
	return
		node &&
		node->type == XML_ELEMENT_NODE && (
			strcasecmp((const char *)node->name, "td") == 0 ||
			strcasecmp((const char *)node->name, "th") == 0
		);
}

static
int is_row_node(xmlNodePtr node) {
	return
		node &&
		node->type == XML_ELEMENT_NODE &&
		strcasecmp((const char *)node->name, "tr") == 0;
}

static
int is_col_node(xmlNodePtr node) {
	return
		node &&
		node->type == XML_ELEMENT_NODE &&
		strcasecmp((const char *)node->name, "col") == 0;
}

static
int is_colgroup_node(xmlNodePtr node) {
	return
		node &&
		node->type == XML_ELEMENT_NODE &&
		strcasecmp((const char *)node->name, "colgroup") == 0;
}

static
int is_table_node(xmlNodePtr node) {
	return
		node &&
		node->type == XML_ELEMENT_NODE &&
		strcasecmp((const char *)node->name, "table") == 0;
}

static
struct cell_t * build_cell(xmlNodePtr node) {
	struct cell_t * result;
	xmlChar * attr;
	if (!is_cell_node(node)) return 0;
	result = (struct cell_t *) malloc(sizeof(struct cell_t));
	result->node = node;
	result->row_span = 1;
	result->col_span = 1;
	result->has_content = 1;
	// extract rowspan / colspan attributes
	attr = xmlGetProp(node, "rowspan");
	if (attr) {
		result->row_span = atoi((const char *) attr);
		xmlFree(attr);
	}
	attr = xmlGetProp(node, "colspan");
	if (attr) {
		result->col_span = atoi((const char *) attr);
		xmlFree(attr);
	}
	// has_content is 0 only if one TEXT child which is only space
	if (
		!node->children || (
			node->children == node->last &&
			node->children->type == XML_TEXT_NODE &&
			strlen(node->content) == strspn(node->content, " \t\r\n")
		)
	) {
			result->has_content = 0;
	}
	return result;
}

static
struct row_t * build_row(xmlNodePtr node) {
	struct row_t * result;
	xmlNodePtr n;
	if (!is_row_node(node)) return 0;
	result = (struct row_t *) malloc(sizeof(struct row_t));
	result->num_cells = 0;
	result->cells = 0;
	result->node = node;

	for (n = node->children; (n != 0); n = n->next) {
		struct cell_t * cell = build_cell(n);
		if (cell) {
			result->num_cells++;
			result->cells = realloc(result->cells, result->num_cells * sizeof(struct cell_t *));
			result->cells[result->num_cells - 1] = cell;
		}
	}
	return result;
}

static
struct col_t * build_col(xmlNodePtr node) {
	struct col_t * result;
	if (!is_col_node(node)) return 0;
	result = (struct col_t *) malloc(sizeof(struct col_t));
	result->node = node;
	result->align = xmlGetProp(node, "align");

	return result;
}

static
xmlNodePtr * build_cols(xmlNodePtr node, struct table_t * result) {
	xmlNodePtr n;
	if (!is_colgroup_node(node)) return 0;
	result->colgroup = node;
	for (n = node->children; (n != 0); n = n->next) {
		struct col_t * col = build_col(n);
		if (col) {
			result->num_cols++;
			result->cols = realloc(result->cols, result->num_cols * sizeof(struct col_t *));
			result->cols[result->num_cols - 1] = col;
		}
	}
}

static
struct table_t * build_table(xmlNodePtr node) {
	struct table_t * result;
	xmlNodePtr n;
	if (!is_table_node(node)) return 0;
	result = (struct table_t *) malloc(sizeof(struct table_t));
	result->num_rows = 0;
	result->rows = 0;
	result->cols = 0;
	result->num_cols = 0;
	result->node = node;


	for (n = node->children; (n != 0); n = n->next) {
		struct row_t * row = build_row(n);
		if (row) {
			result->num_rows++;
			result->rows = realloc(result->rows, result->num_rows * sizeof(struct row_t *));
			result->rows[result->num_rows - 1] = row;
		} else if (!result->cols) {
			build_cols(n, result);
		}
	}
	return result;
}

static
struct cell_t * get_cell_by_index(struct row_t * row, int index) {
	int c, cell_index = 0;
	for (c = 0; c < row->num_cells; ++c) {
		struct cell_t * cell = row->cells[c];
		if (cell_index == index) return cell;
		cell_index += cell->col_span;
	}
	return 0;
}

static
void fix_table_row_span(struct table_t * table) {
	int r, c;
	for (r = 0; r < table->num_rows; ++r) {
		struct row_t * row = table->rows[r];
		int cell_index = 0;
		for (c = 0; c < row->num_cells; ++c) {
			struct cell_t * cell = row->cells[c];
			if (cell->row_span > 1) {
				int i;
				for (i = 1; i < cell->row_span; ++i) {
					if ((r + i) < table->num_rows) {
						struct cell_t * c = get_cell_by_index(table->rows[r + i], cell_index);
						if (c->has_content) {
							fprintf(stderr, "ERROR : clean_rowspan is trying to remove content\n");
							return;
						}
						if (c->row_span != 1 || c->col_span != 1) {
							fprintf(stderr, "ERROR : clean_rowspan is trying to remove an invalid cell\n");
							return;
						}
						if (!c->node) {
							fprintf(stderr, "ERROR : clean_rowspan is trying to remove a cell twice\n");
						}
					} else {
						fprintf(stderr, "ERROR : clean_rowspan is trying to remove too many cells : %d + %d >= %d\n",
							r, i, table->num_rows);
						cell->row_span = table->num_rows - r;
						break;
					}
				}
				for (i = 1; i < cell->row_span; ++i) {
					struct cell_t * c = get_cell_by_index(table->rows[r + i], cell_index);
					if (c->node) {
						xmlUnlinkNode(c->node);
						xmlFreeNode(c->node);
						c->node = 0;
					}
				}
			}
			cell_index += cell->col_span;
		}
	}
}

static
void clean_all_cells(struct table_t * table, xmlTransformationEntry * param) {
	int r, c;
	for (r = 0; r < table->num_rows; ++r) {
		struct row_t * row = table->rows[r];
		for (c = 0; c < row->num_cells; ++c) {
			struct cell_t * cell = row->cells[c];
			if (cell->node) {
				xmlTransformationPush(cell->node, clean_rowspan, param);
			}
		}
	}
}

static
void set_column_alignment(struct table_t * table) {
	int r, c;
	for (r = 0; r < table->num_rows; ++r) {
		struct row_t * row = table->rows[r];
		for (c = 0; c < table->num_cols; ++c) {
			struct cell_t * cell = get_cell_by_index(row, c);
			if (cell && cell->node) {
				xmlChar * align = table->cols[c]->align;
				xmlChar * attr = xmlGetProp(cell->node, "align");
				if (!attr && align) {
					xmlSetProp(cell->node, "align", align);
				}
				if (attr) {
					xmlFree(attr);
				}
			}
		}
	}
	if (table->colgroup) {
		xmlUnlinkNode(table->colgroup);
		xmlFreeNode(table->colgroup);
	}
}

extern
void clean_rowspan(xmlNodePtr root, xmlTransformationEntry * param) {
	xmlNodePtr node = root->children;

	while (node) {
		xmlNodePtr next = node->next;
		struct table_t * table = build_table(node);
		if (table) {
			fix_table_row_span(table);
			set_column_alignment(table);
			clean_all_cells(table, param);
			free_table(table);
		} else {
			xmlTransformationPush(node, clean_rowspan, param);
		}
		node = next;
	}
}
