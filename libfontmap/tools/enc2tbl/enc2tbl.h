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
#ifndef ENC2TBL_H
#define ENC2TBL_H 1

extern int yyparse();
extern int yydebug;
extern int yy_flex_debug;
extern int yylex();
extern void yyerror(const char *);

struct ident_list {
	struct ident_list * next;
	struct ident_list * last;
	const char * name;
};

extern void register_enc(const char * name, struct ident_list * list);
extern struct ident_list * make_ident_list(struct ident_list * list, const char * name);

#endif
