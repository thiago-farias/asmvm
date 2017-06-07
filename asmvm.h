#ifndef ASMVM_H
#define ASMVM_H

#include <map>
#include <string>
#include <stdint.h>


class Value {
 public:
  enum ValueType {
    kValueTypeString,
    kValueTypeInteger
  };
  virtual ~Value();
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
}

class IntegerValue : public Value {
 public:
  explicit IntegerValue(int32_t value) : value_(value) {}
  IntegerValue(const IntegerValue& sv) : value_(sv.value_) {}
  IntegerValue& operator = (const IntegerValue& sv) {
    value_ = sv.value_;
    return *this;
  }
  ValueType type() const { return kValueTypeInteger; }
  std::string value() const { return value_; }
 private:
  int32_t value_;
};

class AsmFile {
 public:
  void add_symbol(const std::string& name, Value* value) {
    symbol_table_.insert(SymbolTable::value_type(name, value));
  }
  
  SymbolTable& symbol_table() { return symbol_table_; }
  const SymbolTable& symbol_table() const { return symbol_table_; }
 private:
  typedef std::map<std::string, Value*> SymbolTable;
  SymbolTable symbol_table_;
};

#endif
