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
#include <assert.h>

#include "libdvi.h"
#include "../libfontmap/libfontmap.h"

#include <kpathsea/kpathsea.h>

void dump_font(dvifont_t * font) {
	char * area = font->area;
	char * name = font->name;
	if (!area) area = "<NULL>";
	if (!name) name = "<NULL>";
	printf("Font<%d> : %s, %s, %d, %d, approximated word_space : %d\n", font->index, area, name, font->design_size, font->scale,
		font->design_size / 5  );
}

void dump_page(dvifont_t * fonts, dvinode_header_t * node) {
	while(node) {
		switch(node->type) {
			case DVINODE_TEXT: {
				int index;
				dvinode_text_t * text = (dvinode_text_t *) node;
				for(index = 0; fonts[index].index != text->font; ++index) ;
				printf("\tTEXT (%s, %d) : %s\n", fonts[index].name, text->size, text->content);
			} break;
			case DVINODE_RULE: printf("\tRULE\n"); break;
			case DVINODE_CONTROL: printf("\tCONTROL\n"); break;
			case DVINODE_BOP: printf("\tBOP\n"); break;
			case DVINODE_XXX: {
				dvinode_xxx_t * xxx = (dvinode_xxx_t *) node;
				printf("\tXXX : %s\n", xxx->content);
			} break;
			default: fprintf(stderr, "Invalid node type : %d\n", node->type); exit(-1);
		}

		node = node->next;
	}
}

int main(int argc, char * argv[]) {
	dvifile_t * f;
	int err;
	int p = 1;
	int flags = 0;
	const char * filename = 0;

	kpse_set_program_name(argv[0], "dvidump");

	err = 0;
	while (err == 0 && p < argc) {
		if(strcmp(argv[p], "-n") == 0) {
			flags |= DVI_NO_FONT_TRANSLATION;
			p = p + 1;
		} else if(strcmp(argv[p], "-skip") == 0) {
			if (p + 1 == argc)
				err = 1;
			else {
				p = p + 2;
				if (strcmp(argv[p - 1], "1") == 0) {
					flags |= DVI_SKIP_SPACE_SMALL;
				} else if (strcmp(argv[p - 1], "sp") == 0) {
					flags |= DVI_SKIP_SPACE_NORMAL;
				} else if (strcmp(argv[p - 1], "all") == 0) {
					flags |= DVI_SKIP_SPACE_NORMAL|DVI_SKIP_SPACE_SMALL;
				} else err = 1;
			}
		} else {
			if(filename) err = 1;
			else filename = argv[p++];
		}
	}

	if ((flags & (DVI_SKIP_SPACE_NORMAL|DVI_SKIP_SPACE_SMALL)) == 0) {
		flags |= DVI_SKIP_SPACE_SMALL;
	}

	if (!filename || err) {
		fprintf(stderr, "Usage : %s [-skip {1|sp|all}] [-n] <dvifile>\n", argv[0]);
		return err;
	}

	/* printf("flags = %d\n", flags); */

	err = lfm_init();
	if (err) {
		fprintf(stderr, "Error initialising libfontmap (%d)\n", err);
		return err;
	}

	err = dvi_read(&f, filename, flags);
	lfm_close();

	if(!err) {
		printf("DVI Comment : \"%s\"\n", f->comment);
		printf("DVI file has %d pages\n", f->nbpages);

		int i;
		for(i = 0; i < f->nbfonts; ++i) {
			dump_font(&f->fonts[i]);
		}

		for(i = 0; i < f->nbpages; ++i) {
			printf("Page %d\n", i+1);
			dump_page(f->fonts, f->pages[i]);
		}

		dvi_destroy(f);
	} else {
		fprintf(stderr, "Error reading DVI file (%d)\n", err);
		return err;
	}

	return 0;
}
