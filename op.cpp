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

int32_t ConditionalJump::Exec(AsmMachine& vm) {
  Value* value = NULL;
  if (jmp_condition(vm) && vm.GetSymbolValue(label_, &value)) {
    if (value->type() == Value::kValueTypeInteger) {
      IntegerValue* int_value = static_cast<IntegerValue*>(value);
      return int_value->value();
    } else {
      printf("Ivalid label in instruction [JMP %s]. Aborting...\n", label_.c_str());
      return -1;
    }
  }
  return vm.reg_PC() + 1;  
}

int32_t OpMov::Exec(AsmMachine& vm) {
  vm.set_register(rindex_dst_, src_->value(vm));
  return vm.reg_PC() + 1;
}

int32_t OpPush::Exec(AsmMachine& vm) {
  if (!vm.push_value(src_->value(vm))) {
    printf("Stack overflow. Default memory size = %d.", kDefaultMemorySize);
    return -1;
  }
  return vm.reg_PC() + 1;
}

int32_t OpPop::Exec(AsmMachine& vm) {
  int32_t value = 0;
  if (!vm.pop(&value)) {
    printf("Invalid POP operation. Stack is empty.");
    return -1;
  }
  if (store_value_) { 
    vm.set_register(rindex_, value);
  }
  return vm.reg_PC() + 1;
}

int32_t OpSt1::Exec(AsmMachine& vm) {
  if (!vm.push_value(uint8_t(src_->value(vm)), address_->address(vm), 0)) {
    printf("Invalid address [%d]. Default memory size = %d.\n", address_->address(vm), kDefaultMemorySize);
  }
  return vm.reg_PC() + 1;
}

int32_t OpSt2::Exec(AsmMachine& vm) {
  if (!vm.push_value(uint16_t(src_->value(vm)), address_->address(vm), 0)) {
    printf("Invalid address [%d]. Default memory size = %d.\n", address_->address(vm), kDefaultMemorySize);
  }
  return vm.reg_PC() + 1;
}

int32_t OpSt4::Exec(AsmMachine& vm) {
  if (!vm.push_value(src_->value(vm), address_->address(vm), 0)) {
    printf("Invalid address [%d]. Default memory size = %d.\n", address_->address(vm), kDefaultMemorySize);
  }
  return vm.reg_PC() + 1;
}

int32_t OpExit::Exec(AsmMachine& vm) {
  printf("\nProgram exit with code %d.\n", code_->value(vm));
  return -1;
}

int32_t OpInc::Exec(AsmMachine& vm) {
  vm.set_register(rindex_, vm.get_register(rindex_) + 1);
  return vm.reg_PC() + 1;
}

int32_t OpDec::Exec(AsmMachine& vm) {
  vm.set_register(rindex_, vm.get_register(rindex_) - 1);
  return vm.reg_PC() + 1;
}

int32_t OpPrint::Exec(AsmMachine& vm) {
  for (auto i = printables_.begin(); i != printables_.end(); i++) {
    printf("%s", (*i)->str(vm).c_str());
  }
  return vm.reg_PC() + 1;
}

int32_t OpLd1::Exec(AsmMachine& vm) {
  uint8_t value;
  if (!vm.load_value(address_->address(vm), 0, &value)) {
    printf("Invalid address [%d]. Default memory size = %d.\n", address_->address(vm), kDefaultMemorySize);
  }
  vm.set_register(rindex_, value);
  return vm.reg_PC() + 1;
}

int32_t OpLd2::Exec(AsmMachine& vm) {
  uint16_t value;
  if (!vm.load_value(address_->address(vm), 0, &value)) {
    printf("Invalid address [%d]. Default memory size = %d.\n", address_->address(vm), kDefaultMemorySize);
  }
  vm.set_register(rindex_, value);
  return vm.reg_PC() + 1;
}

int32_t OpLd4::Exec(AsmMachine& vm) {
  uint32_t value;
  if (!vm.load_value(address_->address(vm), 0, &value)) {
    printf("Invalid address [%d]. Default memory size = %d.\n", address_->address(vm), kDefaultMemorySize);
  }
  vm.set_register(rindex_, value);
  return vm.reg_PC() + 1;
}

} // namespace asmvm