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

int32_t OpNot::Exec(AsmMachine& vm) {
  vm.set_register(rindex2_, ~vm.get_register(rindex1_));
  return vm.reg_PC() + 1;
}

int32_t OpJmp::Exec(AsmMachine& vm) {
  Value* value = NULL;
  if (vm.GetSymbolValue(label_, &value)) {
    if (value->type() == Value::kValueTypeInteger) {
      IntegerValue* int_value = static_cast<IntegerValue*>(value);
      return int_value->value();
    }
  }
  printf("Ivalid label in instruction [JMP %s]. Aborting...\n", label_.c_str());
  return -1;
}

int32_t OpCall::Exec(AsmMachine& vm) {
  Value* value = NULL;
  if (vm.GetSymbolValue(label_, &value)) {
    if (value->type() == Value::kValueTypeInteger) {
      IntegerValue* int_value = static_cast<IntegerValue*>(value);
      vm.call_push();
      return int_value->value();
    }
  }
  printf("Ivalid label in instruction [CALL %s]. Aborting...\n", label_.c_str());
  return -1;
}

int32_t OpRet::Exec(AsmMachine& vm) {
  return vm.call_pop() + 1;
}

} // namespace asmvm