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
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "libdvi.h"
#include "../libfontmap/libfontmap.h"

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#define POSTAMBLE_MAX_SIZE 32
#define POSTAMBLE_CHAR 223
#define DVI_VERSION_CHAR 2


#define OPCODE_SET_CHAR0	0
#define OPCODE_SET_CHAR127	127
#define OPCODE_SET1 128
#define OPCODE_SET2 129
#define OPCODE_SET3 130
#define OPCODE_SET4 131
#define OPCODE_SET_RULE 132
#define OPCODE_PUT1 133
#define OPCODE_PUT2 134
#define OPCODE_PUT3 135
#define OPCODE_PUT4 136
#define OPCODE_PUT_RULE 137
#define OPCODE_NOP 138
#define OPCODE_BOP 139
#define OPCODE_EOP 140
#define OPCODE_PUSH 141
#define OPCODE_POP 142
#define OPCODE_RIGHT1 143
#define OPCODE_RIGHT2 144
#define OPCODE_RIGHT3 145
#define OPCODE_RIGHT4 146
#define OPCODE_W0 147
#define OPCODE_W1 148
#define OPCODE_W2 149
#define OPCODE_W3 150
#define OPCODE_W4 151
#define OPCODE_X0 152
#define OPCODE_X1 153
#define OPCODE_X2 154
#define OPCODE_X3 155
#define OPCODE_X4 156
#define OPCODE_DOWN1 157
#define OPCODE_DOWN2 158
#define OPCODE_DOWN3 159
#define OPCODE_DOWN4 160
#define OPCODE_Y0 161
#define OPCODE_Y1 162
#define OPCODE_Y2 163
#define OPCODE_Y3 164
#define OPCODE_Y4 165
#define OPCODE_Z0 166
#define OPCODE_Z1 167
#define OPCODE_Z2 168
#define OPCODE_Z3 169
#define OPCODE_Z4 170
#define OPCODE_FONT_NUM0 171
 /* ... */
#define OPCODE_FONT_NUM63 234
#define OPCODE_FONT1 235
#define OPCODE_FONT2 236
#define OPCODE_FONT3 237
#define OPCODE_FONT4 238
#define OPCODE_XXX1 239
#define OPCODE_XXX2 240
#define OPCODE_XXX3 241
#define OPCODE_XXX4 242
#define OPCODE_FONT_DEF1 243
#define OPCODE_FONT_DEF2 244
#define OPCODE_FONT_DEF3 245
#define OPCODE_FONT_DEF4 246
#define OPCODE_PRE 247
#define OPCODE_POST 248
#define OPCODE_POST_POST 249

#define NDEBUG 1
#ifndef NDEBUG
# define DUMP(format ...) DUMP (format, __VA_ARGS__)
#else
# define DUMP(...)
#endif


/* Internal state of the dvi parser */
typedef struct dvistate_s {
	int h, v, w, x, y, z;
} dvistate_t;

typedef struct dvistack_s {
	dvistate_t state;
	struct dvistack_s * next;
} dvistack_t;

typedef struct dvifilestate_s {
	FILE * fp;
	int close_fp;
	dvifile_t * file;
	int current_font;
	dvifont_t * font;

	dvinode_header_t * current_node;
	dvistack_t * stack;
	int buffer_size;
	int size;
	char * buffer;
	int flags;
} dvifilestate_t;

int dvi_push(dvifilestate_t *);
int dvi_pop(dvifilestate_t *);
int dvi_append_text_node(dvifilestate_t *);
int dvi_close_text_node(dvifilestate_t *);
int dvi_extend_buffer(dvifilestate_t *, int);
int dvi_append_text(dvifilestate_t *, int);
int dvi_append_node(dvifilestate_t *, dvinode_header_t *);

int dvi_new(dvifilestate_t **, const char * filename);
int dvi_close(dvifilestate_t *, dvifile_t **);

/* Implementation */
int get_word_space(dvifilestate_t * s) {
	if (!s->font) return 0;
	return s->font->design_size / 5;
}

int is_space_skip(dvifilestate_t * s, int a) {
	return (
		((s->flags & DVI_SKIP_SPACE_SMALL) && a == 1) ||
		((s->flags & DVI_SKIP_SPACE_NORMAL) && a > get_word_space(s))
	);
}

int dvi_push(dvifilestate_t * s) {
	dvistack_t * new_state = (dvistack_t *) malloc(sizeof(dvistack_t));

	if (!new_state) return DVIERR_NOMEM;

	new_state->state = s->stack->state;
	new_state->next = s->stack;
	s->stack = new_state;

	return 0;
}

int dvi_pop(dvifilestate_t * s) {
	dvistack_t * state = s->stack;

	if (!state->next) return DVIERR_INVALID_POP;

	s->stack = state->next;
	free(state);

	return 0;
}

int dvi_stack_size(dvifilestate_t * s) {
	int n = 0;
	dvistack_t * state = s->stack;
	while (state) {
		n++;
		state = state->next;
	}
	return n;
}

int dvi_append_node(dvifilestate_t * s, dvinode_header_t * node) {
	s->current_node->next = node;
	s->current_node = node;

	return 0;
}

int dvi_append_text_node(dvifilestate_t * s) {
	dvinode_text_t * node = (dvinode_text_t *) malloc(sizeof(dvinode_text_t)); /* fixme */

	if (!node) return DVIERR_NOMEM;

	node->header.type = DVINODE_TEXT;
	node->header.next = 0;
	node->font = s->current_font;
	node->h = s->stack->state.h;
	node->v = s->stack->state.v;
	node->size = 0;
	node->content = 0;

	return dvi_append_node(s, &node->header);
}

int dvi_close_text_node(dvifilestate_t * s) {
	dvinode_text_t * c = (dvinode_text_t *) s->current_node;

	if (!c || c->header.type != DVINODE_TEXT || c->content) return 0;

	if((s->flags & DVI_SKIP_SPACE_NORMAL) == 1 && s->size > 0 && s->buffer[s->size - 1] == ' ') s->size--;

	// fixme : what if size == 0 ?

	c->content = malloc(s->size + 1);

	if (!c->content) return DVIERR_NOMEM;

	memcpy(c->content, s->buffer, s->size);
	c->content[s->size]  = 0;
	// DUMP("TEXT : \"%s\"\n", c->content);
	c->size = s->size;
	s->size = 0;

	return 0;
}

int dvi_append_rule_node(dvifilestate_t * s, int a, int b) {
	int err = dvi_close_text_node(s);
	if (err) return err;

	dvinode_rule_t * node = (dvinode_rule_t *) malloc(sizeof(dvinode_rule_t));

	if (!node) return DVIERR_NOMEM;

	node->header.type = DVINODE_RULE;
	node->header.next = 0;
	node->h = s->stack->state.h;
	node->v = s->stack->state.v;
	node->a = a;
	node->b = b;

	return dvi_append_node(s, &node->header);
}

int dvi_append_xxx_node(dvifilestate_t * s, char * xxx) {
	int err = dvi_close_text_node(s);
	if (err) return err;

	dvinode_xxx_t * node = (dvinode_xxx_t *) malloc(sizeof(dvinode_xxx_t));

	if (!node) return DVIERR_NOMEM;

	node->header.type = DVINODE_XXX;
	node->header.next = 0;
	node->size = strlen(xxx);
	node->content = xxx;

	return dvi_append_node(s, &node->header);
}

int dvi_append_control_node(dvifilestate_t * s, int type, int param) {
	int err = dvi_close_text_node(s);
	if (err) return err;

	dvinode_control_t * node = (dvinode_control_t *) malloc(sizeof(dvinode_control_t));

	if (!node) return DVIERR_NOMEM;

	node->header.type = DVINODE_CONTROL;
	node->header.next = 0;
	node->type = type;
	node->param = param;

	return dvi_append_node(s, &node->header);
}

int dvi_extend_buffer(dvifilestate_t * s, int new_size) {
	char * new_buffer = malloc(new_size);
	if(!new_buffer) return DVIERR_NOMEM;

	memcpy(new_buffer, s->buffer, s->size);
	free(s->buffer);
	s->buffer = new_buffer;
	s->buffer_size = new_size;

	return 0;
}

int dvi_append_text(dvifilestate_t * s, int ch) {
	dvinode_text_t * c = (dvinode_text_t *) s->current_node;
	/* will create a new DVINODE_TEXT if there is no current */
	if (!c || c->header.type != DVINODE_TEXT || c->content) {
		int err = dvi_append_text_node (s);
		if (err) return err;
	}
	if(!(s->flags & DVI_NO_FONT_TRANSLATION)) {
		char * chr;
		int l, size;
		if(ch < 0 || ch >= s->font->mapsize) {
			return DVIERR_INVALID_CHAR;
		}

		chr = s->font->map[ch];
		l = strlen(chr);
		size = s->size + l;
		if (size > s->buffer_size) {
			int err = dvi_extend_buffer(s, MAX(size, s->buffer_size * 2));
			if (err) return err;
		}

		memcpy(s->buffer + s->size, chr, l);
		s->size += l;
	} else {
		int size = s->size + 1;
		if (size > s->buffer_size) {
			int err = dvi_extend_buffer(s, MAX(size, s->buffer_size * 2));
			if (err) return err;
		}
		s->buffer[s->size] = ch;
		s->size += 1;
	}

	return 0;
}

int dvi_append_space(dvifilestate_t * s) {
	dvinode_text_t * c = (dvinode_text_t *) s->current_node;
	if (!c || c->header.type != DVINODE_TEXT || c->content) {
		int err;
		if (s->flags & DVI_SKIP_SPACE_NORMAL) return 0;
		err = dvi_append_text_node (s);
		if (err) return err;
	}
	if ((s->flags & DVI_SKIP_SPACE_NORMAL) == 0 || (s->size > 0 && s->buffer[s->size - 1] != ' ')) {
		if (s->size >= s->buffer_size) {
			int err = dvi_extend_buffer(s, MAX(s->size + 1, s->buffer_size * 2));
			if (err) return err;
		}
		s->buffer[s->size++] = ' ';
	}
	return 0;
}

int dvi_new_fp(dvifilestate_t ** ps, FILE * fp, int close) {
	dvifilestate_t * s = malloc(sizeof(dvifilestate_t));
	if(!s) return DVIERR_NOMEM;

	s->flags = 0;
	s->fp = fp;
	s->close_fp = close;
	if (!s->fp) {
		free(s);
		return DVIERR_INVALID_FILEP;
	}

	s->file = malloc(sizeof(dvifile_t));
	if(!s->file) {
		if(s->close_fp) fclose(s->fp);
		free(s);
		return DVIERR_NOMEM;
	}

	s->file->nbfonts = 0;
	s->file->fonts = 0;
	s->file->nbpages = 0;
	s->file->pages = 0;

	s->current_node = 0;
	s->stack = 0;
	s->size = 0;
	s->buffer_size = 1024;
	s->buffer = malloc(s->buffer_size);
	if(!s->buffer) {
		free(s->file);
		if(s->close_fp) fclose(s->fp);
		free(s);
		return DVIERR_NOMEM;
	}

	*ps = s;
	return 0;
}

int dvi_new(dvifilestate_t ** ps, const char * filename) {
	return dvi_new_fp(ps, fopen(filename, "rb"), 1);
}

int dvi_close(dvifilestate_t * s, dvifile_t ** pf) {
	if(s->close_fp) fclose(s->fp);
	dvi_close_text_node(s);

	*pf = s->file;

	free(s->buffer);
	free(s);

	return 0;
}

int dvi_read_bytes(dvifilestate_t * s, int size, char * buffer) {
	return fread(buffer, 1, size, s->fp) != size;
}


int dvi_read_number_1(dvifilestate_t *s, int * pn) {
	union {
		signed char number;
		unsigned char byte;
	} v;
	int c = fgetc(s->fp);
	if (c == EOF) return DVIERR_UNEXPECTED_EOF;
	v.byte = c;
	*pn = v.number;

	return 0;
}

int dvi_read_number_2(dvifilestate_t *s, int * pn) {
	union {
		signed short number;
		unsigned char bytes[2];
	} v;
	int i, c;
	for (i = 0; i < 2; ++i) {
		c = fgetc(s->fp);
		if (c == EOF) return DVIERR_UNEXPECTED_EOF;
		v.bytes[1 - i] = c;
	}

	//~ DUMP("0x%2.2x%2.2x => %d\n", v.bytes[0], v.bytes[1], v.number);
	*pn = v.number;

	return 0;
}

int dvi_read_number_3(dvifilestate_t *s, int * pn) {
	union {
		signed int number;
		unsigned char bytes[4];
	} v;
	int i, c;
	for (i = 0; i < 3; ++i) {
		c = fgetc(s->fp);
		if (c == EOF) return DVIERR_UNEXPECTED_EOF;
		v.bytes[2 - i] = c;
	}
	v.bytes[3] = v.bytes[2] & 0x80 ? 0xFF : 0;

	//~ DUMP("0x%2.2x%2.2x%2.2x%2.2x => %d\n", v.bytes[0], v.bytes[1], v.bytes[2], v.bytes[3], v.number);

	*pn = v.number;

	return 0;
}

int dvi_read_number_4(dvifilestate_t *s, int * pn) {
	union {
		signed int number;
		unsigned char bytes[4];
	} v;
	int i, c;
	for (i = 0; i < 4; ++i) {
		c = fgetc(s->fp);
		if (c == EOF) return DVIERR_UNEXPECTED_EOF;
		v.bytes[3 - i] = c;
	}
	//~ DUMP("0x%2.2x%2.2x%2.2x%2.2x => %d\n", v.bytes[0], v.bytes[1], v.bytes[2], v.bytes[3], v.number);

	*pn = v.number;

	return 0;
}

int dvi_read_number(dvifilestate_t *s, int size, int * pn) {
	switch(size) {
		case 1: return dvi_read_number_1(s, pn);
		case 2: return dvi_read_number_2(s, pn);
		case 3: return dvi_read_number_3(s, pn);
		case 4: return dvi_read_number_4(s, pn);
	}
	return DVIERR_INVALID_NUMBER_SIZE;
}

int dvi_read_number_1_u(dvifilestate_t *s, unsigned int * pn) {
	union {
		unsigned char number;
		unsigned char byte;
	} v;
	int c = fgetc(s->fp);
	if (c == EOF) return DVIERR_UNEXPECTED_EOF;
	v.byte = c;
	*pn = v.number;

	return 0;
}

int dvi_read_number_2_u(dvifilestate_t *s, unsigned int * pn) {
	union {
		unsigned short number;
		unsigned char bytes[2];
	} v;
	int i, c;
	for (i = 0; i < 2; ++i) {
		c = fgetc(s->fp);
		if (c == EOF) return DVIERR_UNEXPECTED_EOF;
		v.bytes[1 - i] = c;
	}

	//~ DUMP("0x%2.2x%2.2x => %d\n", v.bytes[0], v.bytes[1], v.number);
	*pn = v.number;

	return 0;
}

int dvi_read_number_3_u(dvifilestate_t *s, unsigned int * pn) {
	union {
		unsigned int number;
		unsigned char bytes[4];
	} v;
	int i, c;
	for (i = 0; i < 3; ++i) {
		c = fgetc(s->fp);
		if (c == EOF) return DVIERR_UNEXPECTED_EOF;
		v.bytes[2 - i] = c;
	}
	v.bytes[3] = 0;

	//~ DUMP("0x%2.2x%2.2x%2.2x%2.2x => %d\n", v.bytes[0], v.bytes[1], v.bytes[2], v.bytes[3], v.number);

	*pn = v.number;

	return 0;
}

int dvi_read_number_4_u(dvifilestate_t *s, unsigned int * pn) {
	union {
		unsigned int number;
		unsigned char bytes[4];
	} v;
	int i, c;
	for (i = 0; i < 4; ++i) {
		c = fgetc(s->fp);
		if (c == EOF) return DVIERR_UNEXPECTED_EOF;
		v.bytes[3 - i] = c;
	}
	//~ DUMP("0x%2.2x%2.2x%2.2x%2.2x => %d\n", v.bytes[0], v.bytes[1], v.bytes[2], v.bytes[3], v.number);

	*pn = v.number;

	return 0;
}

int dvi_read_number_u(dvifilestate_t *s, int size, unsigned int * pn) {
	switch(size) {
		case 1: return dvi_read_number_1_u(s, pn);
		case 2: return dvi_read_number_2_u(s, pn);
		case 3: return dvi_read_number_3_u(s, pn);
		case 4: return dvi_read_number_4_u(s, pn);
	}
	return DVIERR_INVALID_NUMBER_SIZE;
}

int dvi_get_postamble_offset(dvifilestate_t * s, long * po) {
	int i, o, err;
	long offset;

	unsigned char buffer[POSTAMBLE_MAX_SIZE];

	err = fseek(s->fp, -POSTAMBLE_MAX_SIZE, SEEK_END);
	if (err) return DVIERR_INVALID_FSEEK;

	err = fread(buffer, 1, POSTAMBLE_MAX_SIZE, s->fp);
	if (err != POSTAMBLE_MAX_SIZE) return DVIERR_INVALID_READ;

	for (i = POSTAMBLE_MAX_SIZE-1; i >= 0 && buffer[i] == POSTAMBLE_CHAR; --i) ;

	if (i < 6 || buffer[i] != DVI_VERSION_CHAR || buffer[i-5] != OPCODE_POST_POST)
		return DVIERR_INVALID_POSTAMBLE;

	offset = 0;
	for (o = i-4; o < i; ++o)
		offset = (offset << 8) | buffer[o];

	*po = offset;

	return 0;
}

int dvi_read_nbpages(dvifilestate_t * s) {
	int n, err;
	dvinode_header_t ** pages;

	err = dvi_read_number(s, 2, &n);
	if (err) return err;

	pages = malloc(sizeof(dvinode_header_t *)*n);
	if(!pages) return DVIERR_NOMEM;
	memset(pages, 0, sizeof(dvinode_header_t *)*n);

	s->file->nbpages = n;
	s->file->pages = pages;

	return 0;
}

int dvi_read_postamble(dvifilestate_t * s) {
	int c, err;
	long offset;
	int nfont = 0;
	struct font_list_s {
		dvifont_t current;
		struct font_list_s * next;
	} * list = 0;

	err = dvi_get_postamble_offset(s, &offset);
	if (err) return err;

	err = fseek(s->fp, offset, SEEK_SET);
	if (err != 0) return DVIERR_INVALID_FSEEK;

	c = fgetc(s->fp);
	if (c != OPCODE_POST) return DVIERR_INVALID_POSTAMBLE;

	/* read postamble and fill fonts */
	err = dvi_read_number(s, 4, &c); /* c = offset to last bop */
	if (err) return err;

	err = dvi_read_number(s, 4, &(s->file->num));
	if (err) return err;
	err = dvi_read_number(s, 4, &(s->file->den));
	if (err) return err;
	err = dvi_read_number(s, 4, &(s->file->mag));
	if (err) return err;
	err = dvi_read_number(s, 4, &c); /* c = biggest left size of a page */
	if (err) return err;
	err = dvi_read_number(s, 4, &c); /* c = biggest up size of a page */
	if (err) return err;
	err = dvi_read_number(s, 2, &c); /* c = max stack size */
	if (err) return err;
	err = dvi_read_nbpages(s);
	if (err) return err;

	while (1) {
		c = fgetc(s->fp);
		if (c == OPCODE_POST_POST) {
			break;
		} else if (c >= OPCODE_FONT_DEF1 && c <= OPCODE_FONT_DEF4) {
			int a, l;
			struct font_list_s * pfont;

			++nfont;

			pfont = malloc(sizeof(struct font_list_s));
			if (!pfont) return -1; /* fixme : more cleanup is necessary */

			err = dvi_read_number(s, c - OPCODE_FONT_DEF1 + 1, &pfont->current.index);
			if (err) return err;
			err = dvi_read_number(s, 4, &pfont->current.checksum);
			if (err) return err;
			err = dvi_read_number(s, 4, &pfont->current.scale);
			if (err) return err;
			err = dvi_read_number(s, 4, &pfont->current.design_size);
			if (err) return err;
			a = fgetc(s->fp);
			if (a == EOF) return DVIERR_UNEXPECTED_EOF;
			l = fgetc(s->fp);
			if (l == EOF) return DVIERR_UNEXPECTED_EOF;

			if (a == 0) {
				pfont->current.area = 0;
			} else {
				pfont->current.area = malloc(a + 1);
				if (!pfont->current.area) return DVIERR_NOMEM;
				err = dvi_read_bytes(s, a, pfont->current.area);
				pfont->current.area[a] = 0;
			}

			if (l == 0) {
				pfont->current.name = 0;
			} else {
				pfont->current.name = malloc(l + 1);
				if (!pfont->current.name) return DVIERR_NOMEM;
				err = dvi_read_bytes(s, l, pfont->current.name);
				pfont->current.name[l] = 0;
			}

			if (!(s->flags & DVI_NO_FONT_TRANSLATION)) {
				struct translation_info ti;
				err = lfm_get_translation_map(pfont->current.name ? pfont->current.name : "cmr10", &ti);
				if (err) return DVIERR_NOTRANSLATION_MAP;

				pfont->current.map = ti.map;
				pfont->current.mapsize = ti.size;
			}

			pfont->next = list;
			list = pfont;

		} else if (c != OPCODE_NOP) {
			fprintf(stderr, "Invalid opcode \"%d\" between POST and POST_POST\n", c);
			return DVIERR_INVALID_OPCODE;
		}
	}

	s->file->nbfonts = nfont;
	s->file->fonts = malloc(sizeof(dvifont_t) * nfont);
	c = 0;
	while(list) {
		struct font_list_s * next = list->next;

		memcpy(&s->file->fonts[c++], &list->current, sizeof(dvifont_t));

		free(list);

		list = next;
	}

	assert (c == nfont);

	return 0;
}

int dvi_read_preamble(dvifilestate_t * s) {
	int c, err;

	err = fseek(s->fp, 0, SEEK_SET);
	if (err != 0) return DVIERR_INVALID_FSEEK;

	c = fgetc(s->fp);

	if (c != OPCODE_PRE) {
		fprintf(stderr, "Invalid Preamble code \"%d\"", c);
		return DVIERR_INVALID_OPCODE;
	}

	c = fgetc(s->fp);
	if (c != DVI_VERSION_CHAR) {
		fprintf(stderr, "unknown DVI version\n");
		return DVIERR_UNKNOWN_VERSION;
	}

	err = dvi_read_number(s, 4, &c);
	if (err) return err;
	assert (c == s->file->num);
	err = dvi_read_number(s, 4, &c);
	if (err) return err;
	assert (c == s->file->den);
	err = dvi_read_number(s, 4, &c);
	if (err) return err;
	assert (c == s->file->mag);

	c = fgetc(s->fp);
	if (c == EOF) return DVIERR_UNEXPECTED_EOF;
	s->file->comment = malloc(c+1);
	if(!s->file->comment) return DVIERR_NOMEM;
	dvi_read_bytes(s, c, s->file->comment);
	s->file->comment[c] = 0;

	return 0;
}

int dvi_check_font(dvifilestate_t * s) {
	int i;
	s->font = 0;
	for(i = 0; i < s->file->nbfonts; ++i) {
		if (s->current_font == s->file->fonts[i].index) {
			s->font = &s->file->fonts[i];
			return 0;
		}
	}
	fprintf(stderr, "Invalid font \"%d\" set in page\n", s->current_font);
	return DVIERR_INVALID_FONT;
}

int dvi_read_page_opcodes(dvifilestate_t * s) {
	int c, err, a;
	while (1) {
		c = fgetc(s->fp);
		if (c >= OPCODE_SET_CHAR0 && c <= OPCODE_SET_CHAR127) {
			DUMP("OPCODE : %d : CHAR : %c\n", c, c);
			err = dvi_append_text(s, c);
			if (err) return err;
		} else if (c >= OPCODE_SET1 && c <= OPCODE_SET4) {
			err = dvi_read_number(s, c - OPCODE_SET1 + 1, &a);
			a = *((unsigned char *)&a);
			if (err) return err;
			DUMP("OPCODE : %d : SET (%d)\n", c, a);
			err = dvi_append_text(s, a); // ??
			if (err) return err;
		} else if (c == OPCODE_SET_RULE) {
			int b;
			DUMP("OPCODE : %d : SET_RULE\n", c);
			err = dvi_read_number(s, 4, &a);
			if (err) return err;
			err = dvi_read_number(s, 4, &b);
			if (err) return err;
			err = dvi_append_rule_node(s, a, b);
			if (err) return err;
			s->stack->state.h += b;
		} else if (c >= OPCODE_SET1 && c <= OPCODE_SET4) {
			err = dvi_read_number(s, c - OPCODE_SET1 + 1, &a);
			if (err) return err;
			DUMP("OPCODE : %d : PUT (%d)\n", c, a);
			err = dvi_append_text(s, a); /* fixme : probably not */
			if (err) return err;
		} else if (c == OPCODE_PUT_RULE) {
			int b;
			DUMP("OPCODE : %d : PUT_RULE\n", c);
			err = dvi_read_number(s, 4, &a);
			if (err) return err;
			err = dvi_read_number(s, 4, &b);
			if (err) return err;
			err = dvi_append_rule_node(s, a, b);
			if (err) return err;
		} else if (c == OPCODE_NOP) {
			DUMP("OPCODE : %d : NOP\n", c);
		/* } else if (c == OPCODE_BOP) { */
		} else 	if (c == OPCODE_EOP) {
			dvi_close_text_node(s);
			DUMP("OPCODE : %d : EOP\n", c);
			break;
		} else if (c == OPCODE_PUSH) {
			DUMP("OPCODE : %d : PUSH\n", c);
			err = dvi_push(s);
			if (err) return err;
			err = dvi_append_control_node(s, DVICONTROL_PUSH, 0);
			if (err) return err;
		} else if (c == OPCODE_POP) {
			DUMP("OPCODE : %d : POP\n", c);
			err = dvi_pop(s);
			if (err) return err;
			err = dvi_append_control_node(s, DVICONTROL_POP, 0);
			if (err) return err;
		} else if (c >= OPCODE_RIGHT1 && c <= OPCODE_RIGHT4) {
			err = dvi_read_number(s, c - OPCODE_RIGHT1 + 1, &a);
			if (err) return err;
			err = dvi_append_control_node(s, DVICONTROL_RIGHT, a);
			if (err) return err;
			DUMP("OPCODE : %d : RIGHT (%d, %f)\n", c, a, (float)a / get_word_space(s));
			if (is_space_skip(s, a)) {
				err = dvi_append_space(s);
				if (err) return err;
			}
			s->stack->state.h += a;
		} else if (c >= OPCODE_W0 && c <= OPCODE_W4) {
			if (c !=  OPCODE_W0) {
				err = dvi_read_number(s, c - OPCODE_W1 + 1, &a);
				if (err) return err;
				err = dvi_append_control_node(s, DVICONTROL_W, a);
				if (err) return err;
				s->stack->state.w = a;
			} else {
				a = s->stack->state.w;
			}
			DUMP("OPCODE : %d : W (%d, %f)\n", c, a, (float)a / get_word_space(s));
			s->stack->state.h += a;
			if (is_space_skip(s, a)) {
				err = dvi_append_space(s);
				if (err) return err;
			}
		} else if (c >= OPCODE_X0 && c <= OPCODE_X4) {
			if (c !=  OPCODE_X0) {
				err = dvi_read_number(s, c - OPCODE_X1 + 1, &a);
				if (err) return err;
				err = dvi_append_control_node(s, DVICONTROL_X, a);
				if (err) return err;
				s->stack->state.x = a;
			} else {
				a = s->stack->state.x;
			}
			DUMP("OPCODE : %d : X (%d, %f)\n", c, a, (float)a / get_word_space(s));
			s->stack->state.h += a;
			if (is_space_skip(s, a)) {
				err = dvi_append_space(s);
				if (err) return err;
			}
		} else if (c >= OPCODE_DOWN1 && c <= OPCODE_DOWN4) {
			dvi_close_text_node(s);
			err = dvi_read_number(s, c - OPCODE_DOWN1 + 1, &a);
			if (err) return err;
			DUMP("OPCODE : %d : DOWN (%d)\n", c, a);
			s->stack->state.v += a;
			err = dvi_append_control_node(s, DVICONTROL_DOWN, a);
			if (err) return err;
		} else if (c >= OPCODE_Y0 && c <= OPCODE_Y4) {
			dvi_close_text_node(s);
			if (c !=  OPCODE_Y0) {
				err = dvi_read_number(s, c - OPCODE_Y1 + 1, &a);
				if (err) return err;
				s->stack->state.y = a;
				err = dvi_append_control_node(s, DVICONTROL_Y, a);
				if (err) return err;
			}
			DUMP("OPCODE : %d : Y (%d)\n", c, s->stack->state.y);
			s->stack->state.v += s->stack->state.y;
		} else if (c >= OPCODE_Z0 && c <= OPCODE_Z4) {
			dvi_close_text_node(s);
			if (c !=  OPCODE_Z0) {
				err = dvi_read_number(s, c - OPCODE_Z1 + 1, &a);
				if (err) return err;
				s->stack->state.z = a;
				err = dvi_append_control_node(s, DVICONTROL_Z, a);
				if (err) return err;
			}
			DUMP("OPCODE : %d : Z (%d)\n", c, s->stack->state.z);
			s->stack->state.v += s->stack->state.z;
		} else if (c >= OPCODE_FONT_NUM0 && c <= OPCODE_FONT_NUM63) {
			dvi_close_text_node(s);
			s->current_font = c - OPCODE_FONT_NUM0;
			DUMP("OPCODE : %d : FONT (%d)\n", c, s->current_font);
			err = dvi_check_font(s);
			if (err) return err;
		} else if (c >= OPCODE_FONT1 && c <= OPCODE_FONT4) {
			dvi_close_text_node(s);
			err = dvi_read_number(s, c - OPCODE_FONT1 + 1, &a);
			if (err) return err;
			s->current_font = a;
			DUMP("OPCODE : %d : FONT (%d)\n", c, s->current_font);
			err = dvi_check_font(s);
			if (err) return err;
		} else if (c >= OPCODE_XXX1 && c <= OPCODE_XXX4) {
			dvi_close_text_node(s);
			char * buffer;
			err = dvi_read_number_u(s, c - OPCODE_XXX1 + 1, &a);
			if (err) return err;

			buffer = malloc(a+1);
			if (!buffer) return -1;
			err = dvi_read_bytes(s, a, buffer);
			if (err) return err;
			buffer[a] = 0;

			DUMP("OPCODE : %d : XXX (%s)\n", c, buffer);
			err = dvi_append_xxx_node(s, buffer);
			if (err) return err;

		} else if (c >= OPCODE_FONT_DEF1 && c <= OPCODE_FONT_DEF4) {
			int b, l;
			err = dvi_read_number(s, c - OPCODE_FONT_DEF1 + 1, &a);
			if (err) return err;
			fseek(s->fp, 12, SEEK_CUR); /* get in decl */
			b = fgetc(s->fp);
			if (b == EOF) return DVIERR_UNEXPECTED_EOF;
			l = fgetc(s->fp);
			if (l == EOF) return DVIERR_UNEXPECTED_EOF;
			fseek(s->fp, b+l, SEEK_CUR);

			DUMP("OPCODE : %d : FONT_DEF (%d)\n", c, a);
		/* } else if (c == OPCODE_PRE) { */
		/* } else if (c == OPCODE_POST) { */
		/* } else if (c == OPCODE_POST_POST) { */
		} else {
			fprintf(stderr, "Invalid Opcode \"%d\" in page\n", c);
			return DVIERR_INVALID_OPCODE;
		}
	}
	return 0;
}

int dvi_read_page(dvifilestate_t * s, int npage) {
	dvinode_bop_t * c;
	int i, err;
	s->stack = malloc(sizeof(dvistack_t));
	if(!s->stack) return DVIERR_NOMEM;
	memset(&s->stack->state, 0, sizeof(dvistate_t));
	s->stack->next = 0;

	c = malloc(sizeof(dvinode_bop_t));
	if(!c) return DVIERR_NOMEM;

	c->header.type = DVINODE_BOP;
	c->header.next = 0;
	s->current_node = &c->header;

	for (i=0; i < 10; ++i) {
		err = dvi_read_number(s, 4, &c->counts[i]);
		if (err) return err;
	}

	err = dvi_read_number(s, 4, &i);
	if (err) return err;

	s->file->pages[npage] = s->current_node;

	/* read opcodes */
	err = dvi_read_page_opcodes(s);
	if (err) return err;

	/* fixme, check empty stack */

	if (s->stack->next) {
		fprintf(stderr, "Invalid stack state at the end of a page !\n");
		return DVIERR_INVALID_STACK;
	}

	return 0;
}

int dvi_read_pages(dvifilestate_t * s) {
	int page = 0;
	while (1) {
		int c = fgetc(s->fp);
		if (c == OPCODE_BOP) {
			int err;
			if (page >= s->file->nbpages) {
				fprintf(stderr, "Too many pages in DVI\n");
				return DVIERR_TOO_MANY_PAGES;
			}
			err = dvi_read_page(s, page++);
			if (err) return err;
		} else if (c == OPCODE_POST) {
			/* found the end, return */
			return 0;
		} else if (c != OPCODE_NOP) {
			fprintf(stderr, "Invalid Opcode \"%d\" between pages\n", c);
			return DVIERR_INVALID_OPCODE;
		}
	}
	return 0;
}

int dvi_read(dvifile_t ** pf, const char * filename, int flags) {
	int err;
	dvifilestate_t * s;

	if( (flags & (DVI_SKIP_SPACE_SMALL|DVI_SKIP_SPACE_NORMAL)) == 0) {
		return DVIERR_INVALID_FLAGS;
	}

	err = dvi_new(&s, filename);
	if (err) return err;

	s->flags = flags;

	err = dvi_read_postamble(s);
	if (err) return err;

	err = dvi_read_preamble(s);
	if (err) return err;

	err = dvi_read_pages(s);
	if (err) return err;

	err = dvi_close(s, pf);
	if (err) {
		dvi_destroy(*pf);
		return err;
	}
	return 0;
}

int dvi_read_fp(dvifile_t ** pf, FILE * fp, int flags) {
	int err;
	dvifilestate_t * s;

	err = dvi_new_fp(&s, fp, 0);
	if (err) return err;

	s->flags = flags;

	err = dvi_read_postamble(s);
	if (err) return err;

	err = dvi_read_preamble(s);
	if (err) return err;

	err = dvi_read_pages(s);
	if (err) return err;

	err = dvi_close(s, pf);
	if (err) {
		dvi_destroy(*pf);
		return err;
	}
	return 0;
}

int dvi_destroy_nodes(dvinode_header_t * node) {
	while(node) {
		dvinode_header_t * next = node->next;
		switch(node->type) {
			case DVINODE_TEXT:
				free(((dvinode_text_t *)node)->content);
				break;
			case DVINODE_XXX:
				free(((dvinode_xxx_t *)node)->content);
				break;
		}
		free(node);
		node = next;
	}
	return 0;
}

int dvi_destroy(dvifile_t * f) {
	int i;

	for (i = 0; i < f->nbpages; ++i) {
		dvi_destroy_nodes(f->pages[i]);
	}

	free(f->pages);

	for (i = 0; i < f->nbfonts; ++i) {
		free(f->fonts[i].area);
		free(f->fonts[i].name);
	}

	free(f->fonts);

	free(f->comment);
	free(f);

	return 0;
}
