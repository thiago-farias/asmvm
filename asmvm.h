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
  virtual ~Instruction() {}
  virtual int32_t Exec(AsmMachine& vm) = 0;
};

const uint32_t kDefaultMemorySize = 2048; // 2KB
const uint32_t kRegisterIndexPc = 9;
const uint32_t kRegisterIndexSt = 8;

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
  
  uint32_t reg_PC() { return register_set_[kRegisterIndexPc]; }
  uint32_t reg_ST() { return register_set_[kRegisterIndexPc]; }
  
  int32_t Run();
  
  SymbolTable& symbol_table() { return symbol_table_; }
  const SymbolTable& symbol_table() const { return symbol_table_; }
  bool GetSymbolValue(const std::string& symbol, Value** out_value);

  void call_push() {
    call_stack_.push_back(reg_PC());
  }

  uint32_t call_pop() {
    uint32_t last_call = call_stack_.back();
    call_stack_.pop_back();
    return last_call;
  }

  bool push_reg(uint32_t rindex) {
    if (reg_ST() + sizeof(uint32_t) >= kDefaultMemorySize) return false;
    
    int32_t* mem = reinterpret_cast<int32_t*>(data_memory_ + reg_ST());
    *mem = register_set_[rindex];
    set_register(kRegisterIndexSt, reg_ST() + sizeof(uint32_t));
    return true;
  }

  // Usefull with int32_t, int16_t, int8_t and its unsigned counterparts.
  template <typename inttype> bool push_value(inttype value, uint32_t base_address, int32_t offset) {
    if (reg_ST() + sizeof(inttype) >= kDefaultMemorySize) return false;
    
    inttype* mem = reinterpret_cast<inttype*>(data_memory_ + reg_ST() + offset);
    *mem = value;
    set_register(kRegisterIndexSt, reg_ST() + sizeof(uint32_t));
    return true;
  }

  template <typename inttype> bool push_value(inttype value) {
    return push_value(value, reg_ST(), 0);
  }

  template <typename inttype> bool load_value(uint32_t base_address, int32_t offset, inttype* out_value) {
    int32_t addr = base_address + offset;
    if (addr < 0) return false;
    if (out_value == NULL) return false;
    *out_value = *reinterpret_cast<inttype*>(data_memory_ + addr);
    return true;
  }

  bool pop(int32_t* out) {
    int32_t addr = reg_ST() - sizeof(int32_t);
    if (addr < 0) return false;
    if (out != NULL) {
      *out = *reinterpret_cast<int32_t*>(data_memory_ + addr);
    }
    set_register(kRegisterIndexSt, addr);
    return true;
  }

 private:
  inline void reset_registers();
  SymbolTable symbol_table_;
  uint8_t data_memory_[kDefaultMemorySize];
  std::vector<Instruction*> program_;
  int32_t register_set_[10]; // 8 general purpose registers + 2 specific: ST and PC.
  std::vector<uint32_t> call_stack_;
};

} // namespace asmvm

#endif
