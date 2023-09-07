#include "include/IG/Instruction.hpp"
#include <llvm/TableGen/Record.h>

#include <bitset>
#include <format>
#include <iostream>
#include <string_view>
#include <unordered_map>

namespace {
std::string GetVariableName(std::string_view strValue)
{
  std::string output;
  for (auto current_char : strValue) {
    if (current_char == '{') { break; }
    output += current_char;
  }
  return output;
}


}// namespace

namespace IG {

Instruction::Instruction(const llvm::BitsInit *instruction)// NOLINT
{
  std::string description = instruction->getAsString();

  /**
   * Represents the following scheme:
   * {
   *    "name": { mask, offset }
   * }
   *
   * For example, the instruction ADDCCrr has this format:
   *
   * {
   *     1, 0,
   *     rd{4}, rd{3}, rd{2}, rd{1}, rd{0},
   *     0, 1, 0, 0, 0, 0,
   *     rs1{4}, rs1{3}, rs1{2}, rs1{1}, rs1{0},
   *     0, 0, 0, 0, 0, 0, 0, 0, 0,
   *     rs2{4}, rs2{3}, rs2{2}, rs2{1}, rs2{0}
   * }
   *
   * and the registers rd could ve represented as:
   *
   * {
   *     "rd": { 5, 7 }
   * }
   *
   * To be translated as:
   *
   * auto rd = (instruction >> (32 - 7)) & 0b11111
   *
   */
  std::unordered_map<std::string, std::pair<uint32_t, uint32_t>>// NOLINT
    variable_data;

  static constexpr auto EXPECTED_BITS = 32;
  if (instruction->getNumBits() != EXPECTED_BITS) {// NOLINT
    std::cerr << "WARNING: Inst with differenct num of bits!";
    return;
  }

  // Extract individual bits and convert to uint8_t
  for (unsigned i = 0, e = instruction->getNumBits(); i != e; ++i) {
    if (auto *bit = instruction->getBit(e - i - 1)) {
      const auto &str_value = bit->getAsString();

      if (str_value == "1" || str_value == "0") { continue; }

      auto variable_name = GetVariableName(str_value);

      auto &[mask, offset] = variable_data[variable_name];

      mask |= static_cast<uint32_t>(1) << (e - i - 1);// NOLINT
      offset = i;
    }
  }

  for (const auto &[key, value] : variable_data) {
    const auto &[mask, offset] = value;
    // std::cout << std::format("uint32_t {} = ", key)
    //           << std::format("(instruction & {:#x}) >> {:#x};",
    //                mask,
    //                31 - offset)// NOLINT
    //           << '\n';

    involved_registers_[key] =
      std::format("(instruction & {:#x}) >> {:#x}", mask, 31 - offset);// NOLINT
  }
}

std::ostream &operator<<(std::ostream &os, const Instruction &inst)
{
  for (const auto &[variable, mask] : inst.involved_registers_) {
    os << std::format(Instruction::TEMPLATE, variable, mask) << '\n';
  }

  return os;
}

}// namespace IG
