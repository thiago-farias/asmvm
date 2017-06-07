#include "op.h"

namespace asmvm {

int32_t OpAdd::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) + param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpSub::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) - param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpMul::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) * param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpDiv::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), int32_t(param1()->value(vm) / param2()->value(vm)));
  return vm.reg_PC() + 1;
}

int32_t OpMod::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) % param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpAnd::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) & param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpOr::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) | param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpXor::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) ^ param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpShl::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) << param2()->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpShr::Exec(AsmMachine& vm) {
  vm.set_register(output_rindex(), param1()->value(vm) >> param2()->value(vm));
  return vm.reg_PC() + 1;
}

} // namespace asmvm