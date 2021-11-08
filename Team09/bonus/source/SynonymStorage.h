#pragma once

#include "Common.h"
#include "MiniTable.h"
#include "QuerySelectResult.h"

typedef vector<MiniSingleTable> SINGLE_TABLE_LIST;
typedef vector<MiniDoubleTable> DOUBLE_TABLE_LIST;
typedef unordered_map<SYNONYM_INDEX, int> SYNONYM_COUNTER;
typedef vector<VALUE_LIST> INTERMEDIATE_ANSWER;
typedef unordered_map<SYNONYM_INDEX, stack<VALUE>> SYNONYM_VALUE_TRACKER;

class SynonymStorage {
public:
	SynonymStorage(SYNONYM_COUNTER& synonymCounter = SYNONYM_COUNTER());

	/* Retrieves the list of single tables in the storage */
	SINGLE_TABLE_LIST getSingleTables();
	/* Retrieves the list of double tables in the storage */
	DOUBLE_TABLE_LIST getDoubleTables();

	/* Merges list of possible values for one synonym into the table */
	bool mergeWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Merges list of possible values for two synonyms into the table */
	bool mergeWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs);
	/* Adds a single table into the storage*/
	void addSingleTable(MiniSingleTable& singleTable, SYNONYM_INDEX synonym);
	/* Adds a double table into the storage*/
	void addDoubleTable(MiniDoubleTable& doubleTable, SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	/* Retrives the index of the single table that stores the values of the given synonym */
	INDEX getSingleTableIndexOf(SYNONYM_INDEX synonym);
	/* Retrives the index of the double table that stores the values of the given synonym pair */
	INDEX getDoubleTableIndexOf(PAIR<SYNONYM_INDEX> synonymPair);
	/* Retrives the indices of the double tables that store the values of the given synonym */
	INDICES getDoubleTableIndexOf(SYNONYM_INDEX synonym);

	/* Retrieves the values from the table corresponding to the synonym */
	VALUES& getSynonymValuesOf(SYNONYM_INDEX synonym);
	/* Retrieves the ordered values from the table corresponding to both synonyms */
	VALUE_PAIRS getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	/* Checks if table contains the synonym */
	bool containsSynonym(SYNONYM_INDEX synonym);

	virtual void clear();

protected:
	INDEX tableIndex;
	M<INDEX, MiniSingleTable> singleTables;
	M<INDEX, MiniDoubleTable> doubleTables;
	M<SYNONYM_INDEX, INDEX> synonymToSingleTableIndex;
	M<PAIR<SYNONYM_INDEX>, INDEX> synonymPairToDoubleTableIndex;
	M<SYNONYM_INDEX, INDICES> synonymToDoubleTableIndex;
	SYNONYM_COUNTER synonymCounter;
	VALUES emptyValues;
	VALUE_PAIRS emptyValuePairs;

private:
	/* Unions list of possible values for one synonym into the storage (Synonym does not exist in the storage) */
	bool unionWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Unions list of possible values for two synonyms into the storage (Both synonyms do not exist in the storage) */
	bool unionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs);
	/* Intersects list of possible values for one synonym into the storage (Synonym exists in the storage) */
	bool intersectWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Intersects list of possible values for two synonym into the storage (Both synonyms exist in the storage) */
	bool intersectWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs);
	/* Intersects then unions list of possible values for two synonyms into the storage (One synonym exists and one synonym does not exist in the storage) */
	bool intersectUnionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuePairs, bool isIntersectFirst);
	/* Creates connecting tables between two synonym groups based on the values from the two synonyms */
	void createConnectingTables(SYNONYM_INDEX synonym, SYNONYM_INDEX referenceSynonym);
	
	/* Removes all values corresponding to the given synonym */
	void removeSynonym(SYNONYM_INDEX synonym);
	/* Verifies if the synonym can be removed */
	bool canRemoveSynonym(SYNONYM_INDEX synonym);
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX synonym);
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	
	/* Verifies if the given synonym is a select synonym */
	virtual bool isSelectSynonym(SYNONYM_INDEX synonym) = 0;
};

class SelectSynonymStorage : public SynonymStorage {
public:
	SelectSynonymStorage(SYNONYM_COUNTER& synonymCounter = SYNONYM_COUNTER(), SYNONYM_INDICES& selectSynonyms = SYNONYM_INDICES());
	
	/* Retrieves the select synonyms*/
	SYNONYM_INDICES& getSelectSynonyms();
	/* Clears the storage */
	void clear();

private:
	SYNONYM_INDICES selectSynonyms;

	/* Verifies if the given synonym is a select synonym */
	bool isSelectSynonym(SYNONYM_INDEX synonym);
};

class TempSynonymStorage : public SynonymStorage {
public:
	TempSynonymStorage(SYNONYM_COUNTER& synonymCounter = SYNONYM_COUNTER());

	/* Clears the storage */
	void clear();

private:
	/* Verifies if the given synonym is a select synonym */
	bool isSelectSynonym(SYNONYM_INDEX synonym);
};

class ResultSynonymStorage : public SynonymStorage {
public:
	ResultSynonymStorage(ELEMENT_LIST& selectElements = ELEMENT_LIST());

	/* Initialises the storage */
	void init(ELEMENT_LIST& selectElements);
	/* Merges another synonym table into the table */
	void mergeTable(SelectSynonymStorage& storage);
	/* Retrives the select synonyms absent in the table */
	SYNONYM_INDICES getMissingSelectSynonyms();
	/* Retrieves the list of intermediate results */
	INDEXED_INTERMEDIATE_ANSWER getIntermediateAnswer();
	/* Clears the storage */
	void clear();

private:
	ELEMENT_LIST selectElements;
	INDEXED_INTERMEDIATE_ANSWER intermediateAnswer;

	/* Populates the intermediate answer */
	void generateIntermediateAnswer(INDEX index, VALUE_LIST& intermediateResult, SYNONYM_VALUE_TRACKER& synonymValues);
	/* Updates the intermediate results and values */
	void updateIntermediateResult(SYNONYM_INDEX synonym, VALUE value, INDEX index, VALUE_LIST& intermediateResult, SYNONYM_VALUE_TRACKER& synonymValues);
	/* Verifies if the given synonym is a select synonym */
	bool isSelectSynonym(SYNONYM_INDEX synonym);
};
