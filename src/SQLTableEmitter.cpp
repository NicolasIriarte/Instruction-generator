#include <llvm/ADT/StringExtras.h>

#include "IG/Error.hpp"
#include "IG/SQLTableEmitter.hpp"

using namespace llvm;

namespace {
enum class SQLType { Unknown, Integer, VarChar };
}// end anonymous namespace

namespace llvm {
raw_ostream &operator<<(raw_ostream &os, const SQLType &ty)
{
  switch (ty) {
  case SQLType::Integer:
    os << "int";
    break;
  case SQLType::VarChar:
    os << "varchar(255)";
    break;
  default:
    break;
  }
  return os;
}
}// end namespace llvm

Error SQLTableEmitter::run(ArrayRef<const Record *> classes)// NOLINT
{
  // {SQL data type, member name}
  SmallVector<std::pair<SQLType, StringRef>, 4> TableMembers;
  // {foreign key member, referee class, referee primary key}
  SmallVector<std::tuple<StringRef, const Record *, StringRef>, 4> ForeignKeys;

  for (const auto *Class : classes) {
    StringRef CurPrimaryKey;
    TableMembers.clear();
    ForeignKeys.clear();
    for (const auto &RV : Class->getValues()) {
      // We only care about member variables.
      if (RV.isTemplateArg()) { continue; }

      auto Name = RV.getName();
      // The PrimaryKey member is not directly used.
      if (Name == "PrimaryKey") { continue; }

      if (auto *VI = dyn_cast<VarInit>(RV.getValue())) {
        if (VI->getName() == "PrimaryKey") { CurPrimaryKey = Name; }
      }

      SQLType SQLTy = SQLType::Unknown;
      const auto *RVT = RV.getType();
      switch (RVT->getRecTyKind()) {
      case RecTy::IntRecTyKind:
      case RecTy::RecordRecTyKind:
        SQLTy = SQLType::Integer;
        if (const auto *RRT = dyn_cast<RecordRecTy>(RVT)) {
          // Implement members with RecordRecTy with foreign keys.
          bool FoundFK = false;
          for (const auto *C : RRT->getClasses()) {
            if (PrimaryKeys.count(C) != 0U) {
              ForeignKeys.push_back({ Name, C, PrimaryKeys[C] });
              FoundFK = true;
              break;
            }
          }

          if (!FoundFK) {
            return createTGStringError(RV.getLoc(),
              "Cannot locate primary key "
              "of the referred table");
          }
        }
        break;
      case RecTy::StringRecTyKind:
        SQLTy = SQLType::VarChar;
        break;
      default:
        return createTGStringError(
          RV.getLoc(), "Unsupported table member type");
      }
      TableMembers.push_back({ SQLTy, Name });
    }

    // Write down the primary key of this table.
    if (!CurPrimaryKey.empty()) {
      PrimaryKeys.insert({ Class, CurPrimaryKey });
    }

    ListSeparator LS(",\n");
    OS << "CREATE TABLE " << Class->getName() << " (\n";
    for (const auto &Member : TableMembers) {
      (OS << LS).indent(4) << Member.second << " " << Member.first;
    }
    if (!CurPrimaryKey.empty()) {
      (OS << LS).indent(4) << "PRIMARY KEY (" << CurPrimaryKey << ")";
    }
    for (const auto &FK : ForeignKeys) {
      (OS << LS).indent(4) << "FOREIGN KEY (" << std::get<0>(FK) << ") "
                           << "REFERENCES " << std::get<1>(FK)->getName() << "("
                           << std::get<2>(FK) << ")";
    }
    OS << "\n);\n\n";
  }

  return Error::success();
}
