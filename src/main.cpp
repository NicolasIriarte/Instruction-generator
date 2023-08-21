#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/WithColor.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TableGen/Error.h>
#include <llvm/TableGen/Main.h>
#include <llvm/TableGen/Record.h>

#include "IG/Error.hpp"

#include <iostream>

using namespace llvm;

namespace {
enum ActionType {
  PrintRecords,
  PrintDetailedRecords,
  EmitInstructions,
};
}// end anonymous namespace

// NOLINTNEXTLINE
static cl::opt<ActionType> Action(cl::desc("Actions to perform"),
  cl::values(// Arg values
    clEnumValN(PrintRecords, "print-records", ""),
    clEnumValN(PrintDetailedRecords, "print-detailed-records", ""),
    clEnumValN(EmitInstructions, "emit-instructions", "")),
  cl::init(PrintRecords));

Error emitSQL(raw_ostream &os, RecordKeeper &records);
Error emitInstructions(raw_ostream &os, RecordKeeper &records);

bool SQLGenMain(raw_ostream &os, RecordKeeper &records)
{
  switch (Action) {
  case PrintRecords:
    os << records;
    break;
  case EmitInstructions:
    if (auto E = emitInstructions(os, records)) {
      handleAllErrors(
        std::move(E),
        [](const SMLocError &e) {
          llvm::PrintError(e.Loc.getPointer(), e.getMessage());
        },
        [](const ErrorInfoBase &e) {
          e.log(WithColor::error());
          errs() << "\n";
        });
      return true;
    }
    break;
  default:
    return true;
  }

  return false;
}

int main(int argc, char **argv)
{
  cl::ParseCommandLineOptions(argc, argv);
  return llvm::TableGenMain(argv[0], &SQLGenMain);// NOLINT
}
