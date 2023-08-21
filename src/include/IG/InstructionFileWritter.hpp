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


class InstructionFileWritter
{
public:
  InstructionFileWritter(std::string name, std::string instAsm)
    : name_(std::move(name)), asm_(std::move(instAsm))
  {}

  void Emit() const;

  void SetOpcode(uint8_t op3) { opcode_ = static_cast<std::byte>(op3); }

private:
  [[nodiscard]] std::string Heading() const;

  [[nodiscard]] std::string HeaderGuardOpen() const;
  [[nodiscard]] std::string HeaderGuardClose() const;

  std::string name_;
  std::string asm_;

  std::byte opcode_{};

  std::filesystem::path output_{ "/tmp/Leon3" };
  std::string extension_{ "hpp" };
};

}// namespace IG

#endif /* INSTRUCTIONFILEWRITTER_H */
