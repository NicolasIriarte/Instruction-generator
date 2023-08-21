#include <llvm/ADT/StringExtras.h>

#include "IG/Error.hpp"
#include "IG/SQLInsertEmitter.hpp"

using namespace llvm;

Error SQLInsertEmitter::insertRowImpl(const Record *rowRecord)// NOLINT
{
  if (InsertedRows.count(rowRecord) != 0U) { return Error::success(); }

  SmallVector<Record *, 1> DirectSuperClasses;
  // {member / SQL column name, value to be inserted (in string)}
  SmallVector<std::pair<StringRef, std::string>, 4> Values;

  DirectSuperClasses.clear();
  rowRecord->getDirectSuperClasses(DirectSuperClasses);
  if (DirectSuperClasses.size() != 1) {
    return createTGStringError(rowRecord->getLoc()[0],
      "Insert to multiple "
      "tables at once is not supported");
  }
  const Record *Class = DirectSuperClasses[0];
  if (TableSizes.count(Class) == 0U) { TableSizes.insert({ Class, 0U }); }
  auto MaybePrimaryKey = GetPrimaryKeyCB(Class);
  if (!MaybePrimaryKey) {
    return createTGStringError(
      rowRecord->getLoc()[0], "Cannot find the corresponding primary key");
  }
  auto PrimaryKey = *MaybePrimaryKey;

  Values.clear();
  for (const auto &RV : rowRecord->getValues()) {
    auto Name = RV.getName();
    // We don't directly use the PrimaryKey member.
    if (Name == "PrimaryKey") { continue; }

    if (Name == PrimaryKey) {
      // Create a new primary key value.
      Values.push_back({ Name, std::to_string(TableSizes[Class]) });
    } else {
      const Init *Val = RV.getValue();
      if (const auto *IntVal = dyn_cast<IntInit>(Val)) {
        // Integer
        Values.push_back({ Name, std::to_string(IntVal->getValue()) });
      } else if (const auto *StrVal = dyn_cast<StringInit>(Val)) {
        // String
        Values.push_back({ Name, StrVal->getAsString() });
      } else if (const auto *DefVal = dyn_cast<DefInit>(Val)) {
        // Another row Record
        const auto *DefRecord = DefVal->getDef();
        if (!DefRecord->isSubClassOf("Table")) {
          return createTGStringError(DefRecord->getLoc()[0],
            "Reference to another non SQL row Record "
            "is not supported yet");
        }
        // Insert the referred row if needed.
        if (auto E = insertRowImpl(DefRecord)) { return E; }
        auto RowI = InsertedRows.find(DefRecord);
        if (RowI == InsertedRows.end()) {
          return createTGStringError(DefRecord->getLoc()[0],
            "Cannot find record \"" + DefRecord->getName() + "\"");
        }
        Values.push_back({ Name, std::to_string(RowI->second) });
      } else {
        return createTGStringError(RV.getLoc(), "Unsupported value type");
      }
    }
  }

  {
    ListSeparator LS;
    OS << "INSERT INTO " << Class->getName() << " (";
    for (const auto &VP : Values) { OS << LS << VP.first; }
    OS << ")\n";
  }
  {
    ListSeparator LS;
    OS << "VALUES (";
    for (const auto &VP : Values) { OS << LS << VP.second; }
    OS << ");\n\n";
  }

  InsertedRows.insert({ rowRecord, TableSizes[Class]++ });

  return Error::success();
}
Error SQLInsertEmitter::run(ArrayRef<const Record *> rows)
{
  for (const auto *RowRecord : rows) {
    if (auto E = insertRowImpl(RowRecord)) { return E; }
  }

  return Error::success();
}
