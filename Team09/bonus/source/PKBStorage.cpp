#include "PKBStorage.h"

#pragma region PKB Storage

IndexTable<VAR> PKBStorage::indexTable;

VAR PKBStorage::getStringValueOf(INDEX indexedValue) {
	return indexTable.getValueWithIndex(indexedValue);
}

INDEX PKBStorage::getIndexOf(VAR value) {
	return indexTable.getIndexOf(value);
}

INDEX PKBStorage::createIndexOf(VAR value) {
	return indexTable.createIndexOf(value);
}

#pragma endregion

template <typename T>
T HybridStorage::getSingleFrom(S<T>& values) {
	if (values.size() != 1) return T();
	return *(values.begin());
}

#pragma region Statement Storage

StatementStorage::StatementStorage() {}

INDICES& StatementStorage::getIndexedStmtsOf(DESIGN_ENTITY stmtType) {
	return stmtDEStore.getFirstValuesOf(stmtType);
}

void StatementStorage::addStmt(DESIGN_ENTITY stmtType, STMT_NUM stmtNum) {
	INDEX index = createIndexOf(stmtNum);
	switch (stmtType) {
	case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::ASSIGN:
	case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF:
		stmtDEStore.addRelationship(index, stmtType);
		stmtDEStore.addRelationship(index, DESIGN_ENTITY::STATEMENT);
		break;
	default:
		return;
	}
}

bool StatementStorage::isOfType(DESIGN_ENTITY stmtType, INDEX indexStmtNum) {
	return stmtDEStore.hasRelationship(indexStmtNum, stmtType);
}

RELATIONSHIP_TABLE<INDEX, INDEX>& StatementStorage::getStmtVarsOf(DESIGN_ENTITY stmtType) {
	switch (stmtType) {
	case DESIGN_ENTITY::READ:
		return readVarStore.getForwardTable();
	case DESIGN_ENTITY::PRINT:
		return printVarStore.getForwardTable();
	case DESIGN_ENTITY::CALL:
		return callProcStore.getForwardTable();
	default:
		return emptyStmtVarTable;
	}
}

void StatementStorage::addStmtVar(DESIGN_ENTITY stmtType, STMT_NUM stmtNum, VAR var) {
	INDEX indexedStmt = createIndexOf(stmtNum), indexedVar = createIndexOf(var);

	switch (stmtType) {
	case DESIGN_ENTITY::READ:
		readVarStore.addRelationship(indexedStmt, indexedVar);
		break;
	case DESIGN_ENTITY::PRINT:
		printVarStore.addRelationship(indexedStmt, indexedVar);
		break;
	case DESIGN_ENTITY::CALL:
		callProcStore.addRelationship(indexedStmt, indexedVar);
		break;
	default:
		return;
	}
}

INDEX StatementStorage::getIndexedVarOf(DESIGN_ENTITY stmtType, INDEX indexedStmt) {
	switch (stmtType) {
	case DESIGN_ENTITY::READ:
		return getSingleFrom(readVarStore.getSecondValuesOf(indexedStmt));
	case DESIGN_ENTITY::PRINT:
		return getSingleFrom(printVarStore.getSecondValuesOf(indexedStmt));
	case DESIGN_ENTITY::CALL:
		return getSingleFrom(callProcStore.getSecondValuesOf(indexedStmt));
	default:
		return 0;
	}
}

INDICES& StatementStorage::getIndexedVarsOf(DESIGN_ENTITY stmtType) {
	switch (stmtType) {
	case DESIGN_ENTITY::READ:
		return readVarStore.getSecondValues();
	case DESIGN_ENTITY::PRINT:
		return printVarStore.getSecondValues();
	case DESIGN_ENTITY::CALL:
		return callProcStore.getSecondValues();
	default:
		return emptyIndices;
	}
}

INDICES& StatementStorage::getIndexedStmtsWithVar(DESIGN_ENTITY stmtType, INDEX indexedVar) {
	switch (stmtType) {
	case DESIGN_ENTITY::READ:
		return readVarStore.getFirstValuesOf(indexedVar);
	case DESIGN_ENTITY::PRINT:
		return printVarStore.getFirstValuesOf(indexedVar);
	case DESIGN_ENTITY::CALL:
		return callProcStore.getFirstValuesOf(indexedVar);
	default:
		return emptyIndices;
	}
}

int StatementStorage::getTableSize() {
	return stmtDEStore.getTableSize();
}

int StatementStorage::getTableSizeOf(DESIGN_ENTITY stmtType) {
	return stmtDEStore.getFirstValuesOf(stmtType).size();
}

bool StatementStorage::isEmpty() {
	return stmtDEStore.isEmpty();
}

void StatementStorage::clear() {
	stmtDEStore.clear();
	readVarStore.clear();
	printVarStore.clear();
	callProcStore.clear();
	indexTable.clear();
}

#pragma endregion

#pragma region Assign Pattern Storage

AssignPatternStorage::AssignPatternStorage() {}

RelationshipStorage<INDEX, INDEX>& AssignPatternStorage::getAssignVarStore() {
	return assignVarStore;
}

RelationshipStorage<INDEX, PATTERN>& AssignPatternStorage::getAssignPatternStore() {
	return assignPatternStore;
}

void AssignPatternStorage::addPattern(INDEX indexedStmtNum, INDEX indexedVarName, PATTERN pattern) {
	assignVarStore.addRelationship(indexedStmtNum, indexedVarName);
	assignPatternStore.addRelationship(indexedStmtNum, pattern);
}

INDEX AssignPatternStorage::getIndexedVarOf(INDEX indexedStmtNum) {
	return getSingleFrom(assignVarStore.getSecondValuesOf(indexedStmtNum));
}

PATTERN AssignPatternStorage::getPatternOf(INDEX indexedStmtNum) {
	return getSingleFrom(assignPatternStore.getSecondValuesOf(indexedStmtNum));
}

INDICES& AssignPatternStorage::getIndexedStmtsWithVar(INDEX indexedVarName) {
	return assignVarStore.getFirstValuesOf(indexedVarName);
}

INDICES AssignPatternStorage::getIndexedStmtsWithPattern(PATTERN pattern, bool isExactMatch) {
	INDICES assignStmts;
	for (auto& row : assignPatternStore.getBackwardTable()) {
		if (matchesPattern(row.first, pattern, isExactMatch)) {
			assignStmts.insert(row.second.begin(), row.second.end());
		}
	}
	return assignStmts;
}

bool AssignPatternStorage::matchesVariableIn(INDEX indexedStmtNum, INDEX indexedVarName) {
	return assignVarStore.hasRelationship(indexedStmtNum, indexedVarName);
}

bool AssignPatternStorage::matchesPatternIn(INDEX indexedStmtNum, PATTERN pattern, bool isExactMatch) {
	return matchesPattern(getPatternOf(indexedStmtNum), pattern, isExactMatch);
}

bool AssignPatternStorage::matchesPattern(PATTERN pattern, PATTERN otherPattern, bool isExactMatch) {
	return isExactMatch ? (pattern == otherPattern) : (pattern.find(otherPattern) != string::npos);
}

int AssignPatternStorage::getTableSize() {
	return assignPatternStore.getTableSize();
}

bool AssignPatternStorage::isEmpty() {
	return assignVarStore.isEmpty() && assignPatternStore.isEmpty();
}

void AssignPatternStorage::clear() {
	assignVarStore.clear();
	assignPatternStore.clear();
}

#pragma endregion
