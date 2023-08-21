/**
 * @file   Default.cpp
 * @brief  Testing script for small C++ functions and methods.
 *
 * @author Iriarte Nicolas <neiriartefernandez@invap.com.ar>
 * @date   2023-05-21
 */

#include <cstddef>
#include <iostream>
#include <string_view>

#define MYDEBUG_MODE

static constexpr bool debug_mode = false;

namespace Nemu::Sparc {

struct ADDCCri
{

  static constexpr std::string_view NAME{ "ADDCCri" };
  static constexpr std::string_view ASM{ "addcc $rs1, $simm13, $rd" };

  static constexpr std::byte OPCODE{ 0b010000 };
  static constexpr void Exec() noexcept
  {
    // #ifdef MYDEBUG_MODE
    //     std::cout << "Nico: 1-default_2023-08-13-212942.cpp" << std::endl;
    // #endif

    if constexpr (debug_mode) {
      std::cout << "Nico: 1-default_2023-08-13-212942.cpp" << std::endl;
    }
  }

private:
};
}// namespace Nemu::Sparc

int main(int argc, char **argv)
{
  // Nemu::Sparc::ADDCCri addccri;

  Nemu::Sparc::ADDCCri::Exec();

  std::cout << Nemu::Sparc::ADDCCri::NAME << std::endl;

  return 0;
}

/*
  Local Variables:
  quickrun-option-cmd-alist: ((:command . "g++")
                              (:args    . "apple melon")
                              (:exec    . ("%c -Werror -std=c++23 -o %n %s"
                                           "%n %a"))
                              (:remove  . ("%n")))
  End:
*/
