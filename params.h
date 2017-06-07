#ifndef ASMVM_PARAMS_H
#define ASMVM_PARAMS_H

#include "asmvm.h"

namespace asmvm {

class Source {
 public:
  virtual ~Source() {}
  virtual int32_t value(AsmMachine& vm) const = 0;
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
 private:
  BaseAddress* base_;
  Source* offset_;
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

} // namespace asmvm

#endif
