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

// constexpr std::string_view IncludeHeaders() noexcept
// {
//   return "#include <string_view>\n"
//          "#include <cstddef>\n";
// }

// constexpr std::string_view Ident(int n) noexcept
// {
//   constexpr std::string_view spaces{
//     "                                                  "
//   };

//   return { spaces.begin(), spaces.begin() + static_cast<ptrdiff_t>(n * 2) };
// }

// std::string ConstexprStringViewVariable(std::string_view name,
//   std::string_view value)
// {
//   static constexpr std::string_view TEXT{
//     "static constexpr std::string_view {}{{\"{}\"}};"
//   };
//   return std::format(TEXT, name, value);
// }

// std::string ConstexprByteVariable(std::string_view name, std::byte value)
// {
//   static constexpr std::string_view TEXT{
//     "static constexpr std::byte {}{{ {} }};"
//   };
//   return std::format(TEXT, name, static_cast<uint8_t>(value));
// }

}// namespace

namespace IG {

std::string InstructionFileWritter::Heading() const
{
  static constexpr std::string_view HEADING{
    "/**\n"
    " * @file   {}\n"
    " * @author {}\n"
    " * @date   {}\n"
    " */\n"
  };

  return std::format(HEADING, name_ + '.' + extension_, CMake::AUTHOR, Date());
}

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

std::string InstructionFileWritter::HeaderGuardClose() const
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


  static constexpr std::string_view TEXT{ "#endif /* {} */" };

  return std::format(TEXT, guard_filename_str);
}

void InstructionFileWritter::Emit() const
{
  std::filesystem::path file_path =
    output_ / std::string{ name_ + '.' + extension_ };

  if (!std::filesystem::exists(output_)) {
    throw std::runtime_error("Output directory doesn't exists.");
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

namespace Sparc {{

struct {0} {{
  static constexpr std::string_view NAME{{ "{0}" }};
  static constexpr std::string_view ASM{{ "{5}" }};
  static constexpr std::byte OPCODE{{ {6} }};


  static constexpr void Exec() noexcept
  {{
    if constexpr (debug_mode) {{
      std::cout << "DEBUG: name {0}" << std::endl;
    }}

  }}

}};

}} // namespace Sparc

#endif /* {4} */
)"
  };

  file << std::format(TEXT,
    name_,
    extension_,
    CMake::AUTHOR,
    Date(),
    HeaderGuardOpen(),
    asm_,
    static_cast<uint8_t>(opcode_));
}


}// namespace IG
