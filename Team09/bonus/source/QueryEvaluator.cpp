#include "QueryEvaluator.h"

/*
* In AutoTester (SPA)
* Empty result is returned by not populating the list of strings in the
* TestWrapper (evaluate function) return an empty list of stings
* Invalid queries should also return an empty result
*/

QueryEvaluator::QueryEvaluator(QueryObject& query) : query(query) {}

ANSWER QueryEvaluator::evaluateQuery() {
    // Invalid Query. Return Empty result.
    if (query.hasError()) {
        if (query.hasSemanticError() && query.getSelectResult().getSelectBoolean()) {
            return ANSWER({ FALSE });
        }
        else {
            return ANSWER();
        }
    }

    synonymStore.clear();

    if (evaluateConstantQueryGroups() && evalauteNonSelectQueryGroups() && evaluateSelectQueryGroups()) {
        return evaluateSelectClause(query.getSelectResult());
    }
    
    return generateNoAnswer();
}

bool QueryEvaluator::evaluateSelectQueryGroups() {
    for (auto& queryGroup : query.getSelectQueryGroups()) {
        synonymStore.setSynonymStorage(SELECT_STORAGE(queryGroup.getSynonymCounter(), queryGroup.getSelectSynonyms()));
        if (!evaluateQueryGroup(queryGroup)) {
            return false;
        }
        synonymStore.mergeToResult();
    }
    return true;
}

bool QueryEvaluator::evalauteNonSelectQueryGroups() {
    for (auto& queryGroup : query.getNonSelectQueryGroups()) {
        synonymStore.setSynonymStorage(TEMP_STORAGE(queryGroup.getSynonymCounter()));
        if (!evaluateQueryGroup(queryGroup)) {
            return false;
        }
    }
    return true;
}

bool QueryEvaluator::evaluateConstantQueryGroups() {
    for (auto& queryGroup : query.getConstantQueryGroups()) {
        if (!evaluateQueryGroup(queryGroup)) {
            return false;
        }
    }
    return true;
}

bool QueryEvaluator::evaluateQueryGroup(QueryGroup& queryGroup) {
    for (auto& clauseIndex : queryGroup.getClauses()) {
        if (!evaluateQueryClause(clauseIndex)) {
            return false;
        }
    }
    return true;
}

bool QueryEvaluator::evaluateQueryClause(CLAUSE_INDEX clauseIndex) {
    switch (query.getClauseTypeOf(clauseIndex)) {
    case CLAUSE_TYPE::SUCH_THAT:
        return evaluateSuchThatClause(query.getSuchThatClauseOf(clauseIndex));
    case CLAUSE_TYPE::PATTERN:
        return evaluatePatternClause(query.getPatternClauseOf(clauseIndex));
    case CLAUSE_TYPE::WITH:
        return evaluateWithClause(query.getWithClauseOf(clauseIndex));
    default:
        return false;
    }
}

bool QueryEvaluator::evaluateSuchThatClause(SuchThatClause& clause) {
    return SuchThatClauseEvaluator(clause).evaluateClause();
}

bool QueryEvaluator::evaluatePatternClause(PatternClause& clause) {
    return PatternClauseEvaluator(clause).evaluateClause();
}

bool QueryEvaluator::evaluateWithClause(WithClause& clause) {
    return WithClauseEvaluator(clause).evaluateClause();
}

ANSWER QueryEvaluator::evaluateSelectClause(QuerySelectResult& result) {
    synonymStore.setSynonymStorage(query.getSelectResult().getSelectElementList());
    SelectClauseEvaluator selectClauseEvaluator(result);
    selectClauseEvaluator.evaluateClause();
    return selectClauseEvaluator.getAnswer();
}

ANSWER QueryEvaluator::generateNoAnswer() {
    if (query.getSelectResult().getSelectSynonyms().empty()) {
        return ANSWER({ FALSE });
    }
    else {
        return ANSWER();
    }
}

