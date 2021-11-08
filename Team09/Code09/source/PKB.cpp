#include "PKB.h"

#pragma region Declarations

TNode PKB::astRoot(TNode::NODE_TYPE::program);

BasicStorage<PROC_NAME> PKB::procStore;
BasicStorage<VAR_NAME> PKB::varStore;
BasicStorage<CONST> PKB::constStore;
StatementStorage PKB::stmtStore;

PKBMain PKB::pkbMain;
PKBCache PKB::pkbCache;

#pragma endregion

PKB::PKB() {}

#pragma region AST

void PKB::setASTRoot(TNode node) {
	astRoot = node;
}

TNode PKB::getASTRoot() {
	return astRoot;
}

#pragma endregion

#pragma region ODE-PKB

void PKB::setNextStarPopulated(bool populated) {
	pkbCache.nStarCache.setPopulated(populated);
}

void PKB::setAffectsPopulated(bool populated) {
	pkbCache.aCache.setPopulated(populated);
}

void PKB::setAffectsStarPopulated(bool populated) {
	pkbCache.aStarCache.setPopulated(populated);
}

#pragma endregion

#pragma region Parser-PKB

void PKB::addStmt(DESIGN_ENTITY stmtType, STMT_NUM stmtNum) {
	stmtStore.addStmt(stmtType, stmtNum);
}

void PKB::addStmtVar(DESIGN_ENTITY stmtType, STMT_NUM stmtNum, VAR var) {
	switch (stmtType) {
	case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::CALL:
		stmtStore.addStmtVar(stmtType, stmtNum, var);
		break;
	default:
		return;
	}
}

void PKB::addDEValue(DESIGN_ENTITY deType, VAR value) {
	switch (deType) {
	case DESIGN_ENTITY::PROCEDURE:
		procStore.addValue(value);
		break;
	case DESIGN_ENTITY::VARIABLE:
		varStore.addValue(value);
		break;
	case DESIGN_ENTITY::CONSTANT:
		constStore.addValue(value);
		break;
	default:
		return;
	}
}

void PKB::addRelationship(RELATIONSHIP_TYPE rType, VAR firstValue, VAR secondValue) {
	auto& relStore = getRelationshipStoreOf(rType);
	relStore.addRelationship(createIndexOfDEValue(firstValue), createIndexOfDEValue(secondValue));
}

void PKB::copyValues(RELATIONSHIP_TYPE fromRType, VAR from, RELATIONSHIP_TYPE toRType, VAR to) {
	auto& fromStore = getRelationshipStoreOf(fromRType);
	auto& toStore = getRelationshipStoreOf(toRType);
	INDEX fromIndex = createIndexOfDEValue(from), toIndex = createIndexOfDEValue(to);
	for (auto& secondValue : fromStore.getSecondValuesOf(fromIndex)) {
		toStore.addRelationship(toIndex, secondValue);
	}
}

void PKB::addPattern(STMT_NUM stmtNum, VAR_NAME varName, PATTERN pattern) {
	pkbMain.aPatStore.addPattern(createIndexOfDEValue(stmtNum), createIndexOfDEValue(varName), pattern);
}

void PKB::addCondVar(RELATIONSHIP_TYPE rType, STMT_NUM stmtNum, VAR_NAMES& condVars) {
	if (rType != RELATIONSHIP_TYPE::WHILE_PATTERN && rType != RELATIONSHIP_TYPE::IF_PATTERN) return;
	if (condVars.empty()) return;
	auto& condVarStore = getRelationshipStoreOf(rType);
	INDEX stmtIndex = createIndexOfDEValue(stmtNum);
	for (auto& varName : condVars) {
		condVarStore.addRelationship(stmtIndex, createIndexOfDEValue(varName));
	}
}

#pragma endregion

#pragma region PKB-QPS

#pragma region Design Entities

BasicStorage<VAR>& PKB::getDEStoreOf(DESIGN_ENTITY deType) {
	switch (deType) {
	case DESIGN_ENTITY::PROCEDURE:
		return procStore;
	case DESIGN_ENTITY::VARIABLE:
		return varStore;
	case DESIGN_ENTITY::CONSTANT:
		return constStore;
	default:
		return emptyDEStore;
	}
}

INDICES& PKB::getIndexedDEValuesOf(DESIGN_ENTITY deType) {
	switch (deType) {
	case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE: case DESIGN_ENTITY::CONSTANT:
		return getDEStoreOf(deType).getIndexedValues();
	case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
	case DESIGN_ENTITY::ASSIGN: case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::WHILE: 
	case DESIGN_ENTITY::IF:
		return stmtStore.getIndexedStmtsOf(deType);
	case DESIGN_ENTITY::PROG_LINE:
		return stmtStore.getIndexedStmtsOf(DESIGN_ENTITY::STATEMENT);
	default:
		return emptyIndices;
	}
}

INDEX PKB::getIndexOfDEValue(VAR value) {
	return PKBStorage::getIndexOf(value);
}

INDEX PKB::createIndexOfDEValue(VAR value) {
	return PKBStorage::createIndexOf(value);
}

VAR PKB::getStringValueOfDE(INDEX indexedValue) {
	return PKBStorage::getStringValueOf(indexedValue);
}

int PKB::getDEStoreSizeOf(DESIGN_ENTITY deType) {
	return getDEStoreOf(deType).getTableSize();
}

#pragma endregion

#pragma region Statements

INDICES& PKB::getIndexedStmtsOf(DESIGN_ENTITY stmtType) {
	return stmtStore.getIndexedStmtsOf(stmtType);
}

int PKB::getStmtStoreSize() {
	return stmtStore.getTableSize();
}

#pragma endregion

#pragma region Relationships

RelationshipStorage<INDEX, INDEX>& PKB::getRelationshipStoreOf(RELATIONSHIP_TYPE rType) {
	switch (rType) {
	case RELATIONSHIP_TYPE::FOLLOWS:
		return pkbMain.fStore;
	case RELATIONSHIP_TYPE::FOLLOWS_STAR:
		return pkbMain.fStarStore;
	case RELATIONSHIP_TYPE::PARENT:
		return pkbMain.pStore;
	case RELATIONSHIP_TYPE::PARENT_STAR:
		return pkbMain.pStarStore;
	case RELATIONSHIP_TYPE::USES_STMT:
		return pkbMain.uStmtStore;
	case RELATIONSHIP_TYPE::USES_PROC:
		return pkbMain.uProcStore;
	case RELATIONSHIP_TYPE::MODIFIES_STMT:
		return pkbMain.mStmtStore;
	case RELATIONSHIP_TYPE::MODIFIES_PROC:
		return pkbMain.mProcStore;
	case RELATIONSHIP_TYPE::CALLS:
		return pkbMain.cStore;
	case RELATIONSHIP_TYPE::CALLS_STAR:
		return pkbMain.cStarStore;
	case RELATIONSHIP_TYPE::NEXT:
		return pkbMain.nStore;
	case RELATIONSHIP_TYPE::NEXT_STAR:
		return pkbCache.nStarCache;
	case RELATIONSHIP_TYPE::AFFECTS:
		return pkbCache.aCache;
	case RELATIONSHIP_TYPE::AFFECTS_STAR:
		return pkbCache.aStarCache;
	case RELATIONSHIP_TYPE::WHILE_PATTERN:
		return pkbMain.wPatStore;
	case RELATIONSHIP_TYPE::IF_PATTERN:
		return pkbMain.iPatStore;
	default:
		return emptyRelationshipStore;
	}
}

bool PKB::hasRelationship(RELATIONSHIP_TYPE rType, VAR firstValue, VAR secondValue) {
	auto& relStore = getRelationshipStoreOf(rType);
	INDEX firstIndex = getIndexOfDEValue(firstValue), secondIndex = getIndexOfDEValue(secondValue);
	if (!firstValue.empty()) {
		return (firstIndex && !secondValue.empty()) 
			? relStore.hasRelationship(firstIndex, secondIndex)
			: relStore.hasRelationshipWithFirstValue(firstIndex);
	}
	if (!secondValue.empty()) {
		return secondIndex ? relStore.hasRelationshipWithSecondValue(secondIndex) : false;
	}
	return !relStore.getFirstValues().empty();
}

VALUES PKB::getFirstValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, 
	VAR secondValue, VALUES& tableValues) {
	if (tableValues.empty()) {
		return getFirstValuesOf(rType, firstValueType, secondValue);
	}

	auto& relStore = getRelationshipStoreOf(rType);
	INDEX secondIndex = getIndexOfDEValue(secondValue);
	VALUES firstValues;
	if (!secondValue.empty()) {
		if (!secondIndex) return VALUES(); // Index does not exist
		for (auto& firstValue : tableValues) {
			if (isOfDEType(firstValueType, firstValue) 
				&& relStore.hasRelationship(firstValue, secondIndex)) {
				firstValues.insert(firstValue);
			}
		}
	}
	else {
		for (auto& firstIndex : tableValues) {
			if (isOfDEType(firstValueType, firstIndex) 
				&& relStore.hasRelationshipWithFirstValue(firstIndex)) {
				firstValues.insert(firstIndex);
			}
		}
	}
	return firstValues;
}

VALUES PKB::getFirstValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, VAR secondValue) {
	auto& relStore = getRelationshipStoreOf(rType);
	INDEX secondIndex = getIndexOfDEValue(secondValue);
	VALUES firstValues;
	if (!secondValue.empty()) {
		if (!secondIndex) return VALUES(); // Index does not exist
		for (auto& firstValue : relStore.getFirstValuesOf(secondIndex)) {
			if (isOfDEType(firstValueType, firstValue)) {
				firstValues.insert(firstValue);
			}
		}
	}
	else {
		for (auto& firstIndex : relStore.getFirstValues()) {
			if (isOfDEType(firstValueType, firstIndex)) {
				firstValues.insert(firstIndex);
			}
		}
	}
	return firstValues;
}

VALUES PKB::getSecondValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY secondValueType, 
	VAR firstValue, VALUES& tableValues) {
	if (tableValues.empty()) {
		return getSecondValuesOf(rType, secondValueType, firstValue);
	}

	auto& relStore = getRelationshipStoreOf(rType);
	INDEX firstIndex = getIndexOfDEValue(firstValue);
	VALUES secondValues;
	if (!firstValue.empty()) {
		if (!firstIndex) return VALUES(); // Index does not exist
		for (auto& secondValue : tableValues) {
			if (isOfDEType(secondValueType, secondValue) 
				&& relStore.hasRelationship(firstIndex, secondValue)) {
				secondValues.insert(secondValue);
			}
		}
	}
	else {
		for (auto& secondIndex : tableValues) {
			if (isOfDEType(secondValueType, secondIndex) 
				&& relStore.hasRelationshipWithSecondValue(secondIndex)) {
				secondValues.insert(secondIndex);
			}
		}
	}
	return secondValues;
}

VALUES PKB::getSecondValuesOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY secondValueType, VAR firstValue) {
	auto& relStore = getRelationshipStoreOf(rType);
	INDEX firstIndex = getIndexOfDEValue(firstValue);
	VALUES secondValues;
	if (!firstValue.empty()) {
		if (!firstIndex) return VALUES(); // Index does not exist
		for (auto& secondValue : relStore.getSecondValuesOf(firstIndex)) {
			if (isOfDEType(secondValueType, secondValue)) {
				secondValues.insert(secondValue);
			}
		}
	}
	else {
		for (auto& secondIndex : relStore.getSecondValues()) {
			if (isOfDEType(secondValueType, secondIndex)) {
				secondValues.insert(secondIndex);
			}
		}
	}
	return secondValues;
}

VALUE_PAIRS PKB::getValuePairsOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, 
	DESIGN_ENTITY secondValueType, VALUE_PAIRS& tableValuePairs) {
	if (tableValuePairs.empty()) {
		return getValuePairsOf(rType, firstValueType, secondValueType);
	}

	auto& relStore = getRelationshipStoreOf(rType);
	VALUE_PAIRS valuePairs;
	for (auto& valuePair : tableValuePairs) {
		if (isOfDEType(firstValueType, valuePair.first) && isOfDEType(secondValueType, valuePair.second)
			&& relStore.hasRelationship(valuePair.first, valuePair.second)) {
			valuePairs.insert(valuePair);
		}
	}
	return valuePairs;
}

VALUE_PAIRS PKB::getValuePairsOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, 
	DESIGN_ENTITY secondValueType, VALUES& tableValues, bool isFirstValues) {
	if (tableValues.empty()) {
		return getValuePairsOf(rType, firstValueType, secondValueType);
	}

	auto& relStore = getRelationshipStoreOf(rType);
	VALUE_PAIRS valuePairs;
	if (isFirstValues) {
		for (auto& firstValue : tableValues) {
			if (isOfDEType(firstValueType, firstValue)) {
				for (auto& secondValue : relStore.getSecondValuesOf(firstValue)) {
					if (isOfDEType(secondValueType, secondValue)) {
						valuePairs.insert({ firstValue, secondValue });
					}
				}
			}
		}
	}
	else {
		for (auto& secondValue : tableValues) {
			if (isOfDEType(secondValueType, secondValue)) {
				for (auto& firstValue : relStore.getFirstValuesOf(secondValue)) {
					if (isOfDEType(firstValueType, firstValue)) {
						valuePairs.insert({ firstValue, secondValue });
					}
				}
			}
		}
	}
	return valuePairs;
}

VALUE_PAIRS PKB::getValuePairsOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY firstValueType, 
	DESIGN_ENTITY secondValueType) {
	auto& relStore = getRelationshipStoreOf(rType);
	VALUE_PAIRS valuePairs;
	for (auto& firstValue : relStore.getFirstValues()) {
		if (isOfDEType(firstValueType, firstValue)) {
			for (auto& secondValue : relStore.getSecondValuesOf(firstValue)) {
				if (isOfDEType(secondValueType, secondValue)) {
					valuePairs.insert({ firstValue, secondValue });
				}
			}
		}
	}
	return valuePairs;
}

VALUES PKB::getRelationshipToSelfOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY valueType, 
	VALUES& tableValues) {
	if (tableValues.empty()) {
		return getRelationshipToSelfOf(rType, valueType);
	}

	auto& relStore = getRelationshipStoreOf(rType);
	VALUES values;
	for (auto& value : tableValues) {
		if (isOfDEType(valueType, value) && relStore.hasRelationship(value, value)) {
			values.insert(value);
		}
	}
	return values;
}

VALUES PKB::getRelationshipToSelfOf(RELATIONSHIP_TYPE rType, DESIGN_ENTITY valueType) {
	auto& relStore = getRelationshipStoreOf(rType);
	VALUES values;
	for (auto& value : relStore.getFirstValues()) {
		if (isOfDEType(valueType, value) && relStore.hasRelationship(value, value)) {
			values.insert(value);
		}
	}
	return values;
}

int PKB::getRelationshipStoreSize(RELATIONSHIP_TYPE rType) {
	return getRelationshipStoreOf(rType).getTableSize();
}

#pragma endregion

#pragma region Relationship Cache

void PKB::populateNextStarCache() {
	pkbCache.nStarCache.populateCache();
}

void PKB::populateAffectsCache() {
	pkbCache.aCache.populateCache();
}

void PKB::populateAffectsStarCache() {
	pkbCache.aCache.populateCache();
	pkbCache.aStarCache.populateCache();
}

#pragma endregion

#pragma region Assign Pattern

VALUES PKB::getAssignOfPattern(VAR_NAME varName, PATTERN pattern, bool isExactMatch, 
	VALUES& tableValues) {
	if (tableValues.empty()) {
		return getAssignOfPattern(varName, pattern, isExactMatch);
	}

	PATTERN parsedPattern = RPNConverter::convert(pattern);
	if (parsedPattern.empty()) return VALUES();

	VALUES assignStmtNums;
	if (varName.empty()) {
		for (auto& assignStmtNum : tableValues) {
			if (pkbMain.aPatStore.matchesPatternIn(assignStmtNum, parsedPattern, isExactMatch)) {
				assignStmtNums.insert(assignStmtNum);
			}
		}
	}
	else {
		INDEX varIndex = getIndexOfDEValue(varName);
		if (!varIndex) return VALUES();
		for (auto& assignStmtNum : tableValues) {
			if (pkbMain.aPatStore.matchesVariableIn(assignStmtNum, varIndex) &&
				pkbMain.aPatStore.matchesPatternIn(assignStmtNum, parsedPattern, isExactMatch)) {
				assignStmtNums.insert(assignStmtNum);
			}
		}
	}
	return assignStmtNums;
}

VALUES PKB::getAssignOfPattern(VAR_NAME varName, PATTERN pattern, bool isExactMatch) {
	PATTERN parsedPattern = RPNConverter::convert(pattern);
	if (parsedPattern.empty()) {
		return VALUES();
	}

	if (varName.empty()) {
		return pkbMain.aPatStore.getIndexedStmtsWithPattern(parsedPattern, isExactMatch);
	}
	else {
		INDEX varIndex = getIndexOfDEValue(varName);
		if (!varIndex) return VALUES();
		VALUES assignStmtNums;
		for (auto& assignStmtNum : pkbMain.aPatStore.getIndexedStmtsWithPattern(parsedPattern, isExactMatch)) {
			if (pkbMain.aPatStore.matchesVariableIn(assignStmtNum, varIndex)) {
				assignStmtNums.insert(assignStmtNum);
			}
		}
		return assignStmtNums;
	}
}

VALUES PKB::getAssignWithoutPattern(VAR_NAME varName, VALUES& tableValues) {
	if (tableValues.empty()) {
		return getAssignWithoutPattern(varName);
	}

	VALUES assignStmtNums;
	if (varName.empty()) {
		for (auto& assignStmtNum : tableValues) {
			if (stmtStore.isOfType(DESIGN_ENTITY::ASSIGN, assignStmtNum)) {
				assignStmtNums.insert(assignStmtNum);
			}
		}
	}
	else {
		INDEX varIndex = getIndexOfDEValue(varName);
		if (!varIndex) return VALUES();
		for (auto& assignStmtNum : tableValues) {
			if (pkbMain.aPatStore.matchesVariableIn(assignStmtNum, varIndex)) {
				assignStmtNums.insert(assignStmtNum);
			}
		}
	}
	return assignStmtNums;
}

VALUES PKB::getAssignWithoutPattern(VAR_NAME varName) {
	if (varName.empty()) {
		return stmtStore.getIndexedStmtsOf(DESIGN_ENTITY::ASSIGN);
	}
	else {
		INDEX varIndex = getIndexOfDEValue(varName);
		if (!varIndex) return VALUES();
		return pkbMain.aPatStore.getIndexedStmtsWithVar(getIndexOfDEValue(varName));
	}
}

VALUE_PAIRS PKB::getAssignVarOfPattern(PATTERN pattern, bool isExactMatch, VALUE_PAIRS& tableValuePairs) {
	if (tableValuePairs.empty()) {
		return getAssignVarOfPattern(pattern, isExactMatch);
	}

	PATTERN parsedPattern = RPNConverter::convert(pattern);
	if (parsedPattern.empty()) {
		return VALUE_PAIRS();
	}

	VALUE_PAIRS assignStmtNumVarPairs;
	for (auto& assignStmtNumVarPair : tableValuePairs) {
		if (pkbMain.aPatStore.matchesPatternIn(assignStmtNumVarPair.first, parsedPattern, isExactMatch)
			&& pkbMain.aPatStore.matchesVariableIn(assignStmtNumVarPair.first, assignStmtNumVarPair.second)) {
			assignStmtNumVarPairs.insert(assignStmtNumVarPair);
		}
	}
	return assignStmtNumVarPairs;
}

VALUE_PAIRS PKB::getAssignVarOfPattern(PATTERN pattern, bool isExactMatch, 
	VALUES& tableValues, bool isAssignValues) {
	if (tableValues.empty()) {
		return getAssignVarOfPattern(pattern, isExactMatch);
	}

	PATTERN parsedPattern = RPNConverter::convert(pattern);
	if (parsedPattern.empty()) {
		return VALUE_PAIRS();
	}

	VALUE_PAIRS assignStmtNumVarPairs;
	if (isAssignValues) {
		for (auto& assignStmtNum : tableValues) {
			if (pkbMain.aPatStore.matchesPatternIn(assignStmtNum, parsedPattern, isExactMatch)) {
				assignStmtNumVarPairs.insert({ assignStmtNum, 
					pkbMain.aPatStore.getIndexedVarOf(assignStmtNum) });
			}
		}
	}
	else {
		for (auto& varName : tableValues) {
			for (auto& assignStmtNum : pkbMain.aPatStore.getIndexedStmtsWithVar(varName)) {
				if (pkbMain.aPatStore.matchesPatternIn(assignStmtNum, parsedPattern, isExactMatch)) {
					assignStmtNumVarPairs.insert({ assignStmtNum, varName });
				}
			}
		}
	}
	return assignStmtNumVarPairs;
}

VALUE_PAIRS PKB::getAssignVarOfPattern(PATTERN pattern, bool isExactMatch) {
	PATTERN parsedPattern = RPNConverter::convert(pattern);
	if (parsedPattern.empty()) {
		return VALUE_PAIRS();
	}

	VALUE_PAIRS assignStmtNumVarPairs;
	for (auto& assignStmtNum : pkbMain.aPatStore.getIndexedStmtsWithPattern(parsedPattern, isExactMatch)) {
		assignStmtNumVarPairs.insert({ assignStmtNum, pkbMain.aPatStore.getIndexedVarOf(assignStmtNum) });
	}
	return assignStmtNumVarPairs;
}

VALUE_PAIRS PKB::getAssignVarWithoutPattern(VALUE_PAIRS& tableValuePairs) {
	if (tableValuePairs.empty()) {
		return getAssignVarWithoutPattern();
	}

	VALUE_PAIRS assignStmtNumVarPairs;
	for (auto& assignStmtNumVarPair : tableValuePairs) {
		if (pkbMain.aPatStore.matchesVariableIn(assignStmtNumVarPair.first, assignStmtNumVarPair.second)) {
			assignStmtNumVarPairs.insert(assignStmtNumVarPair);
		}
	}
	return assignStmtNumVarPairs;
}

VALUE_PAIRS PKB::getAssignVarWithoutPattern(VALUES& tableValues, bool isAssignValues) {
	if (tableValues.empty()) {
		return getAssignVarWithoutPattern();
	}

	VALUE_PAIRS assignStmtNumVarPairs;
	if (isAssignValues) {
		for (auto& assignStmtNum : tableValues) {
			if (stmtStore.isOfType(DESIGN_ENTITY::ASSIGN, assignStmtNum)) {
				assignStmtNumVarPairs.insert({ assignStmtNum, 
					pkbMain.aPatStore.getIndexedVarOf(assignStmtNum) });
			}
		}
	}
	else {
		for (auto& varName : tableValues) {
			for (auto& assignStmtNum : pkbMain.aPatStore.getIndexedStmtsWithVar(varName)) {
				assignStmtNumVarPairs.insert({ assignStmtNum, varName });
			}
		}
	}
	return assignStmtNumVarPairs;
}

VALUE_PAIRS PKB::getAssignVarWithoutPattern() {
	return pkbMain.aPatStore.getAssignVarStore().getValuePairs();
}

RelationshipStorage<INDEX, INDEX>& PKB::getAssignVarStore() {
	return pkbMain.aPatStore.getAssignVarStore();
}

RelationshipStorage<INDEX, PATTERN>& PKB::getAssignPatternStore() {
	return pkbMain.aPatStore.getAssignPatternStore();
}

int PKB::getPatternAssignTableSize() {
	return pkbMain.aPatStore.getTableSize();
}

#pragma endregion

#pragma region With
VALUES PKB::getValuesOf(DESIGN_ENTITY valueType, VALUES& tableValues) {
	if (tableValues.empty()) {
		return getValuesOf(valueType);
	}
	//}
	VALUES values;
	for (auto& value : tableValues) {
		if (isOfDEType(valueType, value)) {
			values.insert(value);
		}
	}
	return values;
}

VALUES PKB::getValuesOf(DESIGN_ENTITY valueType) {
	return getIndexedDEValuesOf(valueType);
}

VALUES PKB::getValuesOf(DESIGN_ENTITY valueType, VAR value, VALUES& tableValues) {
	if (tableValues.empty()) {
		return getValuesOf(valueType, value);
	}
	INDEX indexedValue = getIndexOfDEValue(value);
	return indexedValue && isOfDEType(valueType, indexedValue) && tableValues.count(indexedValue) ?
		VALUES({ indexedValue }) : VALUES();
}

VALUES PKB::getValuesOf(DESIGN_ENTITY valueType, VAR value) {
	INDEX indexedValue = getIndexOfDEValue(value);
	return indexedValue && isOfDEType(valueType, indexedValue) ? VALUES({ indexedValue }) : VALUES();
}

VALUE_PAIRS PKB::getValuePairsOf(DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType, 
	VALUE_PAIRS& tableValuePairs) {
	if (tableValuePairs.empty()) {
		return getValuePairsOf(firstValueType, secondValueType);
	}

	VALUE_PAIRS valuePairs;
	for (auto& valuePair : tableValuePairs) {
		if (valuePair.first == valuePair.second
			&& isOfDEType(firstValueType, valuePair.first)
			&& isOfDEType(secondValueType, valuePair.second)) {
			valuePairs.insert(valuePair);
		}
	}
	return valuePairs;
}

VALUE_PAIRS PKB::getValuePairsOf(DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType, 
	VALUES& tableValues) {
	if (tableValues.empty()) {
		return getValuePairsOf(firstValueType, secondValueType);
	}

	VALUE_PAIRS valuePairs;
	for (auto& value : tableValues) {
		if (isOfDEType(firstValueType, value) && isOfDEType(secondValueType, value)) {
			valuePairs.insert({ value, value });
		}
	}
	return valuePairs;
}

VALUE_PAIRS PKB::getValuePairsOf(DESIGN_ENTITY firstValueType, DESIGN_ENTITY secondValueType) {
	VALUE_PAIRS valuePairs;
	for (auto& value : getIndexedDEValuesOf(firstValueType)) {
		if (isOfDEType(secondValueType, value)) {
			valuePairs.insert({ value, value });
		}
	}
	return valuePairs;
}

VALUES PKB::getStmtsWithVar(DESIGN_ENTITY stmtType, VAR var, VALUES& tableValues) {
	if (tableValues.empty()) {
		return getStmtsWithVar(stmtType, var);
	}
	INDEX indexedVar = getIndexOfDEValue(var);
	if (!indexedVar) return VALUES();
	VALUES stmtNums;
	for (auto& stmtNum : tableValues) {
		if (stmtStore.getIndexedVarOf(stmtType, stmtNum) == indexedVar) {
			stmtNums.insert(stmtNum);
		}
	}
	return stmtNums;
}

VALUES PKB::getStmtsWithVar(DESIGN_ENTITY stmtType, VAR var) {
	INDEX indexedVar = getIndexOfDEValue(var);
	if (!indexedVar) return VALUES();
	return stmtStore.getIndexedStmtsWithVar(stmtType, indexedVar);
}

VALUE_PAIRS PKB::getStmtVarPairsOf(DESIGN_ENTITY firstStmtType, DESIGN_ENTITY secondStmtType, 
	VALUE_PAIRS& tableValuePairs) {
	if (tableValuePairs.empty()) {
		return getStmtVarPairsOf(firstStmtType, secondStmtType);
	}

	VALUE_PAIRS stmtVarPairs;
	for (auto& stmtVarPair : tableValuePairs) {
		if (stmtStore.getIndexedVarOf(firstStmtType, stmtVarPair.first) 
			== stmtStore.getIndexedVarOf(secondStmtType, stmtVarPair.second)
			&& !stmtStore.getIndexedVarOf(firstStmtType, stmtVarPair.first)) {
			stmtVarPairs.insert(stmtVarPair);
		}
	}
	return stmtVarPairs;
}

VALUE_PAIRS PKB::getStmtVarPairsOf(DESIGN_ENTITY firstStmtType, DESIGN_ENTITY secondStmtType, 
	VALUES& tableValues, bool isFirstValues) {
	if (tableValues.empty()) {
		return getStmtVarPairsOf(firstStmtType, secondStmtType);
	}

	VALUE_PAIRS stmtVarPairs;
	if (isFirstValues) {
		for (auto& firstStmtNum : tableValues) {
			INDEX indexedVar = stmtStore.getIndexedVarOf(firstStmtType, firstStmtNum);
			for (auto& secondStmtNum : stmtStore.getIndexedStmtsWithVar(secondStmtType, indexedVar)) {
				stmtVarPairs.insert({ firstStmtNum, secondStmtNum });
			}
		}
	}
	else {
		for (auto& secondStmtNum : tableValues) {
			INDEX indexedVar = stmtStore.getIndexedVarOf(secondStmtType, secondStmtNum);
			for (auto& firstStmtNum : stmtStore.getIndexedStmtsWithVar(firstStmtType, indexedVar)) {
				stmtVarPairs.insert({ firstStmtNum, secondStmtNum });
			}
		}
	}
	return stmtVarPairs;
}

VALUE_PAIRS PKB::getStmtVarPairsOf(DESIGN_ENTITY firstStmtType, DESIGN_ENTITY secondStmtType) {
	VALUE_PAIRS stmtVarPairs;
	for (auto& firstStmtNum : stmtStore.getIndexedStmtsOf(firstStmtType)) {
		INDEX indexedVar = stmtStore.getIndexedVarOf(firstStmtType, firstStmtNum);
		for (auto& secondStmtNum : stmtStore.getIndexedStmtsWithVar(secondStmtType, indexedVar)) {
			stmtVarPairs.insert({ firstStmtNum, secondStmtNum });
		}
	}
	return stmtVarPairs;
}

VALUE_PAIRS PKB::getStmtVarVarPairsOf(DESIGN_ENTITY stmtType, DESIGN_ENTITY varType,
	VALUE_PAIRS& tableValuePairs) {
	if (tableValuePairs.empty()) {
		return getStmtVarVarPairsOf(stmtType, varType);
	}

	VALUE_PAIRS stmtVarPairs;
	for (auto& stmtVarPair : tableValuePairs) {
		if (stmtStore.getIndexedVarOf(stmtType, stmtVarPair.first) == stmtVarPair.second 
			&& isOfDEType(varType, stmtVarPair.second)) {
			stmtVarPairs.insert(stmtVarPair);
		}
	}
	return stmtVarPairs;
}

VALUE_PAIRS PKB::getStmtVarVarPairsOf(DESIGN_ENTITY stmtType, DESIGN_ENTITY varType, 
	VALUES& tableValues, bool isStmtValues) {
	if (tableValues.empty()) {
		return getStmtVarVarPairsOf(stmtType, varType);
	}

	VALUE_PAIRS stmtVarPairs;
	if (isStmtValues) {
		for (auto& stmtNum : tableValues) {
			if (isOfDEType(varType, stmtStore.getIndexedVarOf(stmtType, stmtNum))) {
				stmtVarPairs.insert({ stmtNum, stmtStore.getIndexedVarOf(stmtType, stmtNum) });
			}
		}
	}
	else {
		for (auto& var : tableValues) {
			if (isOfDEType(varType, var)) {
				for (auto& stmtNum : stmtStore.getIndexedStmtsWithVar(stmtType, var)) {
					stmtVarPairs.insert({ stmtNum, var });
				}
			}
		}
	}
	return stmtVarPairs;
}

VALUE_PAIRS PKB::getStmtVarVarPairsOf(DESIGN_ENTITY stmtType, DESIGN_ENTITY varType) {
	VALUE_PAIRS stmtVarPairs;
	for (auto& stmtNum : stmtStore.getIndexedStmtsOf(stmtType)) {
		if (isOfDEType(varType, stmtStore.getIndexedVarOf(stmtType, stmtNum))) {
			stmtVarPairs.insert({ stmtNum, stmtStore.getIndexedVarOf(stmtType, stmtNum) });
		}
	}
	return stmtVarPairs;
}

#pragma endregion

#pragma region Select

VAR PKB::getStringStmtVarOf(DESIGN_ENTITY stmtType, STMT_NUM stmtNum) {
	return getStringValueOfDE(stmtStore.getIndexedVarOf(stmtType, getIndexOfDEValue(stmtNum)));
}

#pragma endregion

#pragma endregion

#pragma region PKB Methods

bool PKB::isOfDEType(DESIGN_ENTITY deType, INDEX indexedVar) {
	switch (deType) {
	case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE: case DESIGN_ENTITY::CONSTANT:
		return getDEStoreOf(deType).contains(indexedVar);
	case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
	case DESIGN_ENTITY::ASSIGN: case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::WHILE: 
	case DESIGN_ENTITY::IF:
		return stmtStore.isOfType(deType, indexedVar);
	case DESIGN_ENTITY::PROG_LINE:
		return stmtStore.isOfType(DESIGN_ENTITY::STATEMENT, indexedVar);
	default:
		return false;
	}
}

bool PKB::isOfDEType(DESIGN_ENTITY deType, VAR value) {
	switch (deType) {
	case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE: case DESIGN_ENTITY::CONSTANT:
		return getDEStoreOf(deType).contains(getIndexOfDEValue(value));
	case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
	case DESIGN_ENTITY::ASSIGN: case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::WHILE: 
	case DESIGN_ENTITY::IF:
		return stmtStore.isOfType(deType, getIndexOfDEValue(value));
	case DESIGN_ENTITY::PROG_LINE:
		return stmtStore.isOfType(DESIGN_ENTITY::STATEMENT, getIndexOfDEValue(value));
	default:
		return false;
	}
}

void PKB::clear() {
	astRoot.clear();
	stmtStore.clear();
	procStore.clear();
	varStore.clear();
	constStore.clear();
	pkbMain.fStore.clear();
	pkbMain.fStarStore.clear();
	pkbMain.pStore.clear();
	pkbMain.pStarStore.clear();
	pkbMain.uStmtStore.clear();
	pkbMain.uProcStore.clear();
	pkbMain.mStmtStore.clear();
	pkbMain.mProcStore.clear();
	pkbMain.cStore.clear();
	pkbMain.cStarStore.clear();
	pkbMain.nStore.clear();
	pkbMain.aPatStore.clear();
	pkbMain.wPatStore.clear();
	pkbMain.iPatStore.clear();
	clearCache();
}

void PKB::clearCache() {
	pkbCache.nStarCache.clear();
	pkbCache.aCache.clear();
	pkbCache.aStarCache.clear();
}

#pragma endregion
