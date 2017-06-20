#include "asmvm.h"

#include <string.h>

#include "params.h"


extern int yyparse();

namespace asmvm {

AsmMachine::AsmMachine() : static_data_end_addr_(0) {
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

void AsmMachine::AddSymbol(const std::string& name, Value* value) {
    if (value->kind() == Value::kValueKindVar) {
      int32_t addr = -1;
      if (value->type() == Value::kValueTypeInteger) {
        int32_t* mem = reinterpret_cast<int32_t*>(data_memory_ + static_data_end_addr_);
        IntegerValue* int_value = static_cast<IntegerValue*>(value);
        *mem = int_value->value();
        addr = static_data_end_addr_;
        static_data_end_addr_ += sizeof(int32_t);
      } else { // value->type() == Value::kValuTypeString
        char* mem = reinterpret_cast<char*>(data_memory_ + static_data_end_addr_);
        StringValue* str_value = static_cast<StringValue*>(value);
        strcpy(mem, str_value->value().c_str());
        addr = static_data_end_addr_; 
        static_data_end_addr_ += str_value->value().length() + 1;
      }
      symbol_table_.insert(SymbolTable::value_type(name, new IntegerValue(Value::kValueKindVar, addr)));
    } else {
      symbol_table_.insert(SymbolTable::value_type(name, value));
    }
  }

void AsmMachine::add_labeled_instruction(const std::string& label, Instruction* instruction) {
  AddSymbol(label, new IntegerValue(Value::kValueKindLabel, program_.size()));
  add_instruction(instruction);
}

void AsmMachine::reset_registers() {
  for(int i=0; i<10; ++i) {
    register_set_[i] = 0;
  }
  register_set_[kRegisterIndexSt] = static_data_end_addr_;
}

int32_t AsmMachine::Run() {
  reset_registers();
  Instruction *ins = program_[reg_PC()];
  int32_t temp_PC;
  //log_regs();
  while ((temp_PC = ins->Exec(*this)) >= 0) {
    ins = program_[temp_PC];
    set_register(kRegisterIndexPc, temp_PC);
    //log_regs();
    //getchar();
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

