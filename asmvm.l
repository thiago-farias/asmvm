%{
#include <string.h>
#include <sstream>
#include <list>
#include "op.h"
#include "asmvm.h"
#include "params.h"

#include "parser.hpp"

extern int yyerror(const char *msg);

int lineNumber = 0;

std::string literal_value;

int32_t hex2int(const char* hex) {   
    uint32_t x;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> x;
    return (int32_t)x;
}

%}

%option noyywrap
%x COMENTARIO
%x STRING

%%
".DATA" { return STATIC; }
".CODE" { return CODE; }
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
"NOT" { return NOT; }
"INC" { return INC; }
"DEC" { return DEC; }
"JMP" { return JMP;}
"JZ" { return JZ;}
"JNZ" { return JNZ;}
"CALL" { return CALL; }
"RET" { return RET; }
"PUSH" { return PUSH; }
"POP" { return POP; }
"MV" { return MV; }
"LD1" { return LD1; }
"LD2" { return LD2; }
"LD4" { return LD4; }
"ST1" { return ST1; }
"ST2" { return ST2; }
"ST4" { return ST4; }
"PRINT" { return PRINT; }
"EXIT" { return EXIT; }
"SYSCALL" { return SYSCALL; }
"[" { return L_BRACKET; }
"]" { return R_BRACKET; }
"=" { return ASSIGN; }
":" { return COLON; }
";" { BEGIN(COMENTARIO); }
<COMENTARIO>\n { BEGIN(INITIAL); lineNumber++; }
<COMENTARIO>. {}

ST|PC|R[1-8] { 
    if (!strcmp(yytext, "ST")) {
        yylval.rindex = 9;
    } else if (!strcmp(yytext, "PC")) {
        yylval.rindex = 10;
    } else {
        yylval.rindex = yytext[1] - '0';
    }
    --yylval.rindex;
    return REGISTER;
}
0|[+-]?[1-9][0-9]* { yylval.int_value = atoi(yytext); return L_INT; }
0x[0-9A-F]+ { yylval.int_value = hex2int(yytext+2); return L_HEX; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }
[\r\t ] {}
\n { lineNumber++; }
"\"" { BEGIN(STRING); literal_value = ""; }
<STRING>"\"" { BEGIN(INITIAL); yylval.str = strdup(literal_value.c_str()); return L_STRING; }
<STRING>\\t { literal_value += '\t'; }
<STRING>\\r { literal_value += '\r'; }
<STRING>\\n { literal_value += '\n'; lineNumber++; }
<STRING>. { literal_value += yytext; }
. { yyerror("Invalid character."); }

%%
