#include <list>

#include "asmvm.h"
#include "params.h"

namespace asmvm {
namespace parser {

class StaticHolder {
 public:
  static StaticHolder& instance() { return instance_; }
  void add(asmvm::Printable* prt) {
    print_arg_list_.push_back(prt);
  }
  void clear() {
    print_arg_list_.clear();
  }
  const std::list<asmvm::Printable*>& print_arg_list() const { return print_arg_list_; }
  asmvm::AsmMachine& vm() { return vm_; }
 private:
  std::list<asmvm::Printable*> print_arg_list_;
  asmvm::AsmMachine vm_;
  static StaticHolder instance_;
};

} // namespace parser
} // namespace asmvm
