/**
 * @file   InstructionFileWritter.cpp
 * @brief  Instruction file generator.
 *
 * @author Iriarte Nicolas <neiriartefernandez@invap.com.ar>
 * @date   2023-08-21
 */


#include "IG/InstructionFileWritter.hpp"
#include "IG/CMake/Config.hpp"

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <filesystem>
#include <format>
#include <functional>
#include <iterator>
#include <sstream>
#include <string_view>

namespace {

std::string_view Date() noexcept
{
  static std::string date;

  if (date.empty()) {
    // Get the current time
    std::chrono::system_clock::time_point now =
      std::chrono::system_clock::now();

    // Convert the current time to a time_t
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    // Convert the time_t to a struct tm
    std::tm *local_time = std::localtime(&current_time);

    // Format the date as "DD-MM-YYYY"
    std::ostringstream formatted_date;
    formatted_date << std::put_time(local_time, "%d-%m-%Y");

    date = formatted_date.str();
  }
  return date;
}

}// namespace

namespace IG {

std::string InstructionFileWritter::HeaderGuardOpen() const
{
  std::string guard_filename_str;

  std::transform(name_.begin(),
    name_.end(),
    std::back_inserter(guard_filename_str),
    ::toupper);

  guard_filename_str.append("_");

  std::transform(extension_.begin(),
    extension_.end(),
    std::back_inserter(guard_filename_str),
    ::toupper);

  return guard_filename_str;
}

void InstructionFileWritter::Emit() const
{
  std::filesystem::path file_path =
    output_ / std::string{ name_ + '.' + extension_ };

  if (!std::filesystem::exists(output_)) {
    std::filesystem::create_directory(output_);
  }

  // if (std::filesystem::exists(file_path)) {
  //   throw std::runtime_error(std::string{ "File " } + file_path.string()
  //                            + " already exists, ommiting it.");
  // }


  std::ofstream file(file_path, std::ios_base::trunc);
  // name_
  // extension_
  static constexpr std::string_view TEXT{
    R"(
/**
 * @file   {0}.{1}
 * @author {2}
 * @date   {3}
 */

#ifndef {4}
#define {4}

#include <cstddef>
#include <string_view>

namespace Nemu::Leon3 {{

struct {0} {{
  static constexpr std::string_view NAME{{ "{0}" }};
  static constexpr std::string_view ASM{{ "{5}" }};
  static constexpr std::byte OPT_TYPE{{ {7} }};
  static constexpr std::byte OPCODE{{ {6} }};

  static constexpr void Exec() noexcept {{
    // 1. Deduced variables for instruction:
{8}

    // 2. Operations

    if constexpr (debug_mode) {{
      std::cout << "DEBUG: name {0}" << std::endl;
    }}

  }}

}};

}} // namespace Nemu::Leon3

#endif /* {4} */

/*

;; TagleGen code of this source file, generated on {3}.

{9}

*/

)"
  };

  file << std::format(TEXT,
    name_,// 0
    extension_,// 1
    CMake::AUTHOR,// 2
    Date(),// 3
    HeaderGuardOpen(),// 4
    asm_,// 5
    static_cast<uint32_t>(opcode_.value),// 6
    std::invoke([&]() {// 7
      auto type = opcode_.type;
      switch (type) {
      case Opcode::Type::Opt3:
        return 3;
      case Opcode::Type::Opt2:
        return 2;
      case Opcode::Type::INVALID:
        return 0;
        break;
      }
      return -1;
    }),
    instruction_variables_,// 8
    tablegen_code_// 9

  );
}


}// namespace IG
