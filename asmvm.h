#ifndef ASMVM_H
#define ASMVM_H

#include <map>
#include <vector>
#include <string>
#include <stdint.h>

namespace asmvm {

class AsmMachine;

class Value {
 public:
  enum ValueType {
    kValueTypeString,
    kValueTypeInteger
  };
  virtual ~Value() {}
  virtual ValueType type() const = 0;
};

class Instruction {
 public:
  enum OpCode {
    kInstructionNop = 0,
    kInstructionAdd,
    kInstructionSub,
    kInstructionMul,
    kInstructionDiv,
    kInstructionMod,
    kInstructionAnd,
    kInstructionOr,
    kInstructionXor,
    kInstructionShr,
    kInstructionShl,
    kInstructionNot,
    kInstructionJmp,
    kInstructionJz,
    kInstructionJnz,
    kInstructionLd,
    kInstructionLd1,
    kInstructionLd2,
    kInstructionLd3,
    kInstructionLd4,
    kInstructionSt,
    kInstructionSt1,
    kInstructionSt2,
    kInstructionSt3,
    kInstructionSt4,
    kInstructionInc,
    kInstructionDec,
    kInstructionCall,
    kInstructionRet,
    kInstructionPush,
    kInstructionPop,
    kInstructionPrint,
    kInstructionExit
  };
  OpCode opcode() { return opcode_; }
  virtual ~Instruction() {}
  virtual int32_t Exec(AsmMachine& vm) = 0;
 protected:
  OpCode opcode_;
};

static const uint32_t kDefaultMemorySize = 2048; // 2KB

class AsmMachine {
 public:
  typedef std::map<std::string, Value*> SymbolTable;
  
  AsmMachine();
  ~AsmMachine();
    
  void add_symbol(const std::string& name, Value* value) {
    symbol_table_.insert(SymbolTable::value_type(name, value));
  }
  
  void add_instruction(Instruction* instruction) {
    program_.push_back(instruction);
  }
  
  void add_labeled_instruction(const std::string& label, Instruction* instruction);
  
  int32_t get_register(uint32_t rindex) const {
    return register_set_[rindex];
  }
  
  void set_register(uint32_t rindex, int32_t value) {
    register_set_[rindex] = value;
  }
  
  uint32_t reg_PC() { return reg_PC_; }
  uint32_t reg_ST() { return reg_ST_; }
  
  void Run();
  
  SymbolTable& symbol_table() { return symbol_table_; }
  const SymbolTable& symbol_table() const { return symbol_table_; }
  bool GetSymbolValue(const std::string& symbol, Value** out_value);

  void call_push() {
    call_stack_.push_back(reg_PC_);
  }

  uint32_t call_pop() {
    uint32_t last_call = call_stack_.back();
    call_stack_.pop_back();
    return last_call;
  }

 private:
  inline void reset_registers();
  SymbolTable symbol_table_;
  uint8_t data_memory_[kDefaultMemorySize];
  std::vector<Instruction*> program_;
  int32_t register_set_[8];
  // These special purpose registers are unsigned.
  uint32_t reg_PC_;
  uint32_t reg_ST_;
  std::vector<uint32_t> call_stack_;
};

} // namespace asmvm

#endif
