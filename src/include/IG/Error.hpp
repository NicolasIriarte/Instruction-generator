#ifndef SQLGEN_ERROR_H
#define SQLGEN_ERROR_H
#include <llvm/ADT/Twine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/SMLoc.h>

namespace llvm {
struct SMLocError : public ErrorInfo<SMLocError, StringError>
{
  SMLoc Loc;

  SMLocError(SMLoc loc, std::error_code ec, const Twine &s = Twine())
    : ErrorInfo(ec, s), Loc(loc)
  {}
};

Error createTGStringError(SMLoc loc, const Twine &s);
}// end namespace llvm
#endif
