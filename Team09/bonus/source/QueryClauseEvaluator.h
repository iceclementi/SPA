#pragma once

#include "Common.h"
#include "PKB.h"
#include "SynonymStorageManager.h"
#include "SuchThatClause.h"
#include "PatternClause.h"
#include "WithClause.h"
#include "QuerySelectResult.h"

class QueryClauseEvaluator {
public:
	virtual bool evaluateClause() = 0;

protected:
	PKB pkb;
	SynonymStorageManager synonymStore;
};

class SuchThatClauseEvaluator : public QueryClauseEvaluator {
public:
	SuchThatClauseEvaluator(SuchThatClause& clause);
	bool evaluateClause();

private:
	static S<DESIGN_ENTITY> statementDEs;

	SuchThatClause clause;
	RELATIONSHIP_TYPE relType = RELATIONSHIP_TYPE::INVALID;
	bool canHaveRelationshipToSelf = false;

	/* Initialises and prepares the evuator for evaluation */
	void init();
	/* Evaluates the Such That relationship given first parameter is not a synonym */
	bool evaluateGivenNonSynonymFirstValue(VAR firstValue, QueryArgument& secondArgument);
	/* Evaluates the Such That relationship given first parameter is a synonym */
	bool evaluateGivenSynonymFirstValue(SYNONYM_INDEX firstSynonym, DESIGN_ENTITY firstDEType, QueryArgument& secondArgument);
	/* Verifies if the clause has the given design entity type */
	bool isOfDEType(DESIGN_ENTITY deType, QueryArgument& argument);
};

class PatternClauseEvaluator : public QueryClauseEvaluator {
public:
	PatternClauseEvaluator(PatternClause& clause);
	bool evaluateClause();

private:
	PatternClause clause;
	RELATIONSHIP_TYPE relType = RELATIONSHIP_TYPE::INVALID;

	/* Initialises and prepares the evuator for evaluation */
	void init();
	/* Evaluates the Assign Pattern clause */
	bool evaluateAssignPattern();
	/* Evaluates pattern-matching for Assign statements given first parameter is either an identifier or an underscore */
	bool evaluateAssignPatternGivenNonSynonym(SYNONYM_INDEX assignSynonym, VAR_NAME varName, QueryPattern patternArgument);
	/* Evaluates the Follows relationship given first parameter is a synonym */
	bool evaluateAssignPatternGivenSynonym(SYNONYM_INDEX assignSynonym, SYNONYM_INDEX varSynonym, QueryPattern patternArgument);
	/* Evaluates the conditional (While/If) Pattern clause */
	bool evaluateConditionalPattern();
	/* Evaluates the conditional Pattern given first parameter is not a synonym */
	bool evaluateConditionalPatternGivenNonSynonym(SYNONYM_INDEX condSynonym, DESIGN_ENTITY condType, VAR_NAME varName);
	/* Evaluates the conditional Pattern given first parameter is a synonym */
	bool evaluateConditionalPatternGivenSynonym(SYNONYM_INDEX condSynonym, DESIGN_ENTITY condType, SYNONYM_INDEX varSynonym);
};

class WithClauseEvaluator : public QueryClauseEvaluator {
public:
	WithClauseEvaluator(WithClause& clause);
	bool evaluateClause();

private:
	WithClause clause;

	/* Evaluates the With clause given first parameter is an integer value */
	bool evaluateGivenIntegerFirstArg(VAR number, QueryArgument& secondArg);
	/* Evaluates the With clause given first parameter is an identifer value */
	bool evaluateGivenIdentFirstArg(VAR identifier, QueryArgument& secondArg);
	/* Evaluates the With clause given first parameter is a synonym */
	bool evaluateGivenSynonymFirstArg(SYNONYM_INDEX progSyn, QueryArgument& secondArg);
	/* Evaluates the With clause given first parameter is an attribute reference */
	bool evaluateGivenAttrRefFirstArg(SYNONYM_INDEX synRef, DESIGN_ENTITY synType, ATTRIBUTE_TYPE attrType, QueryArgument& secondArg);
	/* Evaluates the With clause given first parameter is an integer value and second parameter is an attribute reference */
	bool evaluateGivenIntegerAttrRef(VAR number, SYNONYM_INDEX synRef, DESIGN_ENTITY synType);
	/* Evaluates the With clause given first parameter is an identifier and second parameter is an attribute reference */
	bool evaluateGivenIdentAttrRef(VAR identifier, SYNONYM_INDEX synRef, DESIGN_ENTITY synType);
	/* Evaluates the With clause given first parameter is a synonym and second parameter is an attribute reference */
	bool evaluateGivenSynonymAttrRef(SYNONYM_INDEX progSyn, SYNONYM_INDEX synRef, DESIGN_ENTITY synType);
	/* Evaluates the With clause given both parameters are attribute references */
	bool evaluateGivenAttrRefAttrRef(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
	/* Evaluates the With clause given both parameters are attribute references and are both the same */
	bool evaluateGivenSameAttrRef(SYNONYM_INDEX synRef, DESIGN_ENTITY synType, ATTRIBUTE_TYPE attrType);
	/* Evaluates the With clause given both parameters are attribute references and are both of integer attribute type */
	bool evaluateGivenIntegerAttrRefs(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
	/* Evaluates the With clause given both parameters are attribute references and are both of integer attribute type and first is statement */
	bool evaluateGivenIntegerAttrRefStmt(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
	/* Evaluates the With clause given both parameters are attribute references and are both of integer attribute type and first is constant */
	bool evaluateGivenIntegerAttrRefConst(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
	/* Evaluates the With clause given both parameters are attribute references and are both of name attribute type */
	bool evaluateGivenNameAttrRefs(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
	/* Evaluates the With clause given both parameters are attribute references and are both of name attribute type and first is procedure/variable */
	bool evaluateGivenNameAttrRefProcVar(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
	/* Evaluates the With clause given both parameters are attribute references and are both of name attribute type and first is statement */
	bool evaluateGivenNameAttrRefStmt(SYNONYM_INDEX firstSynRef, DESIGN_ENTITY firstSynType, ATTRIBUTE_TYPE firstAttrType,
		SYNONYM_INDEX secondSynRef, DESIGN_ENTITY secondSynType, ATTRIBUTE_TYPE secondAttrType);
};

class SelectClauseEvaluator : public QueryClauseEvaluator {
public:
	SelectClauseEvaluator(QuerySelectResult& selectResult);
	bool evaluateClause();
	ANSWER getAnswer();

private:
	ELEMENT_LIST selectElements;
	STRING_INTERMEDIATE_ANSWER intermediateAnswer;

	/* Merges the reamining select synonym values not in the table */
	void mergeRemainingSelectSynonymValues();
	/* Initialises the intermediate answer in string form */
	void initStringIntermediateAnswer();
	/* Converts design entity to its var form */
	void convertToStringStmtVar(DESIGN_ENTITY deType, INDEX index);

	/* Formats the intermediate table into the final answer */
	ANSWER formatAnswer();
	/* Converts a row of the intermediate table into a string */
	RESULT convertToResultString(VAR_LIST& valueList);
};
