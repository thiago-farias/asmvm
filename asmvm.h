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

class StringValue : public Value {
 public:
  explicit StringValue(const std::string& value) : value_(value) {}
  StringValue(const StringValue& sv) : value_(sv.value_) {}
  StringValue& operator = (const StringValue& sv) {
    value_ = sv.value_;
    return *this;
  }
  ValueType type() const { return kValueTypeString; }
  std::string value() const { return value_; }
 private:
  std::string value_;
};

class Source {
 public:
  virtual ~Source() {}
  virtual int32_t value(AsmMachine& vm) const = 0;
};

class IntegerValue : public Value, public Source {
 public:
  explicit IntegerValue(int32_t value) : value_(value) {}
  IntegerValue(const IntegerValue& sv) : value_(sv.value_) {}
  IntegerValue& operator = (const IntegerValue& sv) {
    value_ = sv.value_;
    return *this;
  }
  ValueType type() const { return kValueTypeInteger; }
  int32_t value() const { return value_; }
  int32_t value(AsmMachine& vm) const { return value_; }
 private:
  int32_t value_;
};

class Base {
 public:
  virtual ~Base() {}
  virtual uint32_t base_address(AsmMachine& vm) const = 0;
};

class HexBase : public Base {
 public:
  explicit HexBase(uint32_t hex) : hex_(hex) {}
  uint32_t base_address(AsmMachine& vm) const { return hex_; }
 private:
  uint32_t hex_;
};

class Address {
 public:
  explicit Address(Base* base) : base_(base), offset_(NULL) {}
  Address(Base* base, Source* offset) : base_(base), offset_(offset) {}
  Address(const Address& address) : base_(address.base_), offset_(address.offset_) {}
  ~Address() {
    delete base_;
    delete offset_;
  }
  Address& operator = (const Address& address) {
    base_ = address.base_;
    offset_ = address.offset_;
    return *this;
  }
  Base* base() { return base_; }
  const Base* base() const { return base_; }
  Source* offset() { return offset_; }
  const Source* offset() const { return offset_; }
 private:
  Base* base_;
  Source* offset_;
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
  
  void add_labeled_instruction(const std::string& label, Instruction* instruction) {
    add_symbol(label, new IntegerValue(program_.size()));
    add_instruction(instruction);
  }
  
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

class RegisterSource : public Source {
 public:
  explicit RegisterSource(uint32_t rindex) : rindex_(rindex) {}
  int32_t value(AsmMachine& vm) const { return vm.get_register(rindex_); }
 private:
  uint32_t rindex_;
};

class RegisterBase : public Base {
 public:
  explicit RegisterBase(uint32_t rindex) : rindex_(rindex) {}
  uint32_t base_address(AsmMachine& vm) const { return uint32_t(vm.get_register(rindex_)); }
 private:
  uint32_t rindex_;
};



} // namespace asmvm

#endif
