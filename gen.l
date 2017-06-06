%{
#include <string.h>
#include "vm.h"
#include "vm_parser.hpp"


int lineNumber = 0;

%}

%option noyywrap nodefault
%x COMENTARIO

%%
"text" { yylval.type = gen::TEXT; return TEXT;}
"date" { yylval.type = gen::DATE; return DATE;}
"time" {yylval.type = gen::TIME; return TIME;}
"file" {yylval.type = gen::FILE; return FILE_;}
"long" {yylval.type = gen::LONG; return LONG;} 
"integer" {yylval.type = gen::INT; return INT;}
"real" {yylval.type = gen::REAL;return REAL;}
"boolean" {yylval.type = gen::BOOL;return BOOL;}

"project" {return PROJECT;}
"entity" { return ENTITY;}
"{" {return LBRACE;}
"}" {return RBRACE;}
"pk" {return PK;}
"unique" {return UNIQUE;}
";" {return SEMICOLON;}
"weak" {return WEAK;}
"relationship" {return RELATIONSHIP;}
"bilateral" {return BILATERAL;}
"(" {return LPAR;}
")" {return RPAR;}
"from" {return FROM;}
"to" {return TO;}
".." {return UPTO;}
[1N] { yylval.chr = yytext[0]; return ARITY; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }
"\n" {	lineNumber++; }


"/*" {BEGIN(COMENTARIO);}

<COMENTARIO>"*/" {BEGIN(INITIAL);}

<COMENTARIO>\n { lineNumber++;}

<COMENTARIO>. {}

. {}

%%