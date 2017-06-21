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
  if (code_->value(vm) < 0) return -1;
  return -1 - code_->value(vm);
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

enum SysCallCode {
  kSysCallFopen = 0,
  kSysCallFclose,
  kSysCallFprint,
  kSysCallFread,
  kSysCallFwrite,
  kSysCallFseek,
  kSysCallNow,
  kSysCallSleep,
  kSysCallSocket,
  kSysCallTCPConnect,
  kSysCallSend,
  kSysCallRecv,
  kSysCallListen,
  kSysCallBind
};

enum OpenMode {
  kOpenModeRead = 1,
  kOpenModeWrite = 2,
  kOpenModeCreate = 4,
  kOpenModeBinary = 8,
  kOpenModeAppend = 16
};

enum ParamType {
  kTypeString = 0,
  kTypeInt,
  kTypeNoMoreParams
};


int32_t OpSysCall::Exec(AsmMachine& vm) {
  int32_t function_code = src_->value(vm);
  int32_t pointer;
  int32_t mode;
  int32_t nparams;
  FILE* handler = NULL;
  const char* filename = NULL;
  switch (function_code) {
  case kSysCallFopen:
    //printf("[kSysCallFopen]\n");
    vm.pop(&pointer);
    vm.pop(&mode);
    filename = (const char*)vm.data() + pointer;
    if ((mode & (kOpenModeRead | kOpenModeWrite | kOpenModeCreate | kOpenModeBinary)) != 0) {
      handler = fopen(filename, "wb+");
    } else if ((mode & (kOpenModeRead | kOpenModeWrite | kOpenModeCreate)) != 0) {
      handler = fopen(filename, "w+");
    } else if ((mode & (kOpenModeRead | kOpenModeWrite)) != 0) {
      handler = fopen(filename, "r+");
    } else if ((mode & (kOpenModeRead)) != 0) {
      handler = fopen(filename, "r");
    } else if ((mode & (kOpenModeWrite | kOpenModeCreate)) != 0) {
      handler = fopen(filename, "w");
    } else if ((mode & (kOpenModeRead | kOpenModeWrite | kOpenModeBinary)) != 0) {
      handler = fopen(filename, "rb+");
    } else if ((mode & (kOpenModeRead | kOpenModeBinary)) != 0) {
      handler = fopen(filename, "rb");
    } else if ((mode & (kOpenModeWrite | kOpenModeBinary)) != 0) {
      handler = fopen(filename, "wb");
    } else if ((mode & (kOpenModeAppend | kOpenModeBinary)) != 0) {
      handler = fopen(filename, "ab");
    } else if ((mode & (kOpenModeAppend)) != 0) {
      handler = fopen(filename, "a");
    } else if ((mode & (kOpenModeAppend | kOpenModeRead | kOpenModeBinary)) != 0) {
      handler = fopen(filename, "ab+");
    } else if ((mode & (kOpenModeAppend | kOpenModeRead )) != 0) {
      handler = fopen(filename, "a+");
    }
    vm.push_value((int32_t)handler);
    break;
  case kSysCallFclose:
    vm.pop(&pointer);
    handler = reinterpret_cast<FILE*>(pointer);
    fclose(handler);
    break;
  case kSysCallFprint: {
      int type = -1;
      vm.pop(&pointer);
      handler = reinterpret_cast<FILE*>(pointer);
      do {
        const char* str = NULL;
        int32_t value;
        vm.pop(&type);
        switch (type) {
        case kTypeString:
          vm.pop(&pointer);
          str = (const char*)vm.data() + pointer;
          //printf("[kSysCallFprint] str = [%s]\n", str);
          fprintf(handler, "%s", str);
          break;
        case kTypeInt:
          vm.pop(&value);
          fprintf(handler, "%d", value);
          //printf("[kSysCallFprint] int = [%d]\n", value);
          break;
        }
      } while (type != kTypeNoMoreParams);
    }
    break;
  }
  vm.set_register(rindex_, 0);
  return vm.reg_PC() + 1;
}

} // namespace asmvm