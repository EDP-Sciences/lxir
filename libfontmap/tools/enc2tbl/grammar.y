%debug

%{
#include "grammar.h"
%}

%union {
	char * string;
	struct ident_list * list;
}

%token<string> IDENT
%token TOK_OPEN TOK_CLOSE TOK_DEF

%type<list> idents

%%

document:
	IDENT TOK_OPEN idents close { register_enc($1, $3); }
	;

close:
	  TOK_CLOSE
	| TOK_CLOSE TOK_DEF
	;

idents:
	/* empty */						{ $$ = 0; }
	| idents IDENT					{ $$ = make_ident_list($1, $2); }
	;

