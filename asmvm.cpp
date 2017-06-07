#include "asmvm.h"

extern int yyparse();

namespace asmvm {

AsmMachine::AsmMachine() {
  reset_registers();
}

AsmMachine::~AsmMachine() {
  for (SymbolTable::iterator i = symbol_table_.begin(); i != symbol_table_.end(); ++i) {
    delete i->second;
  }
  for (int i=0; i < program_.size(); ++i) {
    delete program_[i];
  }
}

void AsmMachine::reset_registers() {
  reg_PC_ = 0;
  reg_ST_ = 0;
  for(int i=0; i<8; ++i) {
    register_set_[i] = 0;
  }
}

void AsmMachine::Run() {
  reset_registers();
  Instruction *ins = program_[reg_PC_];
  while ((reg_PC_ = ins->Exec(*this)) >= 0) {
    ins = program_[reg_PC_];
  }
  if (!call_stack_.empty()) {
    printf("A pilha de chamadas não está vazia. Cheque se há chamadas para a instrução RET" 
           " em todas as funções.\n");
  }
}

} // namespace asmvm

int main(int argc, char **argv) {
	extern FILE *yyin;
	if(argc != 2) {
		printf("Uso: %s arquivo_de_entrada\n", argv[0]);
		return 1;
	}
	
	if ((yyin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Erro ao tentar abrir o arquivo %s!\n", argv[1]);
		return 1;
	}
	
	if (yyparse()) {
		fprintf(stderr, "Não foi possível compilar %s!\n", argv[1]);
		return 1;
	}
  return 0;
}
