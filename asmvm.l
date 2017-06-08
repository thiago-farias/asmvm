%{
#include <string.h>
#include <sstream>
#include <list>
#include "asmvm.h"
#include "params.h"
#include "asmvm_parser.hpp"


int lineNumber = 0;

int hex2int(const char* hex) {   
    int x;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> x;
    return x;
}

%}

%option noyywrap nodefault
%x COMENTARIO

%%
"ADD" { return ADD;}
"SUB" { return SUB;}
"MUL" { return MUL;}
"DIV" { return DIV;}
"MOD" { return MOD;} 
"SHL" { return SHL;}
"SHR" { return SHR;}
"AND" { return AND;}
"OR" { return OR;}
"XOR" { return XOR;}
"JMP" { return JMP;}
"JZ" { return JZ;}
"JNZ" { return JNZ;}
"[" {return L_BRACKET;}
"]" {return R_BRACKET;}
":" {return COLON;}
"\n" { return LF; }
R[1-8] { yylval.rindex = '0' + yytext[1]; return REGISTER; }
0|[+-]?[1-9][0-9]* {yylval.int_value = atoi(yytext); return L_INT; }
0x[0-9A-F]+ {yylval.int_value = hex2int(yytext); return L_HEX; }


";" {BEGIN(COMENTARIO);}

<COMENTARIO>\n {BEGIN(INITIAL);}

<COMENTARIO>. {}

. {}

%%