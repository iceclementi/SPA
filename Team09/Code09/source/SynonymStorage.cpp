#include "SynonymStorage.h"

#pragma region Synonym Storage

SynonymStorage::SynonymStorage(SYNONYM_COUNTER& synonymCounter) : synonymCounter(synonymCounter), tableIndex(1) {}

SINGLE_TABLE_LIST SynonymStorage::getSingleTables() {
	SINGLE_TABLE_LIST singleTableList;
	for (auto& indexedTable : singleTables) {
		singleTableList.push_back(indexedTable.second);
	}
	return singleTableList;
}

DOUBLE_TABLE_LIST SynonymStorage::getDoubleTables() {
	DOUBLE_TABLE_LIST doubleTableList;
	for (auto& indexedTable : doubleTables) {
		doubleTableList.push_back(indexedTable.second);
	}
	return doubleTableList;
}

bool SynonymStorage::mergeWith(SYNONYM_INDEX synonym, VALUES& values) {
	if (values.empty()) return false;

	bool merged = containsSynonym(synonym) ? intersectWith(synonym, values) : unionWith(synonym, values);
	return merged ? updateSynonymCounter(synonym) : false;
}

bool SynonymStorage::mergeWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs) {
	if (valuePairs.empty()) return false;

	bool merged = (containsSynonym(firstSynonym) || containsSynonym(secondSynonym)) ?
		intersectWith(firstSynonym, secondSynonym, valuePairs) : unionWith(firstSynonym, secondSynonym, valuePairs);
	return merged ? updateSynonymCounter(firstSynonym, secondSynonym) : false;
}

VALUES& SynonymStorage::getSynonymValuesOf(SYNONYM_INDEX synonym) {
	INDEX index = getSingleTableIndexOf(synonym);
	return !index ? emptyValues : singleTables[index].getSynonymValuesOf(synonym);
}

VALUE_PAIRS SynonymStorage::getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	INDEX firstIndex = getSingleTableIndexOf(firstSynonym), secondIndex = getSingleTableIndexOf(secondSynonym);
	if (!firstIndex || !secondIndex) return emptyValuePairs;
	
	PAIR<SYNONYM_INDEX> keyPair = TableUtils::orderSynonyms(firstSynonym, secondSynonym);
	INDEX doubleIndex = getDoubleTableIndexOf(keyPair);
	// Returns the synonym values depending whether synonyms are connected
	return !doubleIndex ?
		TableUtils::makeValuePairs(singleTables[firstIndex].getSynonymValuesOf(firstSynonym), singleTables[secondIndex].getSynonymValuesOf(secondSynonym)) 
		: doubleTables[doubleIndex].getSynonymValuesOf(firstSynonym, secondSynonym);	
}

bool SynonymStorage::containsSynonym(SYNONYM_INDEX synonym) {
	return synonymToSingleTableIndex.count(synonym);
}

bool SynonymStorage::unionWith(SYNONYM_INDEX synonym, VALUES& values) {
	// Don't merge since will be removed
	if (canRemoveSynonym(synonym)) return true;
	
	addSingleTable(MiniSingleTable(synonym, values), synonym);
	return true;
}

bool SynonymStorage::unionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs) {
	PAIR<VALUES> valuesPair = TableUtils::splitPairedValues(valuePairs);
	if (canRemoveSynonym(firstSynonym)) {
		return canRemoveSynonym(secondSynonym) ? true : unionWith(secondSynonym, valuesPair.second);
	}
	unionWith(firstSynonym, valuesPair.first);
	if (canRemoveSynonym(secondSynonym)) return true;

	unionWith(secondSynonym, valuesPair.second);
	addDoubleTable(MiniDoubleTable(firstSynonym, secondSynonym, valuePairs), firstSynonym, secondSynonym);
	return true;
}

bool SynonymStorage::intersectWith(SYNONYM_INDEX synonym, VALUES& synoymValues) {
	// Standardise all tables after intersection
	queue<PAIR<SYNONYM_INDEX, VALUES&>> synonymQueue({ { synonym, synoymValues } });
	while (!synonymQueue.empty()) {
		SYNONYM_INDEX currentSynonym = synonymQueue.front().first;
		VALUES& currentValues = synonymQueue.front().second;
		synonymQueue.pop();

		INDEX index = synonymToSingleTableIndex[currentSynonym];
		VALUES erasedValues;

		// Check if table is empty after intersection
		if (!singleTables[index].intersectWith(currentSynonym, currentValues, erasedValues)) return false;
		// Check if values are changed
		if (erasedValues.empty()) continue;
		// Standardise the rest of the tables to match changes
		for (auto& doubleTableIndex : synonymToDoubleTableIndex[currentSynonym]) {
			doubleTables[doubleTableIndex].removeSynonymValues(currentSynonym, erasedValues);
			SYNONYM_INDEX otherSynonym = doubleTables[doubleTableIndex].getOtherSynonym(currentSynonym);
			synonymQueue.push({ otherSynonym, doubleTables[doubleTableIndex].getSynonymValuesOf(otherSynonym) });
		}
	}
	return true;
}

bool SynonymStorage::intersectWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs) {
	INDEX firstIndex = getSingleTableIndexOf(firstSynonym), secondIndex = getSingleTableIndexOf(secondSynonym);
	if (!firstIndex) { // Missing first synonym
		return intersectUnionWith(firstSynonym, secondSynonym, valuePairs, false);
	}
	if (!secondIndex) { // Missing second synonym
		return intersectUnionWith(firstSynonym, secondSynonym, valuePairs, true);
	}

	// Intersect both synonyms
	PAIR<SYNONYM_INDEX> keyPair = TableUtils::orderSynonyms(firstSynonym, secondSynonym);
	INDEX doubleIndex = getDoubleTableIndexOf(keyPair);
	if (doubleIndex > 0) { // Synonym pair are connected
		return doubleTables[doubleIndex].intersectWith(firstSynonym, secondSynonym, valuePairs) 
			&& intersectWith(firstSynonym, doubleTables[doubleIndex].getSynonymValuesOf(firstSynonym));
	}
	else { // Synonym pair are not connected
		// Intersect single table first
		PAIR<VALUES> valuesPair = TableUtils::splitPairedValues(valuePairs);
		if (!intersectWith(firstSynonym, valuesPair.first) || !intersectWith(secondSynonym, valuesPair.second)) return false;

		// Make new double table
		MiniDoubleTable doubleTable(firstSynonym, secondSynonym, valuePairs);
		if (!doubleTable.intersectWith(firstSynonym, singleTables[firstIndex].getSynonymValuesOf(firstSynonym)) 
			|| !doubleTable.intersectWith(secondSynonym, singleTables[secondIndex].getSynonymValuesOf(secondSynonym))
			|| !intersectWith(firstSynonym, doubleTable.getSynonymValuesOf(firstSynonym))
			|| !intersectWith(secondSynonym, doubleTable.getSynonymValuesOf(secondSynonym))) {
			return false;
		}

		if (canRemoveSynonym(firstSynonym) || canRemoveSynonym(secondSynonym)) return true;
		addDoubleTable(doubleTable, firstSynonym, secondSynonym);

		SYNONYM_INDICES firstGroupSynonyms({ firstSynonym });
		for (auto& index : getDoubleTableIndexOf(firstSynonym)) {
			SYNONYM_INDEX otherSynonym = doubleTables[index].getOtherSynonym(firstSynonym);
			if (otherSynonym != secondSynonym) {
				firstGroupSynonyms.insert(otherSynonym);
			}
		}

		// Connect the two groups
		createConnectingTables(secondSynonym, firstSynonym);
		for (auto& synonym : firstGroupSynonyms) {
			createConnectingTables(synonym, secondSynonym);
		}

		return true;
	}
}

bool SynonymStorage::intersectUnionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs, bool isIntersectFirst) {
	// Intersect first
	SYNONYM_INDEX intersectSynonym = isIntersectFirst ? firstSynonym : secondSynonym;
	VALUES intersectValues = isIntersectFirst ? TableUtils::splitPairedValues(valuePairs).first : TableUtils::splitPairedValues(valuePairs).second;
	if (!intersectWith(intersectSynonym, intersectValues)) return false;

	// Union second, but don't union synonym that will be removed
	SYNONYM_INDEX unionSynonym = isIntersectFirst ? secondSynonym : firstSynonym;
	if (canRemoveSynonym(unionSynonym)) return true;

	MiniDoubleTable doubleTable(firstSynonym, secondSynonym, valuePairs);
	INDEX intersectIndex = synonymToSingleTableIndex[intersectSynonym];
	doubleTable.intersectWith(intersectSynonym, singleTables[intersectIndex].getSynonymValuesOf(intersectSynonym));
	addSingleTable(MiniSingleTable(unionSynonym, doubleTable.getSynonymValuesOf(unionSynonym)), unionSynonym);
	addDoubleTable(doubleTable, firstSynonym, secondSynonym);

	// Add intermediate double tables
	createConnectingTables(unionSynonym, intersectSynonym);

	return true;
}

void SynonymStorage::createConnectingTables(SYNONYM_INDEX synonym, SYNONYM_INDEX referenceSynonym) {
	PAIR<SYNONYM_INDEX> keyPair = TableUtils::orderSynonyms(synonym, referenceSynonym);
	INDEX doubleIndex = getDoubleTableIndexOf(keyPair);
	auto& doubleTable = doubleTables[doubleIndex];

	for (auto& index : synonymToDoubleTableIndex[referenceSynonym]) {
		SYNONYM_INDEX otherSynonym = doubleTables[index].getOtherSynonym(referenceSynonym);
		if (otherSynonym == synonym) continue;
		if (getDoubleTableIndexOf(TableUtils::orderSynonyms(synonym, otherSynonym)) > 0) continue;

		// Connect values from both double tables to form a new double table
		VALUE_PAIRS valuePairs;
		for (auto& referenceValue : doubleTable.getSynonymValuesOf(referenceSynonym)) {
			for (auto& value : doubleTable.getSynonymValuesOf(synonym, referenceSynonym, referenceValue)) {
				for (auto& otherValue : doubleTables[index].getSynonymValuesOf(otherSynonym, referenceSynonym, referenceValue)) {
					valuePairs.insert({ value, otherValue });
				}
			}
		}
		addDoubleTable(MiniDoubleTable(synonym, otherSynonym, valuePairs), synonym, otherSynonym);
	}
}

void SynonymStorage::addSingleTable(MiniSingleTable& singleTable, SYNONYM_INDEX synonym) {
	singleTables[tableIndex] = singleTable;
	synonymToSingleTableIndex[synonym] = tableIndex++;
}

void SynonymStorage::addDoubleTable(MiniDoubleTable& doubleTable, SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	doubleTables[tableIndex] = doubleTable;
	synonymToDoubleTableIndex[firstSynonym].insert(tableIndex);
	synonymToDoubleTableIndex[secondSynonym].insert(tableIndex);
	synonymPairToDoubleTableIndex[TableUtils::orderSynonyms(firstSynonym, secondSynonym)] = tableIndex++;
}

INDEX SynonymStorage::getSingleTableIndexOf(SYNONYM_INDEX synonym) {
	return getValueWithKey(synonymToSingleTableIndex, synonym);
}

INDEX SynonymStorage::getDoubleTableIndexOf(PAIR<SYNONYM_INDEX> synonymPair) {
	return getValueWithKey(synonymPairToDoubleTableIndex, synonymPair);
}

INDICES SynonymStorage::getDoubleTableIndexOf(SYNONYM_INDEX synonym) {
	return getValueWithKey(synonymToDoubleTableIndex, synonym);
}

void SynonymStorage::removeSynonym(SYNONYM_INDEX synonym) {
	synonymCounter.erase(synonym);

	INDEX singleIndex = getSingleTableIndexOf(synonym);
	if (!singleIndex) return;

	// Remove single table
	synonymToSingleTableIndex.erase(synonym);
	singleTables.erase(singleIndex);

	// Remove double tables containing synonym
	for (auto& doubleIndex : getDoubleTableIndexOf(synonym)) {
		SYNONYM_INDEX otherSynonym = doubleTables[doubleIndex].getOtherSynonym(synonym);
		synonymPairToDoubleTableIndex.erase(TableUtils::orderSynonyms(synonym, otherSynonym));
		synonymToDoubleTableIndex[otherSynonym].erase(doubleIndex);
		if (synonymToDoubleTableIndex[otherSynonym].empty()) {
			synonymToDoubleTableIndex.erase(otherSynonym);
		}
		doubleTables.erase(doubleIndex);
	}
	synonymToDoubleTableIndex.erase(synonym);
}

bool SynonymStorage::canRemoveSynonym(SYNONYM_INDEX synonym) {
	return synonymCounter[synonym] == 1 && !isSelectSynonym(synonym);
}

bool SynonymStorage::updateSynonymCounter(SYNONYM_INDEX synonym) {
	if (canRemoveSynonym(synonym)) {
		removeSynonym(synonym);
	}
	else {
		--synonymCounter[synonym];
	}
	return true;
}

bool SynonymStorage::updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	return updateSynonymCounter(firstSynonym) && updateSynonymCounter(secondSynonym);
}

void SynonymStorage::clear() {
	INDEX tableIndex = 1;
	synonymToSingleTableIndex.clear();
	singleTables.clear();
	doubleTables.clear();
	synonymToSingleTableIndex.clear();
	synonymPairToDoubleTableIndex.clear();
	synonymToDoubleTableIndex.clear();
	synonymCounter.clear();
}

#pragma endregion

#pragma region Select Synonym Storage

SelectSynonymStorage::SelectSynonymStorage(SYNONYM_COUNTER& synonymCounter, SYNONYM_INDICES& selectSynonyms) 
	: SynonymStorage(synonymCounter), selectSynonyms(selectSynonyms) {}

SYNONYM_INDICES& SelectSynonymStorage::getSelectSynonyms() {
	return selectSynonyms;
}

bool SelectSynonymStorage::isSelectSynonym(SYNONYM_INDEX synonym) {
	return selectSynonyms.count(synonym);
}

void SelectSynonymStorage::clear() {
	SynonymStorage::clear();
	selectSynonyms.clear();
}

#pragma endregion

#pragma region Temp Synonym Storage

TempSynonymStorage::TempSynonymStorage(SYNONYM_COUNTER& synonymCounter) : SynonymStorage(synonymCounter) {}

bool TempSynonymStorage::isSelectSynonym(SYNONYM_INDEX synonym) {
	return false;
}

void TempSynonymStorage::clear() {
	SynonymStorage::clear();
}

#pragma endregion

#pragma region Result Synonym Storage

ResultSynonymStorage::ResultSynonymStorage(ELEMENT_LIST& selectElements) : SynonymStorage(), selectElements(selectElements) {}

void ResultSynonymStorage::init(ELEMENT_LIST& selectElements) {
	this->selectElements = selectElements;
}

void ResultSynonymStorage::mergeTable(SelectSynonymStorage& storage) {
	for (auto& singleTable : storage.getSingleTables()) {
		addSingleTable(singleTable, singleTable.getSynonym());
	}
	for (auto& doubleTable : storage.getDoubleTables()) {
		addDoubleTable(doubleTable, doubleTable.getSynonyms().first, doubleTable.getSynonyms().second);
	}
}

SYNONYM_INDICES ResultSynonymStorage::getMissingSelectSynonyms() {
	SYNONYM_INDICES missingSynonyms;
	for (auto& element : selectElements) {
		SYNONYM_INDEX selectSynonym = element.getSynIndex();
		if (!containsSynonym(selectSynonym)) {
			missingSynonyms.insert(selectSynonym);
		}
	}
	return missingSynonyms;
}

INDEXED_INTERMEDIATE_ANSWER ResultSynonymStorage::getIntermediateAnswer() {
	SYNONYM_INDEX_LIST selectSynonymList;
	for (auto& element : selectElements) {
		selectSynonymList.push_back(element.getSynIndex());
	}

	if (selectElements.empty()) return intermediateAnswer;

	generateIntermediateAnswer(0, VALUE_LIST(), SYNONYM_VALUE_TRACKER());
	return intermediateAnswer;
}

void ResultSynonymStorage::generateIntermediateAnswer(INDEX index, VALUE_LIST& intermediateResult, SYNONYM_VALUE_TRACKER& synonymValues) {
	// Base case
	if (index == selectElements.size()) {
		intermediateAnswer.push_back(VALUE_LIST(intermediateResult.begin(), intermediateResult.end()));
		return;
	}

	SYNONYM_INDEX synonym = selectElements[index].getSynIndex();
	// Synonym exists in list
	if (!synonymValues[synonym].empty()) {
		VALUE value = synonymValues[synonym].top();
		updateIntermediateResult(synonym, value, index, intermediateResult, synonymValues);
		return;
	}
	// Synonym does not exist in list yet
	VALUES branchValues;
	for (auto& otherSynonymValue : synonymValues) {
		if (otherSynonymValue.second.empty()) continue;
		PAIR<SYNONYM_INDEX> keyPair = TableUtils::orderSynonyms(synonym, otherSynonymValue.first);
		INDEX doubleIndex = getDoubleTableIndexOf(keyPair);
		// Synonym is connected to another synonym in the list
		if (doubleIndex > 0) {
			branchValues = TableUtils::intersectValues(branchValues,
				doubleTables[doubleIndex].getSynonymValuesOf(synonym, otherSynonymValue.first, otherSynonymValue.second.top()));
		}
	}
	if (!branchValues.empty()) {
		for (auto& value : branchValues) {
			updateIntermediateResult(synonym, value, index, intermediateResult, synonymValues);
		}
		return;
	}

	// Synonym is not connected to any synonym in the list
	INDEX singleIndex = getSingleTableIndexOf(synonym);
	if (!singleIndex) return;
	for (auto& value : singleTables[singleIndex].getSynonymValuesOf(synonym)) {
		updateIntermediateResult(synonym, value, index, intermediateResult, synonymValues);
	}
}

void ResultSynonymStorage::updateIntermediateResult(SYNONYM_INDEX synonym, VALUE value, INDEX index, 
	VALUE_LIST& intermediateResult, SYNONYM_VALUE_TRACKER& synonymValues) {
	synonymValues[synonym].push(value);
	intermediateResult.push_back(value);
	generateIntermediateAnswer(index + 1, intermediateResult, synonymValues);
	intermediateResult.pop_back();
	synonymValues[synonym].pop();
}

bool ResultSynonymStorage::isSelectSynonym(SYNONYM_INDEX synonym) {
	return true;
}

void ResultSynonymStorage::clear() {
	SynonymStorage::clear();
	selectElements.clear();
	intermediateAnswer.clear();
}

#pragma endregion
