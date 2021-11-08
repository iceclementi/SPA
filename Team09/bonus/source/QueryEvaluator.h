#pragma once

#include "Common.h"
#include "PKB.h"
#include "QueryObject.h"
#include "SynonymStorageManager.h"
#include "QueryClauseEvaluator.h"

class QueryEvaluator {
public:	
	QueryEvaluator(QueryObject& query);
	ANSWER evaluateQuery();

private:
	QueryObject query;
	PKB pkb;
	SynonymStorageManager synonymStore;

	bool evaluateSelectQueryGroups();
	bool evalauteNonSelectQueryGroups();
	bool evaluateConstantQueryGroups();

	bool evaluateQueryGroup(QueryGroup& queryGroup);
	bool evaluateQueryClause(CLAUSE_INDEX clauseIndex);

	bool evaluateSuchThatClause(SuchThatClause& clause);
	bool evaluatePatternClause(PatternClause& clause);
	bool evaluateWithClause(WithClause& clause);
	ANSWER evaluateSelectClause(QuerySelectResult& result);

	ANSWER generateNoAnswer();
};
