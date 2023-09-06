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
  return (asm_string != nullptr) && (inst_string != nullptr);
}

IG::Opcode GetOpcode(const std::unique_ptr<llvm::Record> &record)
{
  uint8_t opcode = 0;

  if (record->getValue("op3") != nullptr) {
    auto *bits = record->getValueAsBitsInit("op3");

    // Extract individual bits and convert to uint8_t
    for (unsigned i = 0, e = bits->getNumBits(); i != e; ++i) {
      if (Init *bit = bits->getBit(e - i - 1)) {
        auto value = bit->getAsString() == "1" ? 1 : 0;
        opcode |= static_cast<uint8_t>(value << (e - i - 1));
        // Result += bit->getAsString();
      }
    }

    return { IG::Opcode::Type::Opt3, static_cast<std::byte>(opcode) };
  }

  if (record->getValue("op2") != nullptr) {
    auto *bits = record->getValueAsBitsInit("op2");

    // Extract individual bits and convert to uint8_t
    for (unsigned i = 0, e = bits->getNumBits(); i != e; ++i) {
      if (Init *bit = bits->getBit(e - i - 1)) {
        auto value = bit->getAsString() == "1" ? 1 : 0;
        opcode |= static_cast<uint8_t>(value << (e - i - 1));
        // Result += bit->getAsString();
      }
    }

    return { IG::Opcode::Type::Opt2, static_cast<std::byte>(opcode) };
  }

  return { IG::Opcode::Type::INVALID, {} };
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
