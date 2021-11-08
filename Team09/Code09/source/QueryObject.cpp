#include "QueryObject.h"

QueryObject::QueryObject() {}

QuerySelectResult& QueryObject::getSelectResult() {
	return selectResult;
}

void QueryObject::setSelectResult(QuerySelectResult& result) {
	selectResult = result;
}

CLAUSE_TYPE QueryObject::getClauseTypeOf(CLAUSE_INDEX index) {
	return clauseIndexToType.count(index) ? clauseIndexToType[index] : CLAUSE_TYPE::INVALID;
}

unordered_map<CLAUSE_INDEX, QueryClause>& QueryObject::getIndexedQueryClauses() {
	return clauseIndexToClause;
}

SuchThatClause& QueryObject::getSuchThatClauseOf(CLAUSE_INDEX index) {
	// Possible throw error if not found?
	return clauseIndexToSuchThatClause[index];
}

PatternClause& QueryObject::getPatternClauseOf(CLAUSE_INDEX index) {
	// Possible throw error if not found?
	return clauseIndexToPatternClause[index];
}

WithClause& QueryObject::getWithClauseOf(CLAUSE_INDEX index) {
	// Possible throw error if not found?
	return clauseIndexToWithClause[index];
}

void QueryObject::addSuchThatClause(SuchThatClause& clause, CLAUSE_INDEX clauseIndex) {
	clauseIndexToType[clauseIndex] = CLAUSE_TYPE::SUCH_THAT;
	clauseIndexToClause[clauseIndex] = clause;
	clauseIndexToSuchThatClause[clauseIndex] = clause;
}

void QueryObject::addPatternClause(PatternClause& clause, CLAUSE_INDEX clauseIndex) {
	clauseIndexToType[clauseIndex] = CLAUSE_TYPE::PATTERN;
	clauseIndexToClause[clauseIndex] = clause;
	clauseIndexToPatternClause[clauseIndex] = clause;
}

void QueryObject::addWithClause(WithClause& clause, CLAUSE_INDEX clauseIndex) {
	clauseIndexToType[clauseIndex] = CLAUSE_TYPE::WITH;
	clauseIndexToClause[clauseIndex] = clause;
	clauseIndexToWithClause[clauseIndex] = clause;
}

vector<SelectQueryGroup>& QueryObject::getSelectQueryGroups() {
	return selectQueryGroups;
}

vector<NonSelectQueryGroup>& QueryObject::getNonSelectQueryGroups() {
	return nonSelectQueryGroups;
}

vector<ConstantQueryGroup>& QueryObject::getConstantQueryGroups() {
	return constantQueryGroups;
}

void QueryObject::addSelectQueryGroup(SelectQueryGroup& queryGroup) {
	selectQueryGroups.push_back(queryGroup);
}

void QueryObject::addNonSelectQueryGroup(NonSelectQueryGroup& queryGroup) {
	nonSelectQueryGroups.push_back(queryGroup);
}

void QueryObject::addConstantQueryGroup(ConstantQueryGroup& queryGroup) {
	constantQueryGroups.push_back(queryGroup);
}

bool QueryObject::hasError() {
	return errorMessage == SYNTAX_ERROR ||
		errorMessage == SEMANTIC_ERROR;
}

bool QueryObject::hasSemanticError() {
	return errorMessage == SEMANTIC_ERROR;
}

void QueryObject::handleError() {
	// Clear storage to facilitate testing
	if (selectResult.getSelectBoolean()) {
		selectResult = QuerySelectResult();
		selectResult.setSelectBoolean(true);
	}
	else {
		selectResult = QuerySelectResult();
	}
	clauseIndexToType.clear();
	clauseIndexToClause.clear();
	clauseIndexToSuchThatClause.clear();
	clauseIndexToPatternClause.clear();
	clauseIndexToWithClause.clear();
}

unordered_map<CLAUSE_INDEX, CLAUSE_TYPE> QueryObject::getClauseIndexToType() {
	return clauseIndexToType;
}

unordered_map<CLAUSE_INDEX, SuchThatClause> QueryObject::getClauseIndexToSuchThatClause() {
	return clauseIndexToSuchThatClause;
}

unordered_map<CLAUSE_INDEX, PatternClause> QueryObject::getClauseIndexToPatternClause() {
	return clauseIndexToPatternClause;
}

unordered_map<CLAUSE_INDEX, WithClause> QueryObject::getClauseIndexToWithClause() {
	return clauseIndexToWithClause;
}
