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
"LD3" { return LD3; }
"LD4" { return LD4; }
"ST1" { return ST1; }
"ST2" { return ST2; }
"ST3" { return ST3; }
"ST4" { return ST4; }
"PRINT" { return PRINT; }
"EXIT" { return EXIT; }
"[" {return L_BRACKET;}
"]" {return R_BRACKET;}
"=" { return ASSIGN; }
":" {return COLON;}
"\n" { return LF; }
\"[^\"]*\" { yytext[strlen(yytext)-1] = '\0'; yylval.str = strdup(yytext+1); return L_STRING; }
ST|PC|R[1-8] { 
    if (!strcmp(yytext, "ST")) {
        yylval.rindex = 9;
    } else if (!strcmp(yytext, "PC")) {
        yylval.rindex = 10;
    } else {
        yylval.rindex = '0' + yytext[1]; 
    }   
    return REGISTER; 
}
0|[+-]?[1-9][0-9]* { yylval.int_value = atoi(yytext); return L_INT; }
0x[0-9A-F]+ { yylval.int_value = hex2int(yytext+2); return L_HEX; }
[a-z_][a-z0-9_]* { yylval.str = strdup(yytext); return IDENTIFIER; }



";" {BEGIN(COMENTARIO);}

<COMENTARIO>\n {BEGIN(INITIAL);}

<COMENTARIO>. {}

. {}

%%