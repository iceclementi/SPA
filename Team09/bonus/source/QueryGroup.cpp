#include "QueryGroup.h"
#include <iostream>
QueryGroup::QueryGroup(CLAUSE_INDICES& queryClauses) : queryClauses(queryClauses) {}

CLAUSE_INDICES& QueryGroup::getClauses() {
	return queryClauses;
}

SelectQueryGroup::SelectQueryGroup(CLAUSE_INDICES& queryClauses, SYNONYM_COUNTER& synonymCounter, SYNONYM_INDICES& selectSynonyms)
	: QueryGroup(queryClauses), synonymCounter(synonymCounter), selectSynonyms(selectSynonyms) {}

SYNONYM_COUNTER& SelectQueryGroup::getSynonymCounter() {
	return synonymCounter;
}

SYNONYM_INDICES& SelectQueryGroup::getSelectSynonyms() {
	return selectSynonyms;
}

NonSelectQueryGroup::NonSelectQueryGroup(CLAUSE_INDICES& queryClauses, SYNONYM_COUNTER& synonymCounter)
	: QueryGroup(queryClauses), synonymCounter(synonymCounter) {}

SYNONYM_COUNTER& NonSelectQueryGroup::getSynonymCounter() {
	return synonymCounter;
}

ConstantQueryGroup::ConstantQueryGroup(CLAUSE_INDICES& queryClauses) : QueryGroup(queryClauses) {}
