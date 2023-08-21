/**
 * @file   InstructionEmitter.hpp
 * @brief  InstructionEmitter
 *
 * @author Iriarte Nicolas <neiriartefernandez@invap.com.ar>
 * @date   2023-08-13
 */


#ifndef INSTRUCTIONEMITTER_H
#define INSTRUCTIONEMITTER_H

#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TableGen/Record.h>

class InstructionEmitter
{
public:
  explicit InstructionEmitter(llvm::raw_ostream &os) : os_(os) {}

  llvm::Error run(llvm::ArrayRef<const llvm::Record *> queries);

private:
  llvm::raw_ostream &os_;
};

#endif /* INSTRUCTIONEMITTER_H */
