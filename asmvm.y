%{
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <list>
#include "op.h"
#include "asmvm.h"
#include "params.h"
#include "parser_aid.h"

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
%token LD4
%token ST1
%token ST2
%token ST4
%token INC
%token DEC
%token CALL
%token RET
%token PUSH
%token POP
%token PRINT
%token EXIT
%token SYSCALL
%token PUSHN
%token POPN
%token FPRINT
%token SPRINT
%token REGISTER
%token L_INT
%token L_HEX
%token L_STRING
%token IDENTIFIER
%token LABEL
%token STATIC
%token CODE
%token L_BRACKET
%token R_BRACKET
%token COLON
%token ASSIGN

%type <rindex> REGISTER
%type <int_value> L_INT
%type <int_value> L_HEX
%type <int_value> IntValue 
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
%type <print_arg_list> PrintArgList
%type <printable> PrintArg
%type <str> LABEL

%union {
	char *str;
  uint32_t rindex;
  int32_t int_value;
  asmvm::Value* value;
  asmvm::Instruction* instruction;
  asmvm::TernaryInstruction* ternary;
  asmvm::BaseAddress* base;
  asmvm::Source* source;
  asmvm::Address* address;
  std::list<asmvm::Printable*> *print_arg_list;
  asmvm::Printable* printable;
}
%%

File: 
  DataSection CodeSection 
  ;

DataSection: 
  | STATIC
  | STATIC Assignments
  ;

Assignments: 
  Assignment
  | Assignment Assignments 
  ;

Assignment: 
  IDENTIFIER ASSIGN Value {
    asmvm::parser::StaticHolder::instance().vm().AddSymbol($1, $3);
  }
  ;

CodeSection:
  CODE Lines
  ;

Lines: 
  Line
  | Line Lines 
  ;

Line: 
  Instruction {
    asmvm::parser::StaticHolder::instance().vm().add_instruction($1);
  }
  | LABEL Instruction {
    asmvm::parser::StaticHolder::instance().vm().add_labeled_instruction($1, $2);
  }
  ;

Value: 
  L_STRING {
    $$ = new asmvm::StringValue(asmvm::Value::kValueKindVar, $1);
  }
  | IntValue {
    $$ = new asmvm::IntegerValue(asmvm::Value::kValueKindVar, $1);
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
    $$ = new asmvm::OpPush($2);
  }
  | PUSH IDENTIFIER {
    asmvm::Value* v = NULL;
    asmvm::parser::StaticHolder::instance().vm().GetSymbolValue($2, &v);
    asmvm::IntegerValue* iv = static_cast<asmvm::IntegerValue*>(v);
    $$ = new asmvm::OpPush(new asmvm::IntegerValue(*iv));
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
    $$ = new asmvm::OpExit($2);
  }
  | Print {
    $$ = $1;
  }
  | FPRINT REGISTER {
    $$ = new asmvm::OpFprint($2);
  }
  | SPRINT REGISTER {
    $$ = new asmvm::OpSprint($2);
  }
  | SPRINT IDENTIFIER {
    asmvm::Value* v = NULL;
    asmvm::AsmMachine& vm = asmvm::parser::StaticHolder::instance().vm();
    vm.GetSymbolValue($2, &v);
    asmvm::IntegerValue* iv = static_cast<asmvm::IntegerValue*>(v);
    const char* str = reinterpret_cast<const char*>(vm.data() + iv->value(vm));
    $$ = new asmvm::OpSprint(str);
  }
  | SYSCALL Source REGISTER {
    $$ = new asmvm::OpSysCall($2, $3);
  }
  | PUSHN Source {
    $$ = new asmvm::OpPushN($2->value(asmvm::parser::StaticHolder::instance().vm()));
  }
  | POPN Source {
    $$ = new asmvm::OpPopN($2->value(asmvm::parser::StaticHolder::instance().vm()));
  }
  ;
Move:
  MV REGISTER Source {
    $$ = new asmvm::OpMov($2, $3);
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
  PRINT PrintArgList {
    $$ = new asmvm::OpPrint(asmvm::parser::StaticHolder::instance().print_arg_list());
    asmvm::parser::StaticHolder::instance().clear();
  }
  ;
PrintArgList:
  PrintArg {
    // Does not assign to $$. Recursive lists are not friends of unions. 
    asmvm::parser::StaticHolder::instance().add($1);
  }
  | PrintArg { asmvm::parser::StaticHolder::instance().add($1); } PrintArgList {
    // Does not assign to $$. Recursive lists are not friends of unions.     
  }
  ;
PrintArg:
  L_STRING {
    $$ = new asmvm::StringValue(asmvm::Value::kValueKindConst, $1);
  }
  | Source {
    $$ = $1;
  }
  | IDENTIFIER {
    asmvm::Value* v = NULL;
    asmvm::parser::StaticHolder::instance().vm().GetSymbolValue($1, &v);
    asmvm::IntegerValue* iv = static_cast<asmvm::IntegerValue*>(v);
    $$ = new asmvm::StringValue(asmvm::Value::kValueKindConst, iv->value());
  }
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
  LD1 REGISTER Address {
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
    $$ = new asmvm::IntegerValue(asmvm::Value::kValueKindConst, $1);
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
  | IDENTIFIER {
    $$ = new asmvm::BaseAddressVar($1);
  }
  ;

Store:
  ST1 Source Address {
    $$ = new asmvm::OpSt1($2, $3);
  }
  | ST2 Source Address {
    $$ = new asmvm::OpSt2($2, $3);
  }
  | ST4 Source Address {
    $$ = new asmvm::OpSt4($2, $3);
  }
  ;