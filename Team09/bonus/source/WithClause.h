#pragma once

#include "QueryArgument.h"
#include "QueryClause.h"

using namespace std;

/*
* Enum to represent the general type of a with clause argument
* More generalised version of the ARGUMENT_TYPE enum
* Used to validate the compabaility of the two arguments in a with clause
*/
enum class WITH_ARG_TYPE {
	INTEGER, STRING
};

/*
* Stores a list of valid argument type enums for each argument
*/
static vector<set<ARGUMENT_TYPE>> validWithArgsTypes = {
	{ARGUMENT_TYPE::INTEGER, ARGUMENT_TYPE::IDENT, ARGUMENT_TYPE::ATTRIBUTE_REF, ARGUMENT_TYPE::SYNONYM},
	{ARGUMENT_TYPE::INTEGER, ARGUMENT_TYPE::IDENT, ARGUMENT_TYPE::ATTRIBUTE_REF, ARGUMENT_TYPE::SYNONYM}
};

/*
* Stores a set of valid synonym type enums for each argument
*/
static vector<set<DESIGN_ENTITY>> validWithArgsSynTypes = {
	{DESIGN_ENTITY::PROG_LINE},
	{DESIGN_ENTITY::PROG_LINE}
};

/*
* Stores a mapping of argument type to a more general with clause argument type
*/
static unordered_map<ARGUMENT_TYPE, WITH_ARG_TYPE> mapArgTypeToGeneralWithArgType = {
	{ARGUMENT_TYPE::INTEGER, WITH_ARG_TYPE::INTEGER},
	{ARGUMENT_TYPE::IDENT, WITH_ARG_TYPE::STRING},
	{ARGUMENT_TYPE::SYNONYM, WITH_ARG_TYPE::INTEGER}
};

/*
* Stores a mapping of attribute type (for an argument of ATTRIBUTE_REF type) to a more general with clause argument type
*/
static unordered_map<ATTRIBUTE_TYPE, WITH_ARG_TYPE> mapAttrRefTypeToGeneralWithArgType = {
	{ATTRIBUTE_TYPE::NAME, WITH_ARG_TYPE::STRING},
	{ATTRIBUTE_TYPE::INTEGER, WITH_ARG_TYPE::INTEGER}
};

/*
* Represents a with clause in a query
* 
* Example: "stmt a; print b; Select BOOLEAN with a.stmt# = 1" is stored as:
* clauseType = CLAUSE_TYPE::WITH
* firstArg = { NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "a.stmt#", { NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::INTEGER } }
* secondArg = { NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", { NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE } }
*/
class WithClause : public QueryClause {
	CLAUSE_TYPE clauseType = CLAUSE_TYPE::WITH;

public:
	WithClause();
	WithClause(QueryArgument firstArg, QueryArgument secondArg);

	QueryArgument firstArg;
	QueryArgument secondArg;
};

inline
bool operator== (WithClause lhs, WithClause rhs) {
	return lhs.getClauseSynonyms() == rhs.getClauseSynonyms() &&
		lhs.firstArg == rhs.firstArg &&
		lhs.secondArg == rhs.secondArg;
}
