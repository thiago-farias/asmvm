%{
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <list>
#include "op.h"
#include "asmvm.h"
#include "params.h"

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
%token MV
%token LD1
%token LD2
%token LD3
%token LD4
%token ST1
%token ST2
%token ST3
%token ST4
%token INC
%token DEC
%token CALL
%token RET
%token ST
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
%token COLON
%token ASSIGN
%token LF

%type <rindex> REGISTER
%type <int_value> L_INT
%type <int_value> L_HEX
%type <v_int_value> IntValue 
%type <str> L_STRING
%type <str> IDENTIFIER
%type <value> Value
%type <instruction> Instruction
%type <ternary> TernaryInstructions
%type <instruction> Move
%type <instruction> Pop
%type <instruction> Load
%type <instruction> Store
%type <source> Source
%type <instruction> Print
%type <base> Base
%type <address> Address

%union {
	char *str;
  uint32_t rindex;
  int32_t int_value;
  asmvm::Value* value;
  asmvm::Instruction* instruction;
  asmvm::TernaryInstruction* ternary;
  std::list<asmvm::Source*> *source_list;
  asmvm::BaseAddress* base;
  asmvm::Source* source;
  asmvm::Address* address;
  asmvm::IntegerValue* v_int_value;
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
  IDENTIFIER ASSIGN Value LF
  ;

CodeSection: 
  CODE Lines 
  ;

Lines: 
  Line
  | Line Lines 
  ;

Line: 
  Instruction LF
  | IDENTIFIER COLON Instruction LF
  ;

Value: 
  L_STRING {
    $$ = new asmvm::StringValue($1);
  }
  | IntValue {
    $$ = new asmvm::IntegerValue($1);
  }
  ;

IntValue:
  L_INT {
    $$ = $1;
  }
  | L_HEX {
    $$ = $1;
  }
  ;

Instruction: 
  TernaryInstructions {
    $$ = $1;
  }
  | NOT REGISTER REGISTER {
    $$ = new asmvm::OpNot($2, $3);
  }
  | INC REGISTER {
    $$ = new asmvm::OpInc($2);
  }
  | DEC REGISTER {
    $$ = new asmvm::OpDec($2);
  }
  | JMP IDENTIFIER {
    $$ = new asmvm::OpJmp($2);
  }
  | CALL IDENTIFIER {
    $$ = new asmvm::OpCall($2);
  }
  | RET {
    $$ = new asmvm::OpRet();
  }
  | JZ REGISTER IDENTIFIER {
    $$ = new asmvm::OpJz($2, $3);
  }
  | JNZ REGISTER IDENTIFIER {
    $$ = new asmvm::OpJnz($2, $3);
  }
  | Move {
    $$ = $1;
  }
  | PUSH Source {
    $$ = new OpPush($2);
  }
  | Pop {
    $$ = $1;
  }
  | Load {
    $$ = $1;
  }
  | Store {
    $$ = $1;
  }
  | EXIT Source {
    $$ = asmvm::OpExit($2);
  }
  | Print {
    $$ = $1;
  }
  ;
Move:
  MV REGISTER REGISTER {
    $$ = new asmvm::OpMov($2, new asmvm::RegisterSource($3));
  }
  | MV REGISTER L_BRACKET IntValue R_BRACKET {
    $$ = new asmvm::OpMov($2, new asmvm::IntValue($4));
  }
  ;
Pop:
  POP {
    $$ = new asmvm::OpPop();
  }
  | POP REGISTER {
    $$ = new asmvm::OpPop($2);
  }
  ;
Print:
  PRINT L_STRING
  | PRINT L_STRING SourceList
  ;
SourceList:
  Source
  | Source SourceList
  ;
  
TernaryInstructions:
  ADD Source Source REGISTER {
    $$ = new asmvm::OpAdd($2, $3, $4);
  }
  | SUB Source Source REGISTER {
    $$ = new asmvm::OpSub($2, $3, $4);
  }
  | MUL Source Source REGISTER {
    $$ = new asmvm::OpMul($2, $3, $4);
  }
  | DIV Source Source REGISTER {
    $$ = new asmvm::OpDiv($2, $3, $4);
  }
  | MOD Source Source REGISTER {
    $$ = new asmvm::OpMod($2, $3, $4);
  }
  | AND Source Source REGISTER {
    $$ = new asmvm::OpAnd($2, $3, $4);
  }
  | OR Source Source REGISTER {
    $$ = new asmvm::OpOr($2, $3, $4);
  }
  | XOR Source Source REGISTER {
    $$ = new asmvm::OpXor($2, $3, $4);
  }
  | SHR Source Source REGISTER {
    $$ = new asmvm::OpShr($2, $3, $4);
  }
  | SHL Source Source REGISTER {
    $$ = new asmvm::OpShl($2, $3, $4);
  }
  ;
Load:
  | LD1 REGISTER Address {
    $$ = new asmvm::OpLd1($2, $3);
  }
  | LD2 REGISTER Address {
    $$ = new asmvm::OpLd2($2, $3);
  }
  | LD4 REGISTER Address {
    $$ = new asmvm::OpLd4($2, $3);
  }
  ;
Source:
  REGISTER {
    $$ = new asmvm::RegisterSource($1);
  }
  | IntValue {
    $$ = new asmvm::IntegerValue($1);
  }
  ;
Address:
  Base {
    $$ = new asmvm::Address($1, NULL);
  }
  | Base L_BRACKET Source R_BRACKET {
    $$ = new asmvm::Address($1, $3);
  }
  ;
Base:
  REGISTER {
    $$ = new asmvm::BaseAddressRegister($1);
  }
  | L_HEX {
    $$ = new asmvm::BaseAddressHex($1);
  }
  ;

Store:
  ST Source Address
  | ST1 Source Address
  | ST2 Source Address
  | ST3 Source Address
  | ST4 Source Address
  ;