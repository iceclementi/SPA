#pragma once

#include "QueryGroup.h"
#include "QuerySelectResult.h"
#include "PatternClause.h"
#include "SuchThatClause.h"
#include "WithClause.h"

using namespace std;

/*
* Represents information extracted from a query to be passed to QueryEvaluator
*/
class QueryObject {
public:
	QueryObject();

	string errorMessage;

	/* Gets the results to be selected */
	QuerySelectResult& getSelectResult();
	/* Sets the results to be selected */
	void setSelectResult(QuerySelectResult& selectResult);

	/* Gets the clause type of the clause at the given clause index */
	CLAUSE_TYPE getClauseTypeOf(CLAUSE_INDEX index);
	/* Gets all the tagged Query clauses */
	unordered_map<CLAUSE_INDEX, QueryClause>& getIndexedQueryClauses();
	/* Gets the Such That clause at the given clause index */
	SuchThatClause& getSuchThatClauseOf(CLAUSE_INDEX index);
	/* Gets the Pattern clause at the given clause index */
	PatternClause& getPatternClauseOf(CLAUSE_INDEX index);
	/* Gets the With clause at the given clause index */
	WithClause& getWithClauseOf(CLAUSE_INDEX index);
	
	/* Adds a Such That clause tagged with a clause index */
	void addSuchThatClause(SuchThatClause& clause, CLAUSE_INDEX clauseIndex);
	/* Adds a Pattern clause tagged with a clause index */
	void addPatternClause(PatternClause& clause, CLAUSE_INDEX clauseIndex);
	/* Adds a With clause tagged with a clause index */
	void addWithClause(WithClause& clause, CLAUSE_INDEX clauseIndex);

	/* Gets all of the Select Query Groups */
	vector<SelectQueryGroup>& getSelectQueryGroups();
	/* Gets all of the Non Select Query Groups */
	vector<NonSelectQueryGroup>& getNonSelectQueryGroups();
	/* Gets all of the Constant Query Groups */
	vector<ConstantQueryGroup>& getConstantQueryGroups();

	/* Adds a Select Query Group into an internal list */
	void addSelectQueryGroup(SelectQueryGroup& queryGroup);
	/* Adds a Non Select Query Group into an internal list */
	void addNonSelectQueryGroup(NonSelectQueryGroup& queryGroup);
	/* Adds a Constant Query Group into an internal list */
	void addConstantQueryGroup(ConstantQueryGroup& queryGroup);

	/* Error handling */
	bool hasError();
	bool hasSemanticError();
	void handleError();

	/* These methods are only meant for unit testing of query preprocessor. */
	unordered_map<CLAUSE_INDEX, CLAUSE_TYPE> getClauseIndexToType();
	unordered_map<CLAUSE_INDEX, SuchThatClause> getClauseIndexToSuchThatClause();
	unordered_map<CLAUSE_INDEX, PatternClause> getClauseIndexToPatternClause();
	unordered_map<CLAUSE_INDEX, WithClause> getClauseIndexToWithClause();

private:
	/* Result to be selected */
	QuerySelectResult selectResult;

	/* Clause index to clause conversion */
	unordered_map<CLAUSE_INDEX, CLAUSE_TYPE> clauseIndexToType;
	unordered_map<CLAUSE_INDEX, QueryClause> clauseIndexToClause;
	unordered_map<CLAUSE_INDEX, SuchThatClause> clauseIndexToSuchThatClause;
	unordered_map<CLAUSE_INDEX, PatternClause> clauseIndexToPatternClause;
	unordered_map<CLAUSE_INDEX, WithClause> clauseIndexToWithClause;

	/* Query Groups */
	vector<SelectQueryGroup> selectQueryGroups;
	vector<NonSelectQueryGroup> nonSelectQueryGroups;
	vector<ConstantQueryGroup> constantQueryGroups;
};

/* Used by QueryPreprocessor for unit testing purpose */
inline
bool operator== (QueryObject lhs, QueryObject rhs) {
	return lhs.errorMessage == rhs.errorMessage &&
		lhs.getSelectResult() == rhs.getSelectResult() &&
		lhs.getClauseIndexToType() == rhs.getClauseIndexToType() &&
		lhs.getClauseIndexToSuchThatClause() == rhs.getClauseIndexToSuchThatClause() &&
		lhs.getClauseIndexToPatternClause() == rhs.getClauseIndexToPatternClause() &&
		lhs.getClauseIndexToWithClause() == rhs.getClauseIndexToWithClause();
}
