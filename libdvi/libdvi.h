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
#ifndef LIBDVI_H
#define LIBDVI_H

#ifdef USE_KPSE
#include <libtfm.h>
#endif

/* flags */
#define DVI_SKIP_SPACE_SMALL	1
#define DVI_SKIP_SPACE_NORMAL	2
#define DVI_NO_FONT_TRANSLATION	4
#define DVI_CMTT10_NEUTRAL 8

/* Error codes */
#define DVIERR_INVALID_POP	(-2)
#define DVIERR_INVALID_FILEP	(-3)
#define DVIERR_UNEXPECTED_EOF	(-4)
#define DVIERR_INVALID_NUMBER_SIZE		(-5)
#define DVIERR_INVALID_FSEEK		(-6)
#define DVIERR_INVALID_READ			(-7)
#define DVIERR_INVALID_POSTAMBLE	(-8)
#define DVIERR_INVALID_OPCODE		(-9)
#define DVIERR_UNKNOWN_VERSION		(-10)
#define DVIERR_INVALID_FONT		(-11)
#define DVIERR_INVALID_STACK	(-12)
#define DVIERR_TOO_MANY_PAGES	(-13)
#define DVIERR_NOMEM	(-14)
#define DVIERR_INVALID_CHAR	(-15)
#define DVIERR_NOTRANSLATION_MAP	(-16)
#define DVIERR_INVALID_FLAGS (-17)

/* DVI Nodes */
#define DVINODE_TEXT 0
#define DVINODE_RULE 1
#define DVINODE_BOP 2
#define DVINODE_XXX 3
#define DVINODE_CONTROL 4

#define DVICONTROL_PUSH 0
#define DVICONTROL_POP 1
#define DVICONTROL_DOWN 2
#define DVICONTROL_RIGHT 3
#define DVICONTROL_W 4
#define DVICONTROL_X 5
#define DVICONTROL_Y 6
#define DVICONTROL_Z 7

typedef struct dvinode_header_s {
	int type;
	struct dvinode_header_s * next;
} dvinode_header_t;

typedef struct dvinode_text_s {
	dvinode_header_t header;

	int font;
	int h, v;
	int size;
	char * content;
#if USE_KPSE
	int prev_char;
	int width;
#endif
} dvinode_text_t;

typedef struct dvinode_rule_s {
	dvinode_header_t header;

	int h, v;
	int a, b;
} dvinode_rule_t;

typedef struct dvinode_bop_s {
	dvinode_header_t header;

	int counts[10];
} dvinode_bop_t;

typedef struct dvinode_xxx_s {
	dvinode_header_t header;

	int size;
	char * content;
} dvinode_xxx_t;

typedef struct dvinode_control_s {
	dvinode_header_t header;

	int type;
	int param;
} dvinode_control_t;

typedef struct dvifont_s {
	int index;
	int checksum;
	int scale;
	int design_size;
	char * area;
	char * name;
	char ** map;
	int mapsize;
#if USE_KPSE
	tfmfile_t * tfm;
#endif
} dvifont_t;

typedef struct dvifile_s {
	int version;
	int num;
	int den;
	int mag;
	char * comment;

	int nbfonts;
	dvifont_t * fonts;

	int nbpages;
	dvinode_header_t ** pages;
} dvifile_t;

int dvi_read(dvifile_t **, const char * filename, int flags);
int dvi_read_fp(dvifile_t **, FILE * fp, int flags);
int dvi_destroy(dvifile_t *);

#endif

