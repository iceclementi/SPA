#pragma once

#include "SynonymTable.h"
#include "SynonymStorage.h"

//typedef SelectSynonymTable SELECT_STORAGE;
//typedef TempSynonymTable TEMP_STORAGE;
//typedef ResultSynonymTable RESULT_STORAGE;

typedef SelectSynonymStorage SELECT_STORAGE;
typedef TempSynonymStorage TEMP_STORAGE;
typedef ResultSynonymStorage RESULT_STORAGE;

enum class SYNONYM_STORAGE_MODE {
	SELECT, TEMP, RESULT
};

class SynonymStorageManager {
public:
	SynonymStorageManager();

	static void setSynonymStorage(SELECT_STORAGE& synonymTable);
	static void setSynonymStorage(TEMP_STORAGE& synonymTable);
	static void setSynonymStorage(ELEMENT_LIST& selectElements);

	/* Merges list of possible values for one synonym into the table */
	static bool mergeWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Merges list of possible values for two synonyms into the table */
	static bool mergeWith(SYNONYM_INDEX firstSyn, SYNONYM_INDEX secondSyn, VALUE_PAIRS& valuesPair);
	/* Retrieves the values from the table corresponding to the synonym */
	static VALUES getSynonymValuesOf(SYNONYM_INDEX synonym);
	/* Retrieves the ordered values from the table corresponding to both synonyms */
	static VALUE_PAIRS getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	/* Checks if table contains the synonym */
	static bool containsSynonym(SYNONYM_INDEX synonym);

	/* Merge the select table into the result table */
	static void mergeToResult();
	/* Retrives the select synonyms absent in the table */
	static SYNONYM_INDICES getMissingSelectSynonyms();
	/* Retrieves the list of intermediate results */
	static INDEXED_INTERMEDIATE_ANSWER getIntermediateAnswer();

	static void clear();

private:
	static SYNONYM_STORAGE_MODE storageMode;
	static SELECT_STORAGE selectSynonymStorage;
	static TEMP_STORAGE tempSynonymStorage;
	static RESULT_STORAGE resultSynonymStorage;
};
