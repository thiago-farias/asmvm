#ifndef ASMVM_OP_H
#define ASMVM_OP_H

#include <list>

#include "params.h"

namespace asmvm {

class TernaryInstruction : public Instruction {
 public:
  TernaryInstruction(Source* param1, Source* param2, uint32_t output_rindex)
    : param1_(param1), param2_(param2), output_rindex_(output_rindex) {}
  virtual ~TernaryInstruction() {
    delete param1_;
    delete param2_;
  }
  Source* param1() { return param1_; }
  Source* param2() { return param2_; }
  const Source* param1() const { return param1_; }
  const Source* param2() const { return param2_; }
  uint32_t output_rindex() const { return output_rindex_; }
 private:
  Source* param1_;
  Source* param2_;
  uint32_t output_rindex_;
};

class OpAdd : public TernaryInstruction {
 public:
  OpAdd(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpSub : public TernaryInstruction {
 public:
  OpSub(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpMul : public TernaryInstruction {
 public:
  OpMul(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpDiv : public TernaryInstruction {
 public:
  OpDiv(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpMod : public TernaryInstruction {
 public:
  OpMod(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpAnd : public TernaryInstruction {
 public:
  OpAnd(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpOr : public TernaryInstruction {
 public:
  OpOr(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpXor : public TernaryInstruction {
 public:
  OpXor(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpShl : public TernaryInstruction {
 public:
  OpShl(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpShr : public TernaryInstruction {
 public:
  OpShr(Source* param1, Source* param2, uint32_t output_rindex) 
    : TernaryInstruction(param1, param2, output_rindex) {}
  int32_t Exec(AsmMachine& vm);
};

class OpNot : public Instruction {
 public:
  OpNot(uint32_t rindex1, uint32_t rindex2) : rindex1_(rindex1), rindex2_(rindex2) {}
  int32_t Exec(AsmMachine& vm);
 private:
  uint32_t rindex1_;
  uint32_t rindex2_;
};

class OpJmp : public Instruction {
 public:
  OpJmp(const std::string& label) : label_(label) {}
  int32_t Exec(AsmMachine& vm);
 private:
  std::string label_;
};

class OpCall : public Instruction {
 public:
  OpCall(const std::string& label) : label_(label) {}
  int32_t Exec(AsmMachine& vm);
 private:
  std::string label_;
};

class OpRet : public Instruction {
 public:
  OpRet() {}
  int32_t Exec(AsmMachine& vm);
};

class ConditionalJump : public Instruction {
 public:
  ConditionalJump(uint32_t rindex, const std::string& label) : rindex_(rindex), label_(label) {}
  int32_t Exec(AsmMachine& vm);
  virtual bool jmp_condition(AsmMachine& vm) = 0;
 protected:
  uint32_t rindex_;
  std::string label_;
};

class OpJz : public ConditionalJump {
 public:
  OpJz(uint32_t rindex, const std::string& label) : ConditionalJump(rindex, label) {}
  bool jmp_condition(AsmMachine& vm) { return vm.get_register(rindex_) == 0; }
};

class OpJnz : public ConditionalJump {
 public:
  OpJnz(uint32_t rindex, const std::string& label) : ConditionalJump(rindex, label) {}
  bool jmp_condition(AsmMachine& vm) { return vm.get_register(rindex_) != 0; }
};

class OpMov : public Instruction {
 public:
  OpMov(uint32_t rindex_dst, Source* src) : rindex_dst_(rindex_dst), src_(src) {}
  ~OpMov() {
    delete src_;
  }
  int32_t Exec(AsmMachine& vm);
 private:
  uint32_t rindex_dst_;
  Source* src_;
};

class OpPush : public Instruction {
 public:
  OpPush(Source* src) : src_(src) {}
  ~OpPush() {
    delete src_;
  }
  int32_t Exec(AsmMachine& vm);
 private:
  Source* src_;
};

class OpPop : public Instruction {
 public:
  OpPop() : rindex_(0), store_value_(false) {}
  OpPop(uint32_t rindex) : rindex_(rindex), store_value_(true) {}
  int32_t Exec(AsmMachine& vm);
 private:
  uint32_t rindex_;
  bool store_value_;
};

class OpLoad : public Instruction {
 public:
  OpLoad(uint32_t rindex, Address* address) : rindex_(rindex), address_(address) {}
  virtual ~OpLoad() {
    delete address_;
  }
  
 protected:
  uint32_t rindex_;
  Address* address_;
};

class OpLd1 : public OpLoad {
 public:
  OpLd1(uint32_t rindex, Address* address) : OpLoad(rindex, address) {}
  int32_t Exec(AsmMachine& vm);
};

class OpLd2 : public OpLoad {
 public:
  OpLd2(uint32_t rindex, Address* address) : OpLoad(rindex, address) {}
  int32_t Exec(AsmMachine& vm);
};

class OpLd4 : public OpLoad {
 public:
  OpLd4(uint32_t rindex, Address* address) : OpLoad(rindex, address) {}
  int32_t Exec(AsmMachine& vm);
};

class OpExit : public Instruction {
 public:
  OpExit(Source* code) : code_(code) {}
  ~OpExit() {
    delete code_;
  }
  int32_t Exec(AsmMachine& vm);
 private:
  Source* code_;
};

class OpInc : public Instruction {
 public:
  OpInc(uint32_t rindex) : rindex_(rindex) {}
  int32_t Exec(AsmMachine& vm);
 private:
  uint32_t rindex_;
};

class OpDec : public Instruction {
 public:
  OpDec(uint32_t rindex) : rindex_(rindex) {}
  int32_t Exec(AsmMachine& vm);
 private:
  uint32_t rindex_;
};

class OpPrint : public Instruction {
 public:
  OpPrint(const std::list<Printable*>& printables) : printables_(printables) {}
  int32_t Exec(AsmMachine& vm);
 private:
  std::list<Printable*> printables_;
};

class OpStore : public Instruction {
 public:
  OpStore(Source* src, Address* address) : src_(src), address_(address) {}
  virtual ~OpStore() {
    delete src_;
    delete address_;
  }
 protected:
  Source* src_;
  Address* address_;
};

class OpSt1 : public OpStore {
 public:
  OpSt1(Source* src, Address* address) : OpStore(src, address) {}
  int32_t Exec(AsmMachine& vm);
};

class OpSt2 : public OpStore {
 public:
  OpSt2(Source* src, Address* address) : OpStore(src, address) {}
  int32_t Exec(AsmMachine& vm);
};

class OpSt4 : public OpStore {
 public:
  OpSt4(Source* src, Address* address) : OpStore(src, address) {}
  int32_t Exec(AsmMachine& vm);
};

class OpSysCall : public Instruction {
 public:
  OpSysCall(Source* src, uint32_t rindex) : src_(src), rindex_(rindex) {}
  int32_t Exec(AsmMachine& vm);
 private:
  Source* src_;
  uint32_t rindex_;
};

} // namespace asmvm

#endif
