#pragma once

#include "Common.h"

template <typename T1, typename T2>
using RELATIONSHIP_TABLE = M<T1, S<T2>>;

template <typename T>
class IndexTable {
	typedef M<INDEX, T> INDEX_VALUE_TABLE;
	typedef M<T, INDEX> VALUE_INDEX_TABLE;

public:
	IndexTable();

	/* Adds a mapping from the value to an index into the table */
	INDEX addMapping(T value);
	/* Retrieves the var with the given index */
	T getValueWithIndex(INDEX index);
	/* Retrieves the index of the given value */
	INDEX getIndexOf(T value);
	/* Retrieves the index of the given value if exists or creates a new index */
	INDEX createIndexOf(T value);

	int getTableSize();
	bool isEmpty();
	void clear();

private:
	INDEX index;
	INDEX_VALUE_TABLE indexToValueTable;
	VALUE_INDEX_TABLE valueToIndexTable;
};

class PKBStorage {
public:
	static VAR getStringValueOf(INDEX indexedValue);
	static INDEX getIndexOf(VAR value);
	static INDEX createIndexOf(VAR value);

	virtual int getTableSize() = 0;
	virtual bool isEmpty() = 0;
	virtual void clear() = 0;

protected:
	static IndexTable<VAR> indexTable;
};

template <typename T>
class BasicStorage : public PKBStorage {
public:
	BasicStorage();

	/* Retrieve all the values */
	INDICES& getIndexedValues();
	/* Adds a value into the storage */
	void addValue(T value);
	/* Verifies if the value exists in the storage */
	bool contains(INDEX indexedValue);

	int getTableSize();
	bool isEmpty();
	void clear();

private:
	INDICES indexedValues;
};

template <typename T1, typename T2>
class RelationshipStorage : public PKBStorage {
public:
	RelationshipStorage();

	/* Retrieves the forward relationship table */
	RELATIONSHIP_TABLE<T1, T2>& getForwardTable();
	/* Retrieves the backward relationship table */
	RELATIONSHIP_TABLE<T2, T1>& getBackwardTable();
	/* Adds a relationship into the storage */
	void addRelationship(T1 firstValue, T2 secondValue);
	/* Removes a relationship from the storage */
	void removeRelationship(T1 firstValue, T2 secondValue);

	/* Retrieves all the first values */
	virtual S<T1>& getFirstValues();
	/* Retrieves all the second values */
	virtual S<T2>& getSecondValues();
	/* Retrieves all the relationships as pairs */
	virtual PAIRS<T1, T2> getValuePairs();
	/* Retrieves all the first values of a relationship given the second value */
	virtual S<T1>& getFirstValuesOf(T2 secondValue);
	/* Retrieves all the second values of a relationship given the first value */
	virtual S<T2>& getSecondValuesOf(T1 firstValue);

	/* Verifies if the forward relationship between the first and second values exists */
	virtual bool hasRelationship(T1 firstValue, T2 secondValue);
	/* Verifies if any relationship exists with the given second value */
	virtual bool hasRelationshipWithSecondValue(T2 secondValue);
	/* Verifies if any relationship exists with the given first value */
	virtual bool hasRelationshipWithFirstValue(T1 firstValue);

	int getTableSize();
	bool isEmpty();
	virtual void clear();

private:
	PAIR<DESIGN_ENTITY> valueTypes;
	RELATIONSHIP_TABLE<T1, T2> forwardTable;
	RELATIONSHIP_TABLE<T2, T1> backwardTable;
	S<T1> firstValues;
	S<T2> secondValues;
	S<T1> emptyFirstValues;
	S<T2> emptySecondValues;

	bool firstValueExists(T1 value);
	bool secondValueExists(T2 value);
};

class HybridStorage : public PKBStorage {
public:
	virtual int getTableSize() = 0;
	virtual bool isEmpty() = 0;
	virtual void clear() = 0;

protected:
	template <typename T>
	T getSingleFrom(S<T>& values);
};

class StatementStorage : public HybridStorage {
public:
	StatementStorage();

	/* Retrieves all the indexed statements of the given statement type */
	INDICES& getIndexedStmtsOf(DESIGN_ENTITY stmtType);
	/* Adds a statement into the statement DE storage */
	void addStmt(DESIGN_ENTITY stmtType, STMT_NUM stmtNum);
	/* Verifies if a statement is of the given statement type */
	bool isOfType(DESIGN_ENTITY stmtType, INDEX indexStmtNum);

	/* Retrieves the statement var storage of the given statement type */
	RELATIONSHIP_TABLE<INDEX, INDEX>& getStmtVarsOf(DESIGN_ENTITY stmtType);
	/* Adds a statement into the statement var storage */
	void addStmtVar(DESIGN_ENTITY stmtType, STMT_NUM stmtNum, VAR var);
	/* Retrieves the indexed var of the statement number of the given statement type */
	INDEX getIndexedVarOf(DESIGN_ENTITY stmtType, INDEX indexedStmt);
	/* Retrieves all the indexed vars of the given statement type */
	INDICES& getIndexedVarsOf(DESIGN_ENTITY stmtType);
	/* Retrieves the indexed statement numbers of the given statement type that has the given var */
	INDICES& getIndexedStmtsWithVar(DESIGN_ENTITY stmtType, INDEX indexedVar);

	int getTableSize();
	int getTableSizeOf(DESIGN_ENTITY stmtType);
	bool isEmpty();
	void clear();

private:
	RelationshipStorage<INDEX, DESIGN_ENTITY> stmtDEStore;

	RelationshipStorage<INDEX, INDEX> readVarStore;
	RelationshipStorage<INDEX, INDEX> printVarStore;
	RelationshipStorage<INDEX, INDEX> callProcStore;
	RELATIONSHIP_TABLE<INDEX, INDEX> emptyStmtVarTable;

	INDICES emptyIndices;
};

class AssignPatternStorage : public HybridStorage {
public:
	AssignPatternStorage();

	/* Retrieves the assign variable storage */
	RelationshipStorage<INDEX, INDEX>& getAssignVarStore();
	/* Retrieves the assign pattern storage */
	RelationshipStorage<INDEX, PATTERN>& getAssignPatternStore();
	/* Adds a pattern into the storages */
	void addPattern(INDEX indexedStmtNum, INDEX indexedVarName, PATTERN pattern);
	/* Retrieves the indexed variable associated with the assign statement */
	INDEX getIndexedVarOf(INDEX indexedStmtNum);
	/* Retrieves the pattern associated with the assign statement */
	PATTERN getPatternOf(INDEX indexedStmtNum);
	/* Retrieves the indexed assign statement numbers with the given assign variable */
	INDICES& getIndexedStmtsWithVar(INDEX indexedVarName);
	/* Retrieves the indexed assign statement numbers with the given pattern */
	INDICES getIndexedStmtsWithPattern(PATTERN pattern, bool isExactMatch);
	/* Verifies if the assign statement has the given assign variable */
	bool matchesVariableIn(INDEX indexedStmtNum, INDEX indexedVarName);
	/* Verifies if the assign statement has the given pattern */
	bool matchesPatternIn(INDEX indexedStmtNum, PATTERN pattern, bool isExactMatch);

	int getTableSize();
	bool isEmpty();
	void clear();

private:
	RelationshipStorage<INDEX, INDEX> assignVarStore;
	RelationshipStorage<INDEX, PATTERN> assignPatternStore;

	bool matchesPattern(PATTERN pattern, PATTERN otherPattern, bool isExactMatch);
};

#pragma region Index Table

template <typename T>
inline IndexTable<T>::IndexTable() : index(1) {};

template <typename T>
inline INDEX IndexTable<T>::addMapping(T value) {
	valueToIndexTable[value] = index;
	indexToValueTable[index] = value;
	return index++;
}

template <typename T>
inline T IndexTable<T>::getValueWithIndex(INDEX index) {
	return getValueWithKey(indexToValueTable, index);
}

template <typename T>
inline INDEX IndexTable<T>::getIndexOf(T value) {
	return getValueWithKey(valueToIndexTable, value);
}

template <typename T>
inline INDEX IndexTable<T>::createIndexOf(T value) {
	INDEX indexOfValue = getValueWithKey(valueToIndexTable, value);
	return indexOfValue ? indexOfValue : addMapping(value);
}

template <typename T>
inline int IndexTable<T>::getTableSize() {
	return valueToIndexTable.size();
}

template <typename T>
inline bool IndexTable<T>::isEmpty() {
	return valueToIndexTable.empty();
}

template <typename T>
inline void IndexTable<T>::clear() {
	valueToIndexTable.clear();
	indexToValueTable.clear();
	index = 1;
}

#pragma endregion

#pragma region Basic Storage

template <typename T>
inline BasicStorage<T>::BasicStorage() {}

template <typename T>
inline INDICES& BasicStorage<T>::getIndexedValues() {
	return indexedValues;
}

template <typename T>
inline void BasicStorage<T>::addValue(T value) {
	indexedValues.insert(indexTable.createIndexOf(value));
}

template <typename T>
inline bool BasicStorage<T>::contains(INDEX indexedValue) {
	return indexedValues.count(indexedValue);
}

template <typename T>
inline int BasicStorage<T>::getTableSize() {
	return indexedValues.size();
}

template <typename T>
inline bool BasicStorage<T>::isEmpty() {
	return indexedValues.empty();
}

template <typename T>
inline void BasicStorage<T>::clear() {
	indexedValues.clear();
	indexTable.clear();
}

#pragma endregion

#pragma region Relationship Storage

template <typename T1, typename T2>
inline RelationshipStorage<T1, T2>::RelationshipStorage() {}

template <typename T1, typename T2>
inline RELATIONSHIP_TABLE<T1, T2>& RelationshipStorage<T1, T2>::getForwardTable() {
	return forwardTable;
}

template <typename T1, typename T2>
inline RELATIONSHIP_TABLE<T2, T1>& RelationshipStorage<T1, T2>::getBackwardTable() {
	return backwardTable;
}

template <typename T1, typename T2>
inline void RelationshipStorage<T1, T2>::addRelationship(T1 firstValue, T2 secondValue) {
	forwardTable[firstValue].insert(secondValue);
	backwardTable[secondValue].insert(firstValue);
	firstValues.insert(firstValue);
	secondValues.insert(secondValue);
}

template <typename T1, typename T2>
inline void RelationshipStorage<T1, T2>::removeRelationship(T1 firstValue, T2 secondValue) {
	forwardTable[firstValue].erase(secondValue);
	backwardTable[secondValue].erase(firstValue);
	
	if (forwardTable[firstValue].empty()) {
		forwardTable.erase(firstValue);
		firstValues.erase(firstValue);
	}
	if (backwardTable[secondValue].empty()) {
		backwardTable.erase(secondValue);
		secondValues.erase(secondValue);
	}
}

template <typename T1, typename T2>
inline S<T1>& RelationshipStorage<T1, T2>::getFirstValues() {
	return firstValues;
}

template <typename T1, typename T2>
inline S<T2>& RelationshipStorage<T1, T2>::getSecondValues() {
	return secondValues;
}

template <typename T1, typename T2>
inline PAIRS<T1, T2> RelationshipStorage<T1, T2>::getValuePairs() {
	PAIRS<T1, T2> valuePairs;
	for (auto entry : forwardTable) {
		for (auto secondValue : entry.second) {
			valuePairs.insert({ entry.first, secondValue });
		}
	}
	return valuePairs;
}

template <typename T1, typename T2>
inline S<T1>& RelationshipStorage<T1, T2>::getFirstValuesOf(T2 secondValue) {
	return secondValueExists(secondValue) ? backwardTable[secondValue] : emptyFirstValues;
}

template <typename T1, typename T2>
inline S<T2>& RelationshipStorage<T1, T2>::getSecondValuesOf(T1 firstValue) {
	return firstValueExists(firstValue) ? forwardTable[firstValue] : emptySecondValues;
}

template <typename T1, typename T2>
inline bool RelationshipStorage<T1, T2>::hasRelationship(T1 firstValue, T2 secondValue) {
	return getSecondValuesOf(firstValue).count(secondValue);
}

template <typename T1, typename T2>
inline bool RelationshipStorage<T1, T2>::hasRelationshipWithSecondValue(T2 secondValue) {
	return !getFirstValuesOf(secondValue).empty();
}

template <typename T1, typename T2>
inline bool RelationshipStorage<T1, T2>::hasRelationshipWithFirstValue(T1 firstValue) {
	return !getSecondValuesOf(firstValue).empty();
}

template <typename T1, typename T2>
inline int RelationshipStorage<T1, T2>::getTableSize() {
	return getValuePairs().size();
}

template <typename T1, typename T2>
inline bool RelationshipStorage<T1, T2>::isEmpty() {
	return forwardTable.empty() && backwardTable.empty();
}

template <typename T1, typename T2>
inline void RelationshipStorage<T1, T2>::clear() {
	forwardTable.clear();
	backwardTable.clear();
	firstValues.clear();
	secondValues.clear();
}

template <typename T1, typename T2>
inline bool RelationshipStorage<T1, T2>::firstValueExists(T1 value) {
	return firstValues.count(value);
}

template <typename T1, typename T2>
inline bool RelationshipStorage<T1, T2>::secondValueExists(T2 value) {
	return secondValues.count(value);
}
#pragma endregion
