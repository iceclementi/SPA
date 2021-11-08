#pragma once

#include "Query.h"

/*
* Enum to describe a possible clause type
*/
enum class CLAUSE_TYPE {
	SUCH_THAT, PATTERN, WITH, INVALID
};

/*
* Stores a mapping of clause type enum to its first word string
*/
static unordered_map<CLAUSE_TYPE, string> mapClauseTypeToFirstWord = {
	{CLAUSE_TYPE::SUCH_THAT, "such"},
	{CLAUSE_TYPE::PATTERN, "pattern"},
	{CLAUSE_TYPE::WITH, "with"},
};

/*
* Represents a clause in a query and is identified by its clause type
*/
class QueryClause {
public:
	QueryClause(SYNONYM_INDICES clauseSynsIndices);
	QueryClause() = default;

	SYNONYM_INDICES& getClauseSynonyms();
	void addClauseSynonym(SYNONYM_INDEX synIndex);
	
private:
	SYNONYM_INDICES clauseSynonyms;
};

inline
bool operator== (QueryClause lhs, QueryClause rhs) {
	return lhs.getClauseSynonyms() == rhs.getClauseSynonyms();
}
