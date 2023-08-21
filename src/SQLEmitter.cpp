#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TableGen/Record.h>

#include "IG/SQLInsertEmitter.hpp"
#include "IG/SQLQueryEmitter.hpp"
#include "IG/SQLTableEmitter.hpp"

using namespace llvm;

Error emitSQL(raw_ostream &os, RecordKeeper &records)
{

  if (const auto *TableClass = records.getClass("Table")) {
    // Generate SQL tables
    const auto &Classes = records.getClasses();
    SmallVector<const Record *, 4> TableClasses;
    for (const auto &P : Classes) {
      if (P.second->isSubClassOf(TableClass)) {
        TableClasses.push_back(P.second.get());
      }
    }
    SQLTableEmitter TableEmitter(os);
    if (auto E = TableEmitter.run(TableClasses)) { return E; }

    auto getPrimaryKey = [&](const Record *givenClass) -> Optional<StringRef> {
      return TableEmitter.getPrimaryKey(givenClass);
    };

    // Generate SQL rows
    auto SQLRows = records.getAllDerivedDefinitions("Table");
    // RecordKeeper::getAllDerivedDefinitions will only
    // return concrete records, so we don't need to filter out
    // class `Record` instances.
    if (auto E = SQLInsertEmitter(os, getPrimaryKey).run(SQLRows)) { return E; }
  }

  if (records.getClass("Query") != nullptr) {
    auto SQLQueries = records.getAllDerivedDefinitions("Query");
    if (auto E = SQLQueryEmitter(os).run(SQLQueries)) { return E; }
  }

  return Error::success();
}
