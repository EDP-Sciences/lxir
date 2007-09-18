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
#ifndef LIBFONTMAP_H
#define LIBFONTMAP_H 1

struct translation_info {
	char ** map;
	int size;
};

int lfm_init();
int lfm_close();

/*
int lfm_create_context(const char *, struct fontmap_s ** ctxt);
int lfm_convert(struct fontmap_s * ctxt, char input, char ** poutput, int * pout);
int lfm_destroy_context(struct fontmap_s * ctxt);
*/

int lfm_get_math_encoding_map(const char *, struct translation_info *);
int lfm_get_translation_map(const char *, struct translation_info *);
int lfm_get_accent(const char *, const char *, char *);


#endif
