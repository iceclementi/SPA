#pragma once

#include "Common.h"
#include "ExpressionParser.h"
#include "PKBStorage.h"
#include "RelationshipCache.h"
#include "TNode.h"

struct PKBMain;
struct PKBCache;

class PKB {
public:
	PKB();

	//////////////////////////////////////////////
	///                  AST			       ///
	//////////////////////////////////////////////

	void setASTRoot(TNode node);
	TNode getASTRoot();

	//////////////////////////////////////////////
	///               ODE-PKB			       ///
	//////////////////////////////////////////////

	void setNextStarPopulated(bool populated);
	void setAffectsPopulated(bool populated);
	void setAffectsStarPopulated(bool populated);
	void setNextBipPopulated(bool populated);
	void setNextBipStarPopulated(bool populated);
	void setAffectsBipPopulated(bool populated);
	void setAffectsBipStarPopulated(bool populated);

	//////////////////////////////////////////////
	///               Parser-PKB               ///
	//////////////////////////////////////////////

	void addStmt(DESIGN_ENTITY stmtType, STMT_NUM stmtNum);
	void addStmtVar(DESIGN_ENTITY stmtType, STMT_NUM stmtNum, VAR var);
	void addDEValue(DESIGN_ENTITY deType, VAR value);
	void addRelationship(RELATIONSHIP_TYPE rType, VAR firstValue, VAR secondValue);
	void copyValues(RELATIONSHIP_TYPE fromRType, VAR from, RELATIONSHIP_TYPE toRType, VAR to);
	void addPattern(STMT_NUM stmtNum, VAR_NAME varName, PATTERN pattern);
	void addCondVar(RELATIONSHIP_TYPE rType, STMT_NUM stmtNum, VAR_NAMES& condVars);

	//////////////////////////////////////////////
	///                PKB-PQL                 ///
	//////////////////////////////////////////////

	/*-------------------------------------*\
	|            Design Entities            |
	\*-------------------------------------*/

	BasicStorage<VAR>& getDEStoreOf(DESIGN_ENTITY deType);
	INDICES& getIndexedDEValuesOf(DESIGN_ENTITY deType);
	INDEX getIndexOfDEValue(VAR value);
	INDEX createIndexOfDEValue(VAR value);
	VAR getStringValueOfDE(INDEX index);
	int getDEStoreSizeOf(DESIGN_ENTITY deType);

	/*-------------------------------------*\
	|              Statements               |
	\*-------------------------------------*/

	StatementStorage& getStmtStore();
	INDICES& getIndexedStmtsOf(DESIGN_ENTITY stmtType);
	int getStmtStoreSize();

	/*-------------------------------------*\
	|             Relationships             |
	\*-------------------------------------*/

	/* Returns the boolean for one of Relationship (firstValue, secondValue), Relationship (firstValue, _), Relationship (_, secondValue) or Relationship (_, _) */
	bool hasRelationship(RELATIONSHIP_TYPE rType, VAR firstValue, VAR secondValue);
	/* Returns the set of first values x of type for either Relationship (x, secondValue) or Relationship (x, _) */
	VALUES getFirstValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, VAR secondValue, VALUES& tableValues);
	/* Returns the set of first values x of type for either Relationship (x, secondValue) or Relationship (x, _) */
	VALUES getFirstValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, VAR secondValue);
	/* Returns the set of second values x of type for either Relationship (firstValue, x) or Relationship (_, x)*/
	VALUES getSecondValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY secondValueType, VAR firstValue, VALUES& tableValues);
	/* Returns the set of second values x of type for either Relationship (firstValue, x) or Relationship (_, x)*/
	VALUES getSecondValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY secondValueType, VAR firstValue);
	/* Returns the set of pairs of first and second values x1 and x2 of types for Relationship (x1, x2) */
	VALUE_PAIRS getValuePairsOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType, VALUE_PAIRS& tableValuePairs);
	/* Returns the set of pairs of first and second values x1 and x2 of types for Relationship (x1, x2) */
	VALUE_PAIRS getValuePairsOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType, VALUES& tableValues, bool isFirstValues);
	/* Returns the set of pairs of first and second values x1 and x2 of types for Relationship (x1, x2) */
	VALUE_PAIRS getValuePairsOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType);
	/* Returns the set of values x (of optional statement type) for Relationship (x, x) */
	VALUES getRelationshipToSelfOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY valueType, VALUES& tableValues);
	/* Returns the set of values x (of optional statement type) for Relationship (x, x) */
	VALUES getRelationshipToSelfOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY valueType);

	RelationshipStorage<INDEX, INDEX>& getRelationshipStoreOf(RELATIONSHIP_TYPE rType);
	int getRelationshipStoreSize(RELATIONSHIP_TYPE rType);

	/*-------------------------------------*\
	|                 Cache                 |
	\*-------------------------------------*/

	/* Populates the NextStarCache if it has not been populated */
	void populateNextStarCache();
	/* Populates the NextStarCache with a single program line */
	//void populateNextStarCache(VAR progLine);
	/* Populates the AffectsCache if it has not been populated */
	void populateAffectsCache();
	/* Populates the AffectsCache with a single statement */
	//void populateAffectsCache(VAR stmtNum);
	/* Populates the AffectsStarCache if it has not been populated */
	void populateAffectsStarCache();
	/* Populates the AffectsStarCache with a single statement */
	//void populateAffectsStarCache(VAR stmtNum);
	void populateNextBipCache();
	void populateNextBipStarCache();
	void populateAffectsBipCache();
	void populateAffectsBipStarCache();

	/*-------------------------------------*\
	|            ASSIGN Pattern             |
	\*-------------------------------------*/

	/* Returns the set of assign statement numbers a for one of a ("varName", "x") or a (_, "x") or a ("varName", _"x"_) or a (_, _"x"_) */
	VALUES getAssignOfPattern(VAR_NAME varName, PATTERN pattern, bool isExactMatch, VALUES& tableValues);
	/* Returns the set of assign statement numbers a for one of a ("varName", "x") or a (_, "x") or a ("varName", _"x"_) or a (_, _"x"_) */
	VALUES getAssignOfPattern(VAR_NAME varName, PATTERN pattern, bool isExactMatch);
	/* Returns the set of assign statement numbers a for either a ("varName", _) or a (_, _) */
	VALUES getAssignWithoutPattern(VAR_NAME varName, VALUES& tableValues);
	/* Returns the set of assign statement numbers a for either a ("varName", _) or a (_, _) */
	VALUES getAssignWithoutPattern(VAR_NAME varName);
	/* Returns the set of pairs of assign statement numbers a and variables v for either a (v, "x") or a (v _"x"_) */
	VALUE_PAIRS getAssignVarOfPattern(PATTERN pattern, bool isExactMatch, VALUE_PAIRS& tableValuePairs);
	/* Returns the set of pairs of assign statement numbers a and variables v for either a (v, "x") or a (v _"x"_) */
	VALUE_PAIRS getAssignVarOfPattern(PATTERN pattern, bool isExactMatch, VALUES& tableValues, bool isAssignValues);
	/* Returns the set of pairs of assign statement numbers a and variables v for either a (v, "x") or a (v _"x"_) */
	VALUE_PAIRS getAssignVarOfPattern(PATTERN pattern, bool isExactMatch);
	/* Returns the set of pairs of assign statement numbers a and variables v for a (v, _) */
	VALUE_PAIRS getAssignVarWithoutPattern(VALUE_PAIRS& tableValuePairs);
	/* Returns the set of pairs of assign statement numbers a and variables v for a (v, _) */
	VALUE_PAIRS getAssignVarWithoutPattern(VALUES& tableValues, bool isAssignValues);
	/* Returns the set of pairs of assign statement numbers a and variables v for a (v, _) */
	VALUE_PAIRS getAssignVarWithoutPattern();
	
	RelationshipStorage<INDEX, INDEX>& getAssignVarStore();
	RelationshipStorage<INDEX, PATTERN>& getAssignPatternStore();
	int getPatternAssignTableSize();

	/*-------------------------------------*\
	|                  WITH                 |
	\*-------------------------------------*/

	/* Returns the set of values of the given design entity type */
	VALUES getValuesOf(DESIGN_ENTITY valueType, VALUES& tableValues);
	/* Returns the set of values of the given design entity type */
	VALUES getValuesOf(DESIGN_ENTITY valueType);
	/* Returns the set of values of the given design entity type and value */
	VALUES getValuesOf(DESIGN_ENTITY valueType, VAR value, VALUES& tableValues);
	/* Returns the set of values of the given design entity type and value */
	VALUES getValuesOf(DESIGN_ENTITY valueType, VAR value);
	/* Returns the set of pairs of matching values of the given design entity types */
	VALUE_PAIRS getValuePairsOf(DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType, VALUE_PAIRS& tableValuePairs);
	/* Returns the set of pairs of matching values of the given design entity types */
	VALUE_PAIRS getValuePairsOf(DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType, VALUES& tableValues);
	/* Returns the set of pairs of matching values of the given design entity types */
	VALUE_PAIRS getValuePairsOf(DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType);

	/* Returns the set of statement numbers of the given statement type and interacts with the given variable */
	VALUES getStmtsWithVar(DESIGN_ENTITY stmtType, VAR var, VALUES& tableValues);
	/* Returns the set of statement numbers of the given statement type and interacts with the given variable */
	VALUES getStmtsWithVar(DESIGN_ENTITY stmtType, VAR var);
	/* Returns the set of pairs of statement numbers (read, print, call) with matching vars */
	VALUE_PAIRS getStmtVarPairsOf(DESIGN_ENTITY firstStmtType, DESIGN_ENTITY secondStmtType, VALUE_PAIRS& tableValuePairs);
	/* Returns the set of pairs of statement numbers (read, print, call) with matching vars */
	VALUE_PAIRS getStmtVarPairsOf(DESIGN_ENTITY firstStmtType, DESIGN_ENTITY secondStmtType, VALUES& tableValuePairs, bool isFirstValues);
	/* Returns the set of pairs of statement numbers (read, print, call) with matching vars */
	VALUE_PAIRS getStmtVarPairsOf(DESIGN_ENTITY firstStmtType, DESIGN_ENTITY secondStmtType);
	/* Returns the set of pairs of statement numbers (read, print, call) that matches the vars (procedures, variables) */
	VALUE_PAIRS getStmtVarVarPairsOf(DESIGN_ENTITY stmtType, DESIGN_ENTITY varType, VALUE_PAIRS& tableValuePairs);
	/* Returns the set of pairs of statement numbers (read, print, call) that matches the vars (procedures, variables) */
	VALUE_PAIRS getStmtVarVarPairsOf(DESIGN_ENTITY stmtType, DESIGN_ENTITY varType, VALUES& tableValuePairs, bool isStmtValues);
	/* Returns the set of pairs of statement numbers (read, print, call) that matches the vars (procedures, variables) */
	VALUE_PAIRS getStmtVarVarPairsOf(DESIGN_ENTITY stmtType, DESIGN_ENTITY varType);
	

	/*-------------------------------------*\
	|                 SELECT                |
	\*-------------------------------------*/

	VAR getStringStmtVarOf(DESIGN_ENTITY stmtType, STMT_NUM stmtNum);

	static void clear();
	static void clearCache();

private:
	static TNode astRoot;

	static StatementStorage stmtStore;
	static BasicStorage<PROC_NAME> procStore;
	static BasicStorage<VAR_NAME> varStore;
	static BasicStorage<CONST> constStore;
	static PKBMain pkbMain;
	static PKBCache pkbCache;

	BasicStorage<VAR> emptyDEStore;
	RelationshipStorage<INDEX, INDEX> emptyRelationshipStore;

	bool isOfDEType(DESIGN_ENTITY deType, INDEX indexedVar);
	bool isOfDEType(DESIGN_ENTITY deType, VAR var);

	INDICES emptyIndices;
};

struct PKBMain {
	RelationshipStorage<INDEX, INDEX> fStore, fStarStore, pStore, pStarStore, uStmtStore, uProcStore, mStmtStore, mProcStore, cStore, cStarStore, nStore;

	AssignPatternStorage aPatStore;
	RelationshipStorage<INDEX, INDEX> wPatStore, iPatStore;
};

struct PKBCache {
	RelationshipCache nStarCache = RelationshipCache(RELATIONSHIP_TYPE::NEXT_STAR);
	RelationshipCache aCache = RelationshipCache(RELATIONSHIP_TYPE::AFFECTS);
	RelationshipCache aStarCache = RelationshipCache(RELATIONSHIP_TYPE::AFFECTS_STAR);
	RelationshipBipCache nBipCache = RelationshipBipCache(RELATIONSHIP_TYPE::NEXT_BIP);
	RelationshipBipCache nBipStarCache = RelationshipBipCache(RELATIONSHIP_TYPE::NEXT_BIP_STAR);
	RelationshipCache aBipCache = RelationshipCache(RELATIONSHIP_TYPE::AFFECTS_BIP);
	RelationshipCache aBipStarCache = RelationshipCache(RELATIONSHIP_TYPE::AFFECTS_BIP_STAR);
};
