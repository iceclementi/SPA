#pragma once

#include "QueryObject.h"

using namespace std;

typedef string DECLARATION;
typedef string CLAUSE;

class QueryPreprocessor {
private:
	string query;
	list<string> queryTokens;
	SYNONYM_INDEX synonymIndex = 0;
	unordered_map<SYNONYM, SYNONYM_INDEX> declaredSynonymsToIndex;
	unordered_map<SYNONYM_INDEX, DESIGN_ENTITY> declaredSynIndexToDesignEntity;
	CLAUSE_TYPE prevClauseType = CLAUSE_TYPE::INVALID;
	CLAUSE_INDEX clauseIndex = 0;
	
	QueryObject queryObject;

	string trimLeadingWhitespace(string str);
	string trimTrailingWhitespace(string str);

	string parseName(string name);
	string parseIdentWithQuotationMarks(string ident);
	string parseInteger(string integer);
	string parseSynonym(string synonym);
	QueryElement parseAttrRef(string attrRef);
	QueryElement parseSyn(string syn);
	QueryElement parseElem(string element);
	QueryPattern parseExpressionSpec(string expressionSpec);
	string parseDesignEntity(string designEntity);

	bool isSynonymDeclared(SYNONYM synonym);
	
	QueryArgument validateArgument(ARGUMENT_TYPE argType, ARGUMENT arg);
	string extractArgument();
	list<string> extractArgumentsBetweenParentheses();
	void validateSuchThatClauseArgs(SUCH_THAT_REL_TYPE relType, list<string> argList);
	void parseSuchThatClause();
	void validatePatternClauseArgs(PatternClause pc, list<string> argList);
	void parsePatternClause();
	void validateWithClauseArgs(list<string> argList);
	void parseWithClause();
	void splitAndParseClauses();
	void parseResultClause();
	void parseDeclarations();
	void tokeniseQuery();

public:
	QueryPreprocessor(string query);
	QueryObject& parseQuery();
};
