#include "MiniTable.h"

#pragma region Mini Single Table

MiniSingleTable::MiniSingleTable() : synonym(0) {}

MiniSingleTable::MiniSingleTable(SYNONYM_INDEX synonym, VALUES& values) : synonym(synonym), table(values) {}

SYNONYM_INDEX MiniSingleTable::getSynonym() {
	return synonym;
}

VALUES& MiniSingleTable::getSynonymValuesOf(SYNONYM_INDEX synonym) {
	return synonym == this->synonym ? table : emptyValues;
}

bool MiniSingleTable::intersectWith(SYNONYM_INDEX synonym, VALUES& values, VALUES& erasedValues) {
	if (synonym == this->synonym) {
		// Delete rows that do not contain the synonym value
		auto row = table.begin();
		while (row != table.end()) {
			VALUE synonymValue = *row;
			if (!values.count(synonymValue)) {
				erasedValues.insert(synonymValue);
				row = table.erase(row);
				continue;
			}
			++row;
		}
		return !table.empty();
	}
	else {
		return false;
	}
}

#pragma endregion

#pragma region Mini Double Table

MiniDoubleTable::MiniDoubleTable() {}

MiniDoubleTable::MiniDoubleTable(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs) {
	indexedSynonyms.insert({ { firstSynonym, -1 }, { secondSynonym, 1 } });
	for (auto& valuePair : valuePairs) {
		table.insert({ valuePair.first, valuePair.second });
		firstSynonymValues.insert(valuePair.first);
		secondSynonymValues.insert(valuePair.second);
	}
}

PAIR<SYNONYM_INDEX> MiniDoubleTable::getSynonyms() {
	SYNONYM_INDEX firstSynonym = getOtherSynonym(0);
	SYNONYM_INDEX secondSynonym = getOtherSynonym(firstSynonym);
	return { firstSynonym, secondSynonym };
}

SYNONYM_INDEX MiniDoubleTable::getOtherSynonym(SYNONYM_INDEX referenceSynonym) {
	for (auto& synonym : indexedSynonyms) {
		if (synonym.first != referenceSynonym) {
			return synonym.first;
		}
	}
	return referenceSynonym;
}

VALUES& MiniDoubleTable::getSynonymValuesOf(SYNONYM_INDEX synonym) {
	INDEX index = getIndexOfSynonym(synonym);
	if (!index) return emptyValues;
	return index > 0 ? secondSynonymValues : firstSynonymValues;
}

VALUE_PAIRS MiniDoubleTable::getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	INDEX firstIndex = getIndexOfSynonym(firstSynonym), secondIndex = getIndexOfSynonym(secondSynonym);
	if (!firstIndex || !secondIndex) return emptyValuePairs;

	VALUE_PAIRS valuePairs;
	for (auto& row : table) {
		VALUE firstValue = TableUtils::getValueOfPairAt(row, firstIndex);
		VALUE secondValue = TableUtils::getValueOfPairAt(row, secondIndex);
		valuePairs.insert({ firstValue, secondValue });
	}
	return valuePairs;
}

VALUES MiniDoubleTable::getSynonymValuesOf(SYNONYM_INDEX synonym, SYNONYM_INDEX otherSynonym, VALUE otherValue) {
	INDEX index = getIndexOfSynonym(synonym), otherIndex = getIndexOfSynonym(otherSynonym);
	if (!index || !otherIndex) return emptyValues;

	VALUES values;
	for (auto& row : table) {
		if (TableUtils::getValueOfPairAt(row, otherIndex) == otherValue) {
			values.insert(TableUtils::getValueOfPairAt(row, index));
		}
	}
	return values;
}

VALUES MiniDoubleTable::getSynonymValuesOf(SYNONYM_INDEX synonym, SYNONYM_INDEX otherSynonym, VALUES& otherValues) {
	INDEX index = getIndexOfSynonym(synonym), otherIndex = getIndexOfSynonym(otherSynonym);
	if (!index || !otherIndex) return emptyValues;

	VALUES values;
	for (auto& row : table) {
		if (otherValues.count(TableUtils::getValueOfPairAt(row, otherIndex))) {
			values.insert(TableUtils::getValueOfPairAt(row, index));
		}
	}
	return values;
}

bool MiniDoubleTable::intersectWith(SYNONYM_INDEX synonym, VALUES& values) {
	INDEX index = getIndexOfSynonym(synonym);
	if (!index) return false;

	// Delete rows that do not contain the synonym value
	auto row = table.begin();
	while (row != table.end()) {
		VALUE synonymValue = TableUtils::getValueOfPairAt(*row, index);
		if (!values.count(synonymValue)) {
			row = table.erase(row);
			continue;
		}
		++row;
	}
	updateStoredValues();
	return !table.empty();
}

bool MiniDoubleTable::intersectWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs) {
	INDEX firstIndex = getIndexOfSynonym(firstSynonym), secondIndex = getIndexOfSynonym(secondSynonym);
	if (!firstIndex || !secondIndex) return false;

	// Delete rows that do not contain the synonym values
	auto row = table.begin();
	while (row != table.end()) {
		VALUE firstSynonymValue = TableUtils::getValueOfPairAt(*row, firstIndex);
		VALUE secondSynonymValue = TableUtils::getValueOfPairAt(*row, secondIndex);
		if (!valuePairs.count({ firstSynonymValue, secondSynonymValue })) {
			row = table.erase(row);
			continue;
		}
		++row;
	}
	updateStoredValues();
	return !table.empty();
}

void MiniDoubleTable::removeSynonymValues(SYNONYM_INDEX synonym, VALUES& valuesToErase) {
	INDEX index = getIndexOfSynonym(synonym);
	if (!index) return;

	auto row = table.begin();
	while (row != table.end()) {
		VALUE synonymValue = TableUtils::getValueOfPairAt(*row, index);
		if (valuesToErase.count(synonymValue)) {
			row = table.erase(row);
			continue;
		}
		++row;
	}
	updateStoredValues();
}

void MiniDoubleTable::updateStoredValues() {
	PAIR<VALUES> newSynonymValues = TableUtils::splitPairedValues(table);
	firstSynonymValues = newSynonymValues.first;
	secondSynonymValues = newSynonymValues.second;
}

INDEX MiniDoubleTable::getIndexOfSynonym(SYNONYM_INDEX synonym) {
	return getValueWithKey(indexedSynonyms, synonym);
}

#pragma endregion

#pragma region Table Utilities

VALUE_PAIRS TableUtils::makeValuePairs(VALUES& firstValues, VALUES& secondValues) {
	VALUE_PAIRS valuePairs;
	for (auto& firstValue : firstValues) {
		for (auto& secondValue : secondValues) {
			valuePairs.insert({ firstValue, secondValue });
		}
	}
	return valuePairs;
}

PAIR<VALUES> TableUtils::splitPairedValues(VALUE_PAIRS& valuePairs) {
	PAIR<VALUES> valuesPair;
	for (auto& valuePair : valuePairs) {
		valuesPair.first.insert(valuePair.first);
		valuesPair.second.insert(valuePair.second);
	}
	return valuesPair;
}

PAIR<SYNONYM_INDEX> TableUtils::orderSynonyms(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	return firstSynonym < secondSynonym ? PAIR<SYNONYM_INDEX>(firstSynonym, secondSynonym) : PAIR<SYNONYM_INDEX>(secondSynonym, firstSynonym);
}

VALUE TableUtils::getValueOfPairAt(VALUE_PAIR pair, INDEX index) {
	return index > 0 ? pair.second : pair.first;
}

VALUES TableUtils::intersectValues(VALUES& values, VALUES& intersectValues) {
	if (values.empty()) return intersectValues;

	VALUES intersectedValues;
	for (auto& value : values) {
		if (intersectValues.count(value)) {
			intersectedValues.insert(value);
		}
	}
	return intersectedValues;
}

#pragma endregion
