#pragma once

#include "Common.h"
#include "QueryClause.h"

typedef int CLAUSE_INDEX;
typedef vector<CLAUSE_INDEX> CLAUSE_INDICES;
typedef unordered_map<SYNONYM_INDEX, int> SYNONYM_COUNTER;

class QueryGroup {
public:
	QueryGroup(CLAUSE_INDICES& queryClauses);
	CLAUSE_INDICES& getClauses();

private:
	CLAUSE_INDICES queryClauses;
};

class SelectQueryGroup : public QueryGroup {
public:
	SelectQueryGroup(CLAUSE_INDICES& queryClauses, SYNONYM_COUNTER& synonymCounter, SYNONYM_INDICES& selectSynonyms);
	SYNONYM_COUNTER& getSynonymCounter();
	SYNONYM_INDICES& getSelectSynonyms();

private:
	SYNONYM_COUNTER synonymCounter;
	SYNONYM_INDICES selectSynonyms;
};

class NonSelectQueryGroup : public QueryGroup {
public:
	NonSelectQueryGroup(CLAUSE_INDICES& queryClauses, SYNONYM_COUNTER& synonymCounter);
	SYNONYM_COUNTER& getSynonymCounter();

private:
	SYNONYM_COUNTER synonymCounter;
};

class ConstantQueryGroup : public QueryGroup {
public:
	ConstantQueryGroup(CLAUSE_INDICES& queryClauses);
};
