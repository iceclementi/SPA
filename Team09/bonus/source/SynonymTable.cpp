#include "SynonymTable.h"

#pragma region Synonym Table

SynonymTable::SynonymTable(SYNONYM_COUNTER& synonymCounter) : synonymCounter(synonymCounter) {}

SYNONYM_TABLE& SynonymTable::getSynTable() {
	return synTable;
}

void SynonymTable::setSynTable(SYNONYM_TABLE& tableToSet, SYNONYM_INDICES& synonyms) {
	synTable = tableToSet;
	existingSynonyms.insert(synonyms.begin(), synonyms.end());
}

bool SynonymTable::mergeWith(SYNONYM_INDEX synonym, VALUES& values) {
	if (containsSynonym(synonym)) {
		intersectWith(synonym, values);
	}
	else {
		unionWith(synonym, values);
	}
	
	return synTable.empty() ? false : updateSynonymCounter(synonym);
}

bool SynonymTable::mergeWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuesPair) {
	if (containsSynonym(firstSynonym) && containsSynonym(secondSynonym)) {
		intersectWith(firstSynonym, secondSynonym, valuesPair);
	}
	else if (containsSynonym(firstSynonym) && !containsSynonym(secondSynonym)) {
		intersectUnionWith(firstSynonym, secondSynonym, mapValuePairs(valuesPair, true));
	}
	else if (!containsSynonym(firstSynonym) && containsSynonym(secondSynonym)) {
		intersectUnionWith(secondSynonym, firstSynonym, mapValuePairs(valuesPair, false));
	}
	else {
		unionWith(firstSynonym, secondSynonym, valuesPair);
	}
	
	return synTable.empty() ? false : updateSynonymCounter(firstSynonym, secondSynonym);
}

void SynonymTable::intersectWith(SYNONYM_INDEX synonym, VALUES& values) {
	// Delete rows that does not contain the synonym value
	auto row = synTable.begin();
	while (row != synTable.end()) {
		VALUE synValue = getSynonymValueAt(*row, synonym);
		if (!values.count(synValue)) {
			row = synTable.erase(row);
			continue;
		}
		++row;
	}
}

void SynonymTable::intersectWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuesPair) {
	// Delete rows that does not contain the synonym value pair
	auto row = synTable.begin();
	while (row != synTable.end()) {
		VALUE firstSynValue = getSynonymValueAt(*row, firstSynonym);
		VALUE secondSynValue = getSynonymValueAt(*row, secondSynonym);
		if (!valuesPair.count({ firstSynValue, secondSynValue })) {
			row = synTable.erase(row);
			continue;
		}
		++row;
	}
}

void SynonymTable::unionWith(SYNONYM_INDEX synonym, VALUES& values) {
	existingSynonyms.insert(synonym);

	// If table is empty, just add entire list of values
	if (synTable.empty()) {
		for (auto& value : values) {
			SYNONYM_TABLE_ROW newRow({ { synonym, value } });
			synTable.push_back(newRow);
		}
		return;
	}

	// Permutate each row with the addition of one value from the list
	size_t newSize = existingSynonyms.size();
	auto row = synTable.begin();
	while (row != synTable.end()) {
		if ((*row).size() >= newSize) {
			break;
		}
		for (auto& value : values) {
			SYNONYM_TABLE_ROW newRow = *row;
			newRow[synonym] = value;
			synTable.push_back(newRow);
		}
		row = synTable.erase(row);
	}
}

void SynonymTable::unionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuesPair) {
	existingSynonyms.insert({ firstSynonym, secondSynonym });

	// If table is empty, just add entire list of values
	if (synTable.empty()) {
		for (auto& valuePair : valuesPair) {
			SYNONYM_TABLE_ROW newRow({ { firstSynonym, valuePair.first }, { secondSynonym, valuePair.second } });
			synTable.push_back(newRow);
		}
		return;
	}

	// Permutate each row with the addition of one value pair from the list
	size_t newSize = existingSynonyms.size();
	auto row = synTable.begin();
	while (row != synTable.end()) {
		if ((*row).size() >= newSize) {
			break;
		}
		for (auto& valuePair : valuesPair) {
			SYNONYM_TABLE_ROW newRow = *row;
			newRow[firstSynonym] = valuePair.first;
			newRow[secondSynonym] = valuePair.second;
			synTable.push_back(newRow);
		}
		row = synTable.erase(row);
	}
}

void SynonymTable::intersectUnionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUES_MAP& valuesMap) {
	existingSynonyms.insert(secondSynonym);

	// Delete rows that does not contain the first synonym value, 
	// otherwise append the second synonym value
	size_t newSize = existingSynonyms.size();
	auto row = synTable.begin();
	while (row != synTable.end()) {
		if ((*row).size() >= newSize) {
			break;
		}
		VALUE synValue = getSynonymValueAt(*row, firstSynonym);
		if (valuesMap.count(synValue)) {
			for (auto& value : valuesMap.at(synValue)) {
				SYNONYM_TABLE_ROW newRow = *row;
				newRow[secondSynonym] = value;
				synTable.push_back(newRow);
			}
		}
		row = synTable.erase(row);
	}
}

VALUE SynonymTable::getSynonymValueAt(SYNONYM_TABLE_ROW& row, SYNONYM_INDEX synonym) {
	return row.count(synonym) ? row[synonym] : 0;
}

bool SynonymTable::containsSynonym(SYNONYM_INDEX synonym) {
	return existingSynonyms.count(synonym);
}

VALUES SynonymTable::getSynonymValuesOf(SYNONYM_INDEX synonym) {
	VALUES values;
	if (containsSynonym(synonym)) {
		for (auto& row : synTable) {
			values.insert(getSynonymValueAt(row, synonym));
		}
	}
	return values;
}

VALUE_PAIRS SynonymTable::getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	VALUE_PAIRS valuePairs;
	if (containsSynonym(firstSynonym) && containsSynonym(secondSynonym)) {
		for (auto& row : synTable) {
			valuePairs.insert({ getSynonymValueAt(row, firstSynonym), getSynonymValueAt(row, secondSynonym) });
		}
	}
	return valuePairs;
}

VALUE_LISTS SynonymTable::getSynonymValuesOf(SYNONYM_INDEX_LIST& synonyms) {
	for (auto& synonym : synonyms) {
		if (!containsSynonym(synonym)) {
			return VALUE_LISTS();
		}
	}

	VALUE_LISTS valuesList;
	for (auto& row : synTable) {
		VALUE_LIST valueList;
		for (auto& synonym : synonyms) {
			valueList.push_back(getSynonymValueAt(row, synonym));
		}
		valuesList.insert(valueList);
	}
	return valuesList;
}

void SynonymTable::removeSynonym(SYNONYM_INDEX synonym) {
	set<SYNONYM_TABLE_ROW> tableRows;
	for (auto& row : synTable) {
		row.erase(synonym);
		tableRows.insert(row);
	}
	synTable = SYNONYM_TABLE(tableRows.begin(), tableRows.end());
}

void SynonymTable::removeSynonyms(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	set<SYNONYM_TABLE_ROW> tableRows;
	for (auto& row : synTable) {
		row.erase(firstSynonym);
		row.erase(secondSynonym);
		tableRows.insert(row);
	}
	synTable = SYNONYM_TABLE(tableRows.begin(), tableRows.end());
}

VALUES_MAP SynonymTable::mapValuePairs(VALUE_PAIRS& valuesPair, bool isMaptoSecond) {
	VALUES_MAP valuesMap;
	if (isMaptoSecond) {
		for (auto& valuePair : valuesPair) {
			valuesMap[valuePair.first].insert(valuePair.second);
		}
	}
	else {
		for (auto& valuePair : valuesPair) {
			valuesMap[valuePair.second].insert(valuePair.first);
		}
	}
	return valuesMap;
}

void SynonymTable::clear() {
	synTable.clear();
	existingSynonyms.clear();
	synonymCounter.clear();
}

#pragma endregion

SelectSynonymTable::SelectSynonymTable(SYNONYM_COUNTER& synonymCounter, SYNONYM_INDICES& selectSynonyms) 
	: SynonymTable(synonymCounter), selectSynonyms(selectSynonyms) {}

SYNONYM_INDICES& SelectSynonymTable::getSelectSynonyms() {
	return selectSynonyms;
}

bool SelectSynonymTable::updateSynonymCounter(SYNONYM_INDEX synonym) {
	--synonymCounter[synonym];
	// Don't remove column for selected synonyms
	if (!selectSynonyms.count(synonym) && !synonymCounter[synonym]) {
		existingSynonyms.erase(synonym);
		removeSynonym(synonym);
	}
	return true;
}

bool SelectSynonymTable::updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	--synonymCounter[firstSynonym];
	--synonymCounter[secondSynonym];
	// Don't remove column for selected synonyms
	if (!selectSynonyms.count(firstSynonym) && !synonymCounter[firstSynonym]) {
		existingSynonyms.erase(firstSynonym);
		if (!selectSynonyms.count(secondSynonym) && !synonymCounter[secondSynonym]) {
			existingSynonyms.erase(secondSynonym);
			removeSynonyms(firstSynonym, secondSynonym);
		}
		else {
			removeSynonym(firstSynonym);
		}
	}
	else if (!selectSynonyms.count(secondSynonym) && !synonymCounter[secondSynonym]) {
		existingSynonyms.erase(secondSynonym);
		removeSynonym(secondSynonym);
	}
	return true;
}

void SelectSynonymTable::clear() {
	SynonymTable::clear();
	selectSynonyms.clear();
}

TempSynonymTable::TempSynonymTable(SYNONYM_COUNTER& synonymCounter) : SynonymTable(synonymCounter) {}

bool TempSynonymTable::updateSynonymCounter(SYNONYM_INDEX synonym) {
	--synonymCounter[synonym];
	if (!synonymCounter[synonym]) {
		existingSynonyms.erase(synonym);
		removeSynonym(synonym);
	}
	return true;
}

bool TempSynonymTable::updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	--synonymCounter[firstSynonym];
	--synonymCounter[secondSynonym];
	// Don't remove column for selected synonyms
	if (!synonymCounter[firstSynonym]) {
		existingSynonyms.erase(firstSynonym);
		if (!synonymCounter[secondSynonym]) {
			existingSynonyms.erase(secondSynonym);
			removeSynonyms(firstSynonym, secondSynonym);
		}
		else {
			removeSynonym(firstSynonym);
		}
	}
	else if (!synonymCounter[secondSynonym]) {
		existingSynonyms.erase(secondSynonym);
		removeSynonym(secondSynonym);
	}
	return true;
}

ResultSynonymTable::ResultSynonymTable(ELEMENT_LIST& elementList) : SynonymTable(), selectElements(elementList) {}

void ResultSynonymTable::init(ELEMENT_LIST& selectElements) {
	this->selectElements = selectElements;
}

void ResultSynonymTable::mergeTable(SelectSynonymTable& selectTable) {
	if (!selectTable.getSynTable().size()) {
		return;
	}

	if (synTable.empty()) {
		setSynTable(selectTable.getSynTable(), selectTable.getSelectSynonyms());
		for (auto& synoynmValuePair : selectTable.getSynTable().front()) {
			existingSynonyms.insert(synoynmValuePair.first);
		}
		return;
	}

	for (auto& synoynmValuePair : selectTable.getSynTable().front()) {
		existingSynonyms.insert(synoynmValuePair.first);
	}
	superUnion(selectTable.getSynTable());
}

void ResultSynonymTable::superUnion(SYNONYM_TABLE& synonymTable) {
	size_t newSize = existingSynonyms.size();
	auto row = synTable.begin();
	while (row != synTable.end()) {
		if ((*row).size() >= newSize) {
			break;
		}
		for (auto& otherRow : synonymTable) {
			SYNONYM_TABLE_ROW newRow = *row;
			newRow.insert(otherRow.begin(), otherRow.end());
			synTable.push_back(newRow);
		}
		row = synTable.erase(row);
	}
}

SYNONYM_INDICES ResultSynonymTable::getMissingSelectSynonyms() {
	SYNONYM_INDICES missingSynonyms;
	for (auto& element : selectElements) {
		SYNONYM_INDEX selectSynonym = element.getSynIndex();
		if (!existingSynonyms.count(selectSynonym)) {
			missingSynonyms.insert(selectSynonym);
		}
	}
	return missingSynonyms;
}

INDEXED_INTERMEDIATE_ANSWER ResultSynonymTable::getIntermediateAnswer() {
	SYNONYM_INDEX_LIST selectSynonymList;
	for (auto& element : selectElements) {
		selectSynonymList.push_back(element.getSynIndex());
	}
	VALUE_LISTS valueLists = getSynonymValuesOf(selectSynonymList);

	return INDEXED_INTERMEDIATE_ANSWER(valueLists.begin(), valueLists.end());
}

bool ResultSynonymTable::updateSynonymCounter(SYNONYM_INDEX synonym) {
	// Do nothing
	return true;
}

bool ResultSynonymTable::updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	// Do nothing
	return true;
}

void ResultSynonymTable::clear() {
	SynonymTable::clear();
	selectElements.clear();
}
