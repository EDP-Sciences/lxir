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

#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <assert.h>

#if USE_KPSE
#include <kpathsea/kpathsea.h>
#endif

#include "libfontmap.h"

struct ligature_s {
	struct ligature_s * next;
	char * src;
	char * dst;
};

static struct ligature_s * ligatures = 0;

struct fontmap_s {
	struct fontmap_s * next;
	char * name;
	struct translation_info info;
};

static struct fontmap_s * fontmaps = 0;

struct fontenc_s {
	struct fontenc_s * next;
	char * name;
	struct fontmap_s * map;
};

static struct fontenc_s * fontencs = 0;
static struct fontenc_s * mathfontencs = 0;

struct accent_s {
	struct accent_s * next;
	char * base;
	char * accent;
	char * conv;
};

static struct accent_s * accents = 0;

struct doc_s {
	struct doc_s * next;
	xmlDocPtr doc;
};

static
xmlNodePtr find_first_element_node(xmlDocPtr doc) {
	xmlNodePtr node = doc->children;
	while (node && node->type != XML_ELEMENT_NODE)
		node = node->next;
	if (node) return node->children;
	else return 0;
}

static struct doc_s * read_config() {
	xmlDocPtr doc;
	xmlNodePtr node;
	struct doc_s * docs = 0;
#if USE_KPSE
	const char * kpse_path = kpse_find_file ("config.xml", kpse_xml_format, true);
	if (kpse_path) {
		doc = xmlReadFile(kpse_path, NULL, 0);
	} else
#endif
		doc = xmlReadFile(TEXMF_XML_DIR "/config.xml", NULL, 0);
	if(!doc) return 0;
	node = find_first_element_node(doc);
	if (strcmp((const char *)node->name, "lfm-config") != 0)
		return 0;
	node = node->children;
	while (node) {
		if(node->type == XML_ELEMENT_NODE && strcmp((const char *)node->name, "file") == 0) {
			const char * path = (const char *)xmlGetProp(node, BAD_CAST "path");
			if (path && strlen(path) > 0) {
				struct doc_s * doc;
				xmlDocPtr ptr;
				xmlChar * opt = xmlGetProp(node, BAD_CAST "optional");
#if USE_KPSE
				kpse_path = kpse_find_file (path, kpse_xml_format, true);
				if (kpse_path) {
					ptr = xmlReadFile(kpse_path, NULL, XML_PARSE_NOWARNING);
				} else {
#endif
				if (*path != '/') {
					char * p = malloc(strlen(TEXMF_XML_DIR) + strlen((const char *) path) + 2);
					sprintf(p, TEXMF_XML_DIR "/%s", path);
					ptr = xmlReadFile(p, NULL, XML_PARSE_NOWARNING);
					free(p);
				} else {
					ptr = xmlReadFile((const char *) path, NULL, XML_PARSE_NOWARNING);
				}
#if USE_KPSE
				}
#endif
				if(!ptr) {
					if(!opt || strcmp((const char *)opt, "0") == 0) {
						fprintf(stderr, "Unable to find file \"%s\"\n", path);
						return 0;
					}
				} else {
					doc = malloc(sizeof(struct doc_s));
					if(!doc) return 0;
					doc->doc = ptr;
					doc->next = docs;
					docs = doc;
				}
			}
		}
		node = node->next;
	}
	xmlFreeDoc(doc);
	return docs;
}

static int read_ligature_docs(xmlDocPtr doc) {
	xmlNodePtr node;
	
	node = find_first_element_node(doc);
	while (node) {
		if(node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, "ligature") == 0) {
			xmlChar * src, * dst;
			src = xmlGetProp(node, BAD_CAST "src");
			dst = xmlGetProp(node, BAD_CAST "dst");
			
			if (src && dst) {
				struct ligature_s * lig = (struct ligature_s *) malloc(sizeof(struct ligature_s));
				lig->next = ligatures;
				ligatures = lig;
				lig->src = strdup((const char *)src);
				lig->dst = strdup((const char *)dst);
			}
		}
		node = node->next;
	}
	return 0;
}

static char * find_ligature(char * src) {
	struct ligature_s * lig = ligatures;
	
	while (lig) {
		if(strcmp(src, lig->src) == 0)
			return lig->dst;
		lig = lig->next;
	}
	return src;
}

static int read_encoding_docs(xmlDocPtr doc) {
	xmlNodePtr node;
	
	node = find_first_element_node(doc);
	while (node) {
		if(node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, "encoding") == 0) {
			xmlChar * prop;
			xmlNodePtr chr;
			
			chr = node->children;
			prop = xmlGetProp(node, BAD_CAST "name");
			
			if (prop && chr) {
				struct fontmap_s * map = (struct fontmap_s *) malloc(sizeof(struct fontmap_s));
				map->next = fontmaps;
				fontmaps = map;
				map->name = strdup((const char *)prop);
				map->info.map = malloc(256*sizeof(char*));
				map->info.size = 0;
				while(chr) {
					if (chr->type == XML_ELEMENT_NODE) {
						char * src = (char *) xmlGetProp(chr, BAD_CAST "value"); 
						src = find_ligature(src);
						map->info.map[map->info.size++] = strdup(src);
					}
					chr = chr->next;
				}
			}
		}
		node = node->next;
	}
	return 0;
}

static struct fontmap_s * find_encoding(const char * name) {
	struct fontmap_s * map = fontmaps;
	while(map) {
		if (strcmp(map->name, name) == 0)
			return map;
		map = map->next;
	}
	return 0;
}

static int read_font_docs(xmlDocPtr doc) {
	xmlNodePtr node;
	
	node = find_first_element_node(doc);
	while (node) {
		
		if(node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, "font") == 0) {
			xmlChar * name, * encoding;
			struct fontmap_s * map;
			
			name = xmlGetProp(node, BAD_CAST "name");
			encoding = xmlGetProp(node, BAD_CAST "encoding");
			map = find_encoding((const char *) encoding);
			assert (name);
			if (map) {
				struct fontenc_s * font = (struct fontenc_s *)malloc(sizeof(struct fontenc_s));
				
				font->next = fontencs;
				fontencs = font;
				font->name = strdup((const char *)name);
				font->map = map;
				// fprintf(stderr, "Associated font \"%s\" with map \"%s\"\n", name, map->name);
			} else {
				fprintf(stderr, "Unknown encoding in font declaration %s : \"%s\"\n", name, encoding);
			}
		}
		
		node = node->next;
	}
	return 0;
}

static int read_mathfont_docs(xmlDocPtr doc) {
	xmlNodePtr node;
	
	node = find_first_element_node(doc);
	while (node) {
		
		if(node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, "mathfont") == 0) {
			xmlChar * name, * encoding;
			struct fontmap_s * map;
			
			name = xmlGetProp(node, BAD_CAST "name");
			encoding = xmlGetProp(node, BAD_CAST "encoding");
			map = find_encoding((const char *) encoding);
			assert(name);
			if (map) {
				struct fontenc_s * font = (struct fontenc_s *)malloc(sizeof(struct fontenc_s));
				
				font->next = mathfontencs;
				mathfontencs = font;
				font->name = strdup((const char *)name);
				font->map = map;
			} else {
				fprintf(stderr, "Unknown encoding in math font declaration %s : \"%s\"\n", name, encoding);
			}
		}
		
		node = node->next;
	}
	return 0;
}

static int read_accent_docs(xmlDocPtr doc) {
	xmlNodePtr node;
	
	node = find_first_element_node(doc);
	while (node) {
		
		if(node->type == XML_ELEMENT_NODE && strcmp((const char *) node->name, "accent") == 0) {
			xmlChar * base, * accent, * conv;
			struct accent_s * a;
			
			base = xmlGetProp(node, BAD_CAST "base");
			accent = xmlGetProp(node, BAD_CAST "accent");
			conv = xmlGetProp(node, BAD_CAST "char");

			a = (struct accent_s *)malloc(sizeof(struct accent_s));
				
			a->next = accents;
			accents = a;
			a->base = strdup((const char *)base);
			a->accent = strdup((const char *)accent);
			a->conv = strdup((const char *)conv);
		}
		
		node = node->next;
	}
	
	return 0;
}

int lfm_get_accent(const char * base, const char * accent, char * conv) {
	struct accent_s * a = accents;
	
	while(a) {
		if (strcmp(a->base, base) == 0 && strcmp(a->accent, accent) == 0) {
			strcpy(conv, a->conv);
			return 0;
		}
		a = a->next;
	}
	return -1;
}

static int read_data(struct doc_s * docs, int (*ptr)(xmlDocPtr)) {
	struct doc_s * doc = docs;
	while (doc) {
		int err = (*ptr)(doc->doc);
		if(err) return err;
		doc = doc->next;
	}
	return 0;
}

int lfm_init() {
	int err;
	struct doc_s * docs;
    
	LIBXML_TEST_VERSION;
	
	docs = read_config();
	if (!docs) return -1;
	
	err = read_data(docs, read_ligature_docs);
	if (err) return err;
	err = read_data(docs, read_encoding_docs);
	if (err) return err;
	err = read_data(docs, read_font_docs);
	if (err) return err;
	err = read_data(docs, read_mathfont_docs);
	if (err) return err;
	err = read_data(docs, read_accent_docs);
	if (err) return err;
	
	while(docs) {
		struct doc_s * next = docs->next;
		xmlFreeDoc(docs->doc);
		free(docs);
		docs = next;
	}
	
	return 0;
}

int lfm_close() {
	while (accents) {
		struct accent_s * next = accents->next;
		
		free(accents->base);
		free(accents->accent);
		free(accents->conv);
		free(accents);
		accents = next;
	}
	
	while (ligatures) {
		struct ligature_s * next = ligatures->next;
			
		free(ligatures->src);
		free(ligatures->dst);
		free(ligatures);
		ligatures = next;
	}
	
	while (fontencs) {
		struct fontenc_s * next = fontencs->next;
			
		free(fontencs->name);
		free(fontencs);
		fontencs = next;
	}
	
	while(fontmaps) {
		int i;
		struct fontmap_s * next = fontmaps->next;
		
		for(i = 0; i< fontmaps->info.size; ++i)
			free(fontmaps->info.map[i]);
		
		free(fontmaps->info.map);
		free(fontmaps->name);
		free(fontmaps);
		
		fontmaps = next;
	}
	return 0;
}

int lfm_get_translation_map(const char * fontname, struct translation_info * pinfo) {
	struct fontenc_s * font = fontencs;
	while(font) {
		if(strcmp(fontname, font->name) == 0) {
			*pinfo = font->map->info;
			return 0;
		}
		font = font->next;
	}
	fprintf(stderr, "Unknown font : %s\n", fontname);
	return lfm_get_translation_map("cmr10", pinfo);
}

int lfm_get_math_encoding_map(const char * mathfont, struct translation_info * pinfo) {
	struct fontenc_s * font = mathfontencs;
	while(font) {
		if(strcmp(mathfont, font->name) == 0) {
			*pinfo = font->map->info;
			return 0;
		}
		font = font->next;
	}
	fprintf(stderr, "Unknown math font : %s\n", mathfont);
	return lfm_get_math_encoding_map("tenrm", pinfo);
}
