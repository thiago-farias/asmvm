#include "asmvm.h"
#include "params.h"

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

void AsmMachine::add_labeled_instruction(const std::string& label, Instruction* instruction) {
  add_symbol(label, new IntegerValue(program_.size()));
  add_instruction(instruction);
}

void AsmMachine::reset_registers() {
  for(int i=0; i<10; ++i) {
    register_set_[i] = 0;
  }
}

int32_t AsmMachine::Run() {
  reset_registers();
  Instruction *ins = program_[reg_PC()];
  int32_t temp_PC;
  while ((temp_PC = ins->Exec(*this)) >= 0) {
    ins = program_[temp_PC];
    set_register(kRegisterIndexPc, temp_PC);
  }
  if (!call_stack_.empty()) {
    printf("A pilha de chamadas não está vazia. Cheque se há chamadas para a instrução RET" 
           " em todas as funções.\n");
  }
  return -1 - temp_PC;
}

bool AsmMachine::GetSymbolValue(const std::string& symbol, Value** out_value) {
  auto itr = symbol_table_.find(symbol);
  if (itr == symbol_table_.end()) {
    return false;
  }
  *out_value = itr->second;
  return true;
}

} // namespace asmvm

