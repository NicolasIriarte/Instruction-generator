#include "IG/InstructionFileWritter.hpp"
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TableGen/Record.h>

#include <bitset>
#include <cstdint>
#include <string_view>


using namespace llvm;

bool IsAsmInstruction(const std::unique_ptr<llvm::Record> &record)
{
  auto *const asm_string = record->getValue("AsmString");
  auto *const inst_string = record->getValue("Inst");
  auto *const op_code = record->getValue("op3");
  return (asm_string != nullptr) && (inst_string != nullptr)
         && (op_code != nullptr);
}

uint8_t GetOpcode(const std::unique_ptr<llvm::Record> &record)
{
  uint8_t op3 = 0;
  auto *bits = record->getValueAsBitsInit("op3");

  // Extract individual bits and convert to uint8_t
  for (unsigned i = 0, e = bits->getNumBits(); i != e; ++i) {
    if (Init *bit = bits->getBit(e - i - 1)) {
      auto value = bit->getAsString() == "1" ? 1 : 0;
      op3 |= static_cast<uint8_t>(value << (e - i - 1));
      // Result += bit->getAsString();
    }
  }

  return op3;
}


Error emitInstructions(raw_ostream &os, RecordKeeper &records)
{
  uint32_t num_instructions = 0;
  for (const auto &[name, record] : records.getDefs()) {
    if (IsAsmInstruction(record)) {
      IG::InstructionFileWritter instruction(
        name, record->getValueAsString("AsmString").str());

      instruction.SetOpcode(GetOpcode(record));

      instruction.Emit();

      ++num_instructions;
    }
  }

  os << "Emitted " << num_instructions << " instructions\n";

  return Error::success();
}
