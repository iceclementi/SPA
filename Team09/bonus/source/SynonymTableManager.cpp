#include "SynonymStorageManager.h"

SYNONYM_STORAGE_MODE SynonymStorageManager::storageMode = SYNONYM_STORAGE_MODE::TEMP;
SELECT_STORAGE SynonymStorageManager::selectSynonymStorage = SELECT_STORAGE();
TEMP_STORAGE SynonymStorageManager::tempSynonymStorage = TEMP_STORAGE();
RESULT_STORAGE SynonymStorageManager::resultSynonymStorage = RESULT_STORAGE();

SynonymStorageManager::SynonymStorageManager() {}

void SynonymStorageManager::setSynonymStorage(SELECT_STORAGE& synonymTable) {
	storageMode = SYNONYM_STORAGE_MODE::SELECT;
	selectSynonymStorage = synonymTable;
}

void SynonymStorageManager::setSynonymStorage(TEMP_STORAGE& synonymTable) {
	storageMode = SYNONYM_STORAGE_MODE::TEMP;
	tempSynonymStorage = synonymTable;
}

void SynonymStorageManager::setSynonymStorage(ELEMENT_LIST& selectElements) {
	storageMode = SYNONYM_STORAGE_MODE::RESULT;
	resultSynonymStorage.init(selectElements);
}

bool SynonymStorageManager::mergeWith(SYNONYM_INDEX synonym, VALUES& values) {
	switch (storageMode) {
	case SYNONYM_STORAGE_MODE::SELECT:
		return selectSynonymStorage.mergeWith(synonym, values);
	case SYNONYM_STORAGE_MODE::TEMP:
		return tempSynonymStorage.mergeWith(synonym, values);
	case SYNONYM_STORAGE_MODE::RESULT:
		return resultSynonymStorage.mergeWith(synonym, values);
	default:
		return false;
	}
}

bool SynonymStorageManager::mergeWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs) {
	switch (storageMode) {
	case SYNONYM_STORAGE_MODE::SELECT:
		return selectSynonymStorage.mergeWith(firstSynonym, secondSynonym, valuePairs);
	case SYNONYM_STORAGE_MODE::TEMP:
		return tempSynonymStorage.mergeWith(firstSynonym, secondSynonym, valuePairs);
	case SYNONYM_STORAGE_MODE::RESULT:
		return resultSynonymStorage.mergeWith(firstSynonym, secondSynonym, valuePairs);
	default:
		return false;
	}
}

VALUES SynonymStorageManager::getSynonymValuesOf(SYNONYM_INDEX synonym) {
	switch (storageMode) {
	case SYNONYM_STORAGE_MODE::SELECT:
		return selectSynonymStorage.getSynonymValuesOf(synonym);
	case SYNONYM_STORAGE_MODE::TEMP:
		return tempSynonymStorage.getSynonymValuesOf(synonym);
	default:
		return VALUES();
	}
}

VALUE_PAIRS SynonymStorageManager::getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) {
	switch (storageMode) {
	case SYNONYM_STORAGE_MODE::SELECT:
		return selectSynonymStorage.getSynonymValuesOf(firstSynonym, secondSynonym);
	case SYNONYM_STORAGE_MODE::TEMP:
		return tempSynonymStorage.getSynonymValuesOf(firstSynonym, secondSynonym);
	default:
		return VALUE_PAIRS();
	}
}

void SynonymStorageManager::mergeToResult() {
	resultSynonymStorage.mergeTable(selectSynonymStorage);
}

SYNONYM_INDICES SynonymStorageManager::getMissingSelectSynonyms() {
	return resultSynonymStorage.getMissingSelectSynonyms();
}

INDEXED_INTERMEDIATE_ANSWER SynonymStorageManager::getIntermediateAnswer() {
	return resultSynonymStorage.getIntermediateAnswer();
}

bool SynonymStorageManager::containsSynonym(SYNONYM_INDEX synonym) {
	switch (storageMode) {
	case SYNONYM_STORAGE_MODE::SELECT:
		return selectSynonymStorage.containsSynonym(synonym);
	case SYNONYM_STORAGE_MODE::TEMP:
		return tempSynonymStorage.containsSynonym(synonym);
	default:
		return false;
	}
}

void SynonymStorageManager::clear() {
	selectSynonymStorage.clear();
	tempSynonymStorage.clear();
	resultSynonymStorage.clear();
}
