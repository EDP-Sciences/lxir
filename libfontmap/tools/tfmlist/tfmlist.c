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
#include <regex.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

struct tfm_list {
	struct tfm_list * next;
	char * file;
	char * encoding;
};

struct tfm_list * tfms = 0;

struct fontenc_t {
		regex_t * encpattern;
		regex_t * fampattern;
		char * encoding;
};

static int fontenclen;
static struct fontenc_t * fontenctbl;

static
xmlNodePtr find_first_element_node(xmlDocPtr doc) {
	xmlNodePtr node = doc->children;
	while (node && node->type != XML_ELEMENT_NODE)
		node = node->next;
	if (node) return node->children;
	else return 0;
}

void init_fontenc(const char * pattern) {
    xmlDocPtr doc;
    xmlNodePtr node;
	int i;

	doc = xmlReadFile(pattern, NULL, 0);

	node = find_first_element_node(doc); i = 0;

	while(node) {
		if (node->type == XML_ELEMENT_NODE) ++i;
		node = node->next;
	}
	fontenclen = i;
	fontenctbl = malloc(fontenclen * sizeof(struct fontenc_t));
	node = find_first_element_node(doc); i = 0;

	while(node) {
		if (node->type == XML_ELEMENT_NODE) {
			xmlChar * attr;
			struct fontenc_t * enc = fontenctbl + i;

			attr = xmlGetProp(node, BAD_CAST "encoding");
			if (attr) {
				int ret;
				enc->encpattern = (regex_t*) malloc(sizeof(regex_t));
				ret = regcomp(enc->encpattern, (const char *)attr, REG_NOSUB|REG_ICASE);
				if (ret) {
					fprintf(stderr, "Error compiling regexp \"%s\" : %d\n", attr, ret);
					exit(1);
				}
			} else {
				enc->encpattern = 0;
			}

			attr = xmlGetProp(node, BAD_CAST "family");
			if (attr) {
				int ret;
				enc->fampattern = (regex_t*) malloc(sizeof(regex_t));
				ret = regcomp(enc->fampattern, (const char *)attr, REG_NOSUB|REG_ICASE);
				if (ret) {
					fprintf(stderr, "Error compiling regexp \"%s\" : %d\n", attr, ret);
					exit(1);
				}
			} else {
				enc->fampattern = 0;
			}

			attr = xmlGetProp(node, BAD_CAST "result");
			if (!attr) {
				fprintf(stderr, "Invalid pattern\n");
				exit(1);
			}
			enc->encoding = strdup((const char *)attr);

			++i;
		}
		node = node->next;
	}

	xmlFreeDoc(doc);
}

void close_fontenc() {
	int i;
	for (i = 0; i < fontenclen; ++i) {
		struct fontenc_t * enc = fontenctbl + i;
		if(enc->encpattern) {
			regfree(enc->encpattern);
			free(enc->encpattern);
		}
		if(enc->fampattern) {
			regfree(enc->fampattern);
			free(enc->fampattern);
		}
		free(enc->encoding);
	}
	free(fontenctbl);
}

char * get_font_name(const char * filename) {
	char * fontname;
	char * fstart, *fend;
	int len;

	fstart = strrchr(filename, '/') + 1;
	fend = strchr(fstart, '.');
	len = fend - fstart;

	fontname = malloc(len + 1);
	memcpy(fontname, fstart, len);
	fontname[len] = 0;
	return fontname;
}

const char * has_tfm_info(const char * name) {
	struct tfm_list * entry = tfms;
	while (entry) {
		if (strcmp(name, entry->file) == 0) {
			return entry->encoding;
		}
		entry = entry->next;
	}
	return 0;
}

void add_tfm_info(const char * file, const char * encname, const char * name) {
	char * fontname = get_font_name(file);
	struct tfm_list * entry;
	const char * encoding = 0, * penc;
	int i;

	for (i = 0; i < fontenclen; ++i) {
		if(
			(fontenctbl[i].encpattern == 0 || 0 == regexec(fontenctbl[i].encpattern, encname, 0, 0, 0))
		&&
			(fontenctbl[i].fampattern == 0 || 0 == regexec(fontenctbl[i].fampattern, name, 0, 0, 0))
		) {
				encoding = fontenctbl[i].encoding;
				break;
		}
	}
	if (encoding == 0) {
		encoding = fontenctbl[0].encoding;
		fprintf(stderr, "File \"%s\": Couldn't find a match for Font name \"%s\" and encoding name \"%s\", defaulting to \"%s\"\n", file, name, encname, encoding);
	}

	if ((penc = has_tfm_info(fontname))) {
		if (strcmp(penc, encoding)) {
			fprintf(stderr, "Warning, font \"%s\" set twice with mismatching encodings : \"%s\" and \"%s\".\n", fontname, encoding, penc);
		}
		free(fontname);
		return ;
	}

	entry = (struct tfm_list *) malloc(sizeof(struct tfm_list));
	entry->next = tfms;
	tfms = entry;
	entry->file = fontname;
	entry->encoding = strdup(encoding);
}

struct font_list {
	struct font_list * next;
	char * name;
};

struct encoding_list {
	struct encoding_list * next;
	struct font_list * fonts;
	char * name;
};

struct encoding_list * find_encoding(struct encoding_list ** plist, const char * encoding) {
	struct encoding_list * list = *plist;

	while(list) {
		if (strcmp(encoding, list->name) == 0) return list;
		list = list->next;
	}

	/* not found, allocate a new one */

	list = (struct encoding_list *) malloc(sizeof(struct encoding_list));
	list->name = strdup(encoding);
	list->fonts = 0;
	list->next = *plist;
	*plist = list;

	return list;
}

struct encoding_list * make_encoding_list(struct tfm_list * tfms) {
	struct encoding_list * ret = 0;
	while(tfms) {
		struct encoding_list * current = find_encoding(&ret, tfms->encoding);
		struct font_list * font = (struct font_list *) malloc(sizeof(struct font_list));

		font->name = strdup(tfms->file);

		font->next = current->fonts;
		current->fonts = font;

		tfms = tfms->next;
	}

	return ret;
}

int handle_tfm(const char * fname) {
	unsigned short size;
	FILE * fp = fopen(fname, "rb");
	fseek(fp, 2, SEEK_SET);
	fread(&size, 1, sizeof(size), fp);
	if (size < 19) {
		// add_tfm_info(fname, "*UNSPECIFIED*", "*UNSPECIFIED*");
		fprintf(stderr, "invalid TFM : %s", fname);
		fclose(fp);
		return 0;
	} else {
		char encoding[41], name[21];
		fseek(fp, 25 + 8, SEEK_SET);
		fread(encoding, 1, 40, fp);
		encoding[40] = 0;
		fread(name, 1, 20, fp);
		name[20] = 0;
		add_tfm_info(fname, encoding, name);
	}
	fclose(fp);
	return 0;
}

extern int find_in_path(const char *, const char *, int (*)(const char *));

static
int compare_tfm_entry(struct tfm_list ** a, struct tfm_list ** b) {
	return strcmp((*a)->file, (*b)->file);
}

static
void sort_tfm_list() {
	struct tfm_list ** result = 0;
	int count = 0, i;
	struct tfm_list * tfm = tfms;

	while(tfm) {
		++count;
		tfm = tfm->next;
	}

	result = (struct tfm_list **) malloc(count * sizeof(struct tfm_list *));
	if (!result) {
		perror("allocation error");
		exit(-1);
	}
	tfm = tfms;
	count = 0;
	while (tfm) {
		result[count++] = tfm;
		tfm = tfm->next;
	}
	qsort(result, count, sizeof(struct tfm_list *), compare_tfm_entry);
	tfms = result[0];
	for (i = 0; i < count - 1; ++i) {
		result[i]->next = result[i + 1];
	}
	result[count - 1]->next = 0;

	free(result);
}

int export_encodings() {
    xmlDocPtr doc;
    xmlNodePtr root;
	struct tfm_list * tfm;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "encodings");
	xmlDocSetRootElement(doc, root);

	tfm = tfms;
	while(tfm) {
		xmlNodePtr node = xmlNewChild(root, NULL, BAD_CAST "font", NULL);
		xmlNewProp(node, BAD_CAST "name", BAD_CAST tfm->file);
		xmlNewProp(node, BAD_CAST "encoding", BAD_CAST tfm->encoding);

		tfm = tfm->next;
	}


	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
    xmlMemoryDump();
	return 0;
}

int main(int argc, char * argv[]) {
	int i;

    LIBXML_TEST_VERSION;

	if (argc < 3) {
		fprintf(stderr, "Usage : %s <patterns> <path> [<path> ...]\n", argv[1]);
		return -1;
	}

	init_fontenc(argv[1]);
	for(i = 2; i < argc; ++i) {
		find_in_path(argv[i], ".tfm", handle_tfm);
	}
	close_fontenc();

	sort_tfm_list();

	export_encodings();

	return 0;
}
