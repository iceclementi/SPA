#pragma once

#include "Common.h"
#include "QueryObject.h"
#include "OptimiserUtility.h"

typedef pair<int, int> WEIGHTED_CLAUSE_INDICE;
typedef vector<WEIGHTED_CLAUSE_INDICE> WEIGHTED_CLAUSE_INDICES;
class QueryOptimiser {
public:
	QueryOptimiser(QueryObject& query);
	QueryObject& optimiseQuery();
	QueryObject& noOptimiseQuery();

private:
	void createConstantQueryGroup();
	void createSelectQueryGroups();
	void createNonSelectQueryGroups();
	void createConnectedQueryGroup(SYNONYM_INDEX startSynonym, bool isSelectQueryGroup);
	int getPriorityOfNoSynonymClause(CLAUSE_INDEX clauseIndex);
	int getPriorityOfhasSynonymClause(CLAUSE_INDEX clauseIndex, int numOfSynonyms);
	void orderClausesOfSynonymToClausesMap();

	QueryObject queryObject;
	SYNONYM_INDICES selectSynonyms;
	unordered_map<CLAUSE_INDEX, QueryClause> clauseIndexToClause;
	unordered_map<SYNONYM_INDEX, unordered_set<CLAUSE_INDEX>> synonymToClauses;
};
