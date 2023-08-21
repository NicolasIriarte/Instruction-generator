#include "IG/SQLQueryEmitter.hpp"
#include "IG/Error.hpp"

#include <llvm/ADT/SmallSet.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringSwitch.h>

using namespace llvm;

static void printWhereClauseOperator(const Init *comparisonOperator,
  raw_ostream &os)
{
  os << StringSwitch<const char *>(comparisonOperator->getAsString())
          .Case("gt", " > ")
          .Case("ge", " <= ")
          .Case("lt", " < ")
          .Case("le", " <=")
          .Case("ne", " <> ")
          .Case("eq", " = ")
          .Case("or", " OR ")
          .Case("and", " AND ")
          .Default(" (Unrecognized operator) ");
}

static void visitWhereClause(const DagInit *term, raw_ostream &os)// NOLINT
{
  const auto *Operator = term->getOperator();
  for (unsigned i = 0U; i != term->arg_size(); ++i) {
    const auto *Arg = term->getArg(i);
    if (const auto *DagArg = dyn_cast<DagInit>(Arg)) {
      visitWhereClause(DagArg, os);
    } else {
      if (term->getArgNameStr(i) == "str") {
        os << Arg->getAsString();
      } else {
        os << Arg->getAsUnquotedString();
      }
    }
    if (i < term->arg_size() - 1) { printWhereClauseOperator(Operator, os); }
  }
}

Error SQLQueryEmitter::run(ArrayRef<const Record *> queries)// NOLINT
{
  // Map from the tag name to its corresponding SQL table field name.
  StringMap<StringRef> FieldTagMap;

  for (const auto *QueryRecord : queries) {
    auto MaybeTableName = QueryRecord->getValueAsOptionalString("TableName");
    if (!MaybeTableName || MaybeTableName->empty()) {
      return createTGStringError(QueryRecord->getLoc()[0],
        "SQL table name "
        "is missing");
    }
    auto TableName = *MaybeTableName;

    OS << "SELECT ";

    const DagInit *FieldsInit = QueryRecord->getValueAsDag("Fields");
    auto FieldOpName = FieldsInit->getOperator()->getAsString();
    if (FieldOpName == "all") {
      OS << "*";
    } else if (FieldOpName != "fields") {
      // FIXME: This is a terrible SMLoc to use here.
      return createTGStringError(QueryRecord->getLoc()[0],
        "Invalid dag operator"
        " \""
          + FieldOpName + "\"");
    }

    FieldTagMap.clear();
    {
      ListSeparator LS;
      for (unsigned i = 0U; i != FieldsInit->arg_size(); ++i) {
        if (const auto *Field = dyn_cast<StringInit>(FieldsInit->getArg(i))) {
          OS << LS << Field->getValue();
          if (const auto *Tag = FieldsInit->getArgName(i)) {
            FieldTagMap.insert({ Tag->getValue(), Field->getValue() });
          }
        }
      }
    }

    OS << " FROM " << TableName;

    const DagInit *WhereClause = QueryRecord->getValueAsDag("WhereClause");
    if (WhereClause->getOperator()->getAsString() != "none") {
      OS << "\n";
      OS << "WHERE ";
      visitWhereClause(WhereClause, OS);
    }

    auto OrderedBy = QueryRecord->getValueAsListOfStrings("OrderedBy");
    if (!OrderedBy.empty()) {
      OS << "\n";
      OS << "ORDER BY ";
      ListSeparator LS;
      for (auto FieldOrTag : OrderedBy) {
        auto FieldName = FieldOrTag;
        if (FieldOrTag.startswith("$")) {
          // It's a tag
          auto TagName = FieldOrTag.drop_front(1);
          auto I = FieldTagMap.find(TagName);
          if (I == FieldTagMap.end()) {
            return createTGStringError(QueryRecord->getLoc()[0],
              "Unrecognized "
              "tag \""
                + TagName + "\"");
          }
          FieldName = I->second;
        }
        OS << LS << FieldName;
      }
    }

    OS << ";\n\n";
  }

  return Error::success();
}
