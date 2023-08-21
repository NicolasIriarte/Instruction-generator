#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TableGen/Record.h>

#include <cstdint>
#include <string_view>


using namespace llvm;

// static constexpr std::string_view FUNCTION_DECLARATION =
//   "/**\n"
//   " * @file   Default.cpp\n"
//   " * @brief  Testing script for small C++ functions and methods.\n"
//   " *\n"
//   " * @author Iriarte Nicolas <neiriartefernandez@invap.com.ar>\n"
//   " * @date   2023-05-21\n"
//   " */\n"
//   "\n"
//   "#include <cstddef>\n"
//   "#include <iostream>\n"
//   "#include <string_view>\n"
//   "\n"
//   "static constexpr bool debug_mode = false;\n"
//   "\n"
//   "namespace Nemu::Sparc {\n"
//   "\n"
//   "struct ADDCCri {\n"
//   "\n"
//   "  static constexpr std::string_view NAME{\"ADDCCri\"};\n"
//   "  static constexpr std::string_view ASM{\"addcc $rs1, $simm13, $rd\"};\n"
//   "\n"
//   "  static constexpr std::byte OPCODE{0b010000};\n"
//   "  static constexpr void Exec() noexcept {\n"
//   "    // #ifdef MYDEBUG_MODE\n"
//   "    //     std::cout << \"Nico: 1-default_2023-08-13-212942.cpp\" << "
//   "std::endl;\n"
//   "    // #endif\n"
//   "\n"
//   "    if constexpr (debug_mode) {\n"
//   "      std::cout << \"Nico: 1-default_2023-08-13-212942.cpp\" <<
//   std::endl;\n" "    }\n" "  }\n"
//   "\n"
//   "private:\n"
//   "};\n"
//   "} // namespace Nemu::Sparc\n"
//   ";\n";

bool IsAsmInstruction(const std::unique_ptr<llvm::Record> &record)
{
  auto *const asm_string = record->getValue("AsmString");
  auto *const inst_string = record->getValue("Inst");
  return (asm_string != nullptr) && (inst_string != nullptr);
}


Error emitInstructions(raw_ostream &os, RecordKeeper &records)
{
  uint32_t num_instructions = 0;
  for (const auto &[name, record] : records.getDefs()) {
    if (IsAsmInstruction(record)) {
      os << "Nico: 1-InstructionEmitter.cpp -> " << name << '\n';

      for (const auto &value : record->getValues()) { os << "\t-> " << value; }
      os << "\n\n\n";

      ++num_instructions;
    }
  }

  os << "Found " << num_instructions << " instructions\n";

  return Error::success();
}
