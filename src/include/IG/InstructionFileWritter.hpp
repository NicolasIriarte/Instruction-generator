/**
 * @file   InstructionFileWritter.hpp
 * @brief  Instruction generator utility class
 *
 * @author Iriarte Nicolas <neiriartefernandez@invap.com.ar>
 * @date   2023-08-21
 */


#ifndef INSTRUCTIONFILEWRITTER_H
#define INSTRUCTIONFILEWRITTER_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace IG {

struct Opcode
{
  enum class Type {
    Opt3,
    Opt2,
    INVALID,
  };

  Type type;
  std::byte value;
};

class InstructionFileWritter
{
public:
  InstructionFileWritter(std::string name, std::string instAsm)
    : name_(std::move(name)), asm_(std::move(instAsm))
  {}

  void Emit() const;

  void SetOpcode(Opcode opcode) { opcode_ = opcode; }

  void AddVariables(std::string variablesAsString)
  {
    instruction_variables_ = std::move(variablesAsString);
  }

private:
  [[nodiscard]] std::string HeaderGuardOpen() const;

  std::string name_;
  std::string asm_;

  Opcode opcode_{};

  std::string instruction_variables_;

  std::filesystem::path output_{ "/tmp/Leon3" };
  std::string extension_{ "hpp" };
};

}// namespace IG

#endif /* INSTRUCTIONFILEWRITTER_H */
