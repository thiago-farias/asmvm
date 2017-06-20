#ifndef ASMVM_PARAMS_H
#define ASMVM_PARAMS_H

#include "asmvm.h"

#include <string>
#include <sstream>

namespace asmvm {

class Printable {
 public:
  virtual ~Printable() {}
  virtual std::string str(AsmMachine& vm) const = 0;
};

class Source : public Printable {
 public:
  virtual ~Source() {}
  virtual int32_t value(AsmMachine& vm) const = 0;
  
  std::string str(AsmMachine& vm) const {
    std::stringstream ss; 
    ss << value(vm);
    return ss.str();
  }
};

class BaseAddress {
 public:
  virtual ~BaseAddress() {}
  virtual uint32_t base_address(AsmMachine& vm) const = 0;
};

class Address {
 public:
  explicit Address(BaseAddress* base) : base_(base), offset_(NULL) {}
  Address(BaseAddress* base, Source* offset) : base_(base), offset_(offset) {}
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
  BaseAddress* base() { return base_; }
  const BaseAddress* base() const { return base_; }
  Source* offset() { return offset_; }
  const Source* offset() const { return offset_; }
  uint32_t address(AsmMachine& vm) const { return base_->base_address(vm) + ((offset_ == NULL)?0:offset_->value(vm)); }
 private:
  BaseAddress* base_;
  Source* offset_;
};

class StringValue : public Value, public Printable {
 public:
  StringValue(ValueKind kind) : Value(kind) {}
  explicit StringValue(ValueKind kind, const std::string& value) : Value(kind), value_(value) {}
  StringValue(const StringValue& sv) : Value(sv.kind()), value_(sv.value_) {}
  StringValue& operator = (const StringValue& sv) {
    value_ = sv.value_;
    return *this;
  }
  ValueType type() const { return kValueTypeString; }
  std::string value() const { return value_; }
  std::string str(AsmMachine& vm) const {
    return value_;
  }
 private:
  std::string value_;
};

class IntegerValue : public Value, public Source {
 public:
  IntegerValue(ValueKind kind) : Value(kind), value_(0) {}
  explicit IntegerValue(ValueKind kind, int32_t value) : Value(kind), value_(value) {}
  IntegerValue(const IntegerValue& sv) : Value(sv.kind()), value_(sv.value_) {}
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

class RegisterSource : public Source {
 public:
  explicit RegisterSource(uint32_t rindex) : rindex_(rindex) {}
  int32_t value(AsmMachine& vm) const { return vm.get_register(rindex_); }
 private:
  uint32_t rindex_;
};

class BaseAddressRegister : public BaseAddress {
 public:
  explicit BaseAddressRegister(uint32_t rindex) : rindex_(rindex) {}
  uint32_t base_address(AsmMachine& vm) const { return uint32_t(vm.get_register(rindex_)); }
 private:
  uint32_t rindex_;
};

class BaseAddressHex : public BaseAddress {
 public:
  explicit BaseAddressHex(uint32_t hex) : hex_(hex) {}
  uint32_t base_address(AsmMachine& vm) const { return hex_; }
 private:
  uint32_t hex_;
};

class BaseAddressVar : public BaseAddress {
 public:
  explicit BaseAddressVar(const std::string& symbol) : symbol_(symbol) {}
  uint32_t base_address(AsmMachine& vm) const { 
    Value* v;
    vm.GetSymbolValue(symbol_, &v);
    IntegerValue* int_value = static_cast<IntegerValue*>(v);
    return int_value->value();
  }
 private:
  std::string symbol_;
};

} // namespace asmvm

#endif
