#include "IG/Error.hpp"

llvm::Error llvm::createTGStringError(SMLoc loc, const Twine &s)
{
  return make_error<SMLocError>(loc, inconvertibleErrorCode(), s);
}
