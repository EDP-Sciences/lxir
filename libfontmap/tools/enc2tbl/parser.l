%option debug
%option nounput
%option noyywrap
%option never-interactive

%{
#include "grammar.h"
#include <string.h>
%}

IDENT [[:alnum:]@$._-]+
SPACE [ \t\r\n]
%%

\%.*$			{}

"/"{IDENT}	{ yylval.string = strdup(yytext); return IDENT; }
"["			{ return TOK_OPEN; }
"]"			{ return TOK_CLOSE; }
"def"		{ return TOK_DEF; }
{SPACE}		{ /* return TOK_SPACE; */ }
.			{ fprintf(stderr, "invalid char \"%s\"\n", yytext); exit(-1); }
