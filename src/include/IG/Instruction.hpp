#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <format>
#include <llvm/TableGen/Record.h>
#include <ostream>
#include <sstream>
#include <string_view>
#include <unordered_map>

namespace IG {


struct Instruction
{
  static constexpr std::string_view TEMPLATE = "static constexpr auto {} = {};";

  explicit Instruction(const llvm::BitsInit *instruction);

  explicit operator std::string() const
  {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

private:
  std::unordered_map<std::string, std::string> involved_registers_;
  friend std::ostream &operator<<(std::ostream &os, const Instruction &inst);
};


std::ostream &operator<<(std::ostream &os, const Instruction &inst);


}// namespace IG

#endif /* INSTRUCTION_H */
