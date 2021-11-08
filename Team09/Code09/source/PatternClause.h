#pragma once

#include "QueryArgument.h"
#include "QueryClause.h"

using namespace std;

typedef string PATTERN;

/*
* Stores a list of valid pattern synonym type
*/
static set<DESIGN_ENTITY> validPatternSynType = {
	DESIGN_ENTITY::ASSIGN, DESIGN_ENTITY::IF, DESIGN_ENTITY::WHILE
};

/*
* Stores a mapping of pattern synonym type to valid argument type enums for the first argument
*/
static unordered_map<DESIGN_ENTITY, vector<set<ARGUMENT_TYPE>>> mapPatternSynTypeToArgsTypes = {
	{DESIGN_ENTITY::ASSIGN,
		{
			{ARGUMENT_TYPE::UNDERSCORE, ARGUMENT_TYPE::IDENT, ARGUMENT_TYPE::SYNONYM},
			{ARGUMENT_TYPE::EXPRESSION_SPEC}
		}
	},
	{DESIGN_ENTITY::IF,
		{
			{ARGUMENT_TYPE::UNDERSCORE, ARGUMENT_TYPE::IDENT, ARGUMENT_TYPE::SYNONYM},
			{ARGUMENT_TYPE::UNDERSCORE},
			{ARGUMENT_TYPE::UNDERSCORE}
		}
	},
	{DESIGN_ENTITY::WHILE,
		{
			{ARGUMENT_TYPE::UNDERSCORE, ARGUMENT_TYPE::IDENT, ARGUMENT_TYPE::SYNONYM},
			{ARGUMENT_TYPE::UNDERSCORE}
		}
	}
};

/*
* Stores a mapping of pattern synonym type to valid synonym type enums for the first argument of synonym type
*/
static unordered_map<DESIGN_ENTITY, vector<set<DESIGN_ENTITY>>> mapPatternSynTypeToArgsSynTypes = {
	{DESIGN_ENTITY::ASSIGN,
		{
			{DESIGN_ENTITY::VARIABLE},
			{}
		}
	},
	{DESIGN_ENTITY::IF,
		{
			{DESIGN_ENTITY::VARIABLE},
			{},
			{}
		}
	},
	{DESIGN_ENTITY::WHILE,
		{
			{DESIGN_ENTITY::VARIABLE},
			{}
		}
	}
};

/*
* Enum to describe a possible pattern matching type
*/
enum class PATTERN_MATCH_TYPE {
	EXACT_MATCH, PARTIAL_MATCH, UNRESTRICTED, INVALID
};

/*
* Represents a query pattern with a pattern matching type and the pattern to be matched
*/
struct QueryPattern {
	PATTERN_MATCH_TYPE patternMatchType = PATTERN_MATCH_TYPE::INVALID;
	PATTERN patternStr;
};

inline
bool operator== (QueryPattern lhs, QueryPattern rhs) {
	return lhs.patternMatchType == rhs.patternMatchType &&
		lhs.patternStr == rhs.patternStr;
}

/*
* Represents a pattern clause in a query
* 
* Example: "assign a; Select a pattern a(_, "x + y")" is stored as:
* clauseType = CLAUSE_TYPE::PATTERN
* patternSynType = DESIGN_ENTITY::ASSIGN
* synIndex = 1
* arg = { NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", { NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE } }
* pattern = { PATTERN_MATCH_TYPE::EXACT_MATCH, "x+y" }
* 
* Example: "if i; Select i pattern i("v", _, _)" is stored as:
* clauseType = CLAUSE_TYPE::PATTERN
* patternSynType = DESIGN_ENTITY::IF
* synIndex = 1
* arg = { NO_ERROR, ARGUMENT_TYPE::IDENT, "v", { NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE } }
* pattern = NULL
*/
class PatternClause : public QueryClause {
	CLAUSE_TYPE clauseType = CLAUSE_TYPE::PATTERN;

public:
	PatternClause();
	PatternClause(DESIGN_ENTITY patternSynType, SYNONYM_INDEX synIndex, QueryArgument arg, QueryPattern pattern);

	DESIGN_ENTITY patternSynType = DESIGN_ENTITY::NONE;
	SYNONYM_INDEX synIndex;
	QueryArgument arg;
	QueryPattern pattern;
};

inline
bool operator== (PatternClause lhs, PatternClause rhs) {
	return lhs.getClauseSynonyms() == rhs.getClauseSynonyms() &&
		lhs.patternSynType == rhs.patternSynType &&
		lhs.synIndex == rhs.synIndex &&
		lhs.arg == rhs.arg &&
		lhs.pattern == rhs.pattern;
}
