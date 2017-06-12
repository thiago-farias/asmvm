#ifndef ASMVM_OP_H
#define ASMVM_OP_H

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

} // namespace asmvm

#endif
