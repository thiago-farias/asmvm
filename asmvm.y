%{
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "vm.h"
extern int yylex();

extern int lineNumber;
int yyerror(const char *msg)
	{
		fprintf(stderr, "Linha %d: %s\n", lineNumber+1, msg);	
		return 1;
		
	}

%}
%token ADD
%token SUB
%token MUL
%token DIV
%token MOD
%token AND
%token OR
%token XOR
%token SHR
%token SHL
%token NOT
%token JMP
%token JZ
%token JNZ
%token LD
%token LD1
%token LD2
%token LD3
%token LD4
%token ST
%token ST1
%token ST2
%token ST3
%token ST4
%token INC
%token DEC
%token CALL
%token RET
%token PC
%token PUSH
%token POP
%token PRINT
%token EXIT
%token REGISTER
%token L_INT
%token L_HEX
%token L_STRING
%token IDENTIFIER
%token STATIC
%token CODE
%token L_BRACKET
%token R_BRACKET
%token COMMA
%token ASSIGN
%token SEMICOLON

%type <rindex> REGISTER
%type <int_value> L_INT
%type <int_value> L_HEX
%type <str> L_STRING
%type <str> IDENTIFIER

%union {
	char *str;
  uint32_t rindex;
  int32_t int_value;
}
%%

File: 
  DataSection CodeSection 
  ;

DataSection: 
  STATIC
  | STATIC Assignments
  ;
  

Assignments: 
  Assignment
  | Assignment Assignments 
  ;

Assignment: 
  IDENTIFIER ASSIGN Value SEMICOLON
  ;

CodeSection: 
  CODE Lines 
  ;

Lines: 
  Line
  | Line Lines 
  ;

Line: 
  Instruction SEMICOLON
  | IDENTIFIER COMMA Instruction SEMICOLON
  ;

Value: 
  L_STRING
  | IntValue 
  ;

IntValue:
  L_INT
  | L_HEX 
  ;

Instruction: 
  TernaryRegInstructions
  | NOT REGISTER REGISTER
  | INC REGISTER
  | DEC REGISTER
  | JMP IDENTIFIER
  | CALL IDENTIFIER
  | RET
  | JZ REGISTER IDENTIFIER
  | JNZ REGISTER IDENTIFIER
  | PUSH Source
  | Pop
  | Load
  | Store
  | EXIT Source
  | Print
  ;
Pop:
  POP
  | POP REGISTER
  ;
Print:
  PRINT L_STRING
  | PRINT L_STRING SourceList
  ;
SourceList:
  Source
  | Source SourceList
  ;
  
TernaryRegInstructions:
  ADD Source Source REGISTER
  | SUB Source Source REGISTER
  | MUL Source Source REGISTER
  | DIV Source Source REGISTER
  | MOD Source Source REGISTER
  | AND Source Source REGISTER
  | OR Source Source REGISTER
  | XOR Source Source REGISTER
  | SHR Source Source REGISTER
  | SHL Source Source REGISTER
  ;
Load:
  LD REGISTER Address
  | LD REGISTER L_BRACKET Source R_BRACKET
  | LD1 REGISTER Address
  | LD2 REGISTER Address
  | LD3 REGISTER Address
  | LD4 REGISTER Address
  ;
Source:
  REGISTER
  | IntValue
  ;
Address:
  Base
  | Base L_BRACKET Source R_BRACKET
  ;
Base:
  REGISTER
  | L_HEX
  ;

Store:
  ST Source Address
  | ST1 Source Address
  | ST2 Source Address
  | ST3 Source Address
  | ST4 Source Address
  ;