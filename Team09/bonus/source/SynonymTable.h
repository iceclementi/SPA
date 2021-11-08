#pragma once

#include "Common.h"
#include "QuerySelectResult.h"

typedef unordered_map<SYNONYM_INDEX, int> SYNONYM_COUNTER;
typedef unordered_map<VALUE, VALUES> VALUES_MAP;
typedef map<SYNONYM_INDEX, VALUE> SYNONYM_TABLE_ROW;
typedef list<SYNONYM_TABLE_ROW> SYNONYM_TABLE;
typedef vector<VALUE_LIST> INDEXED_INTERMEDIATE_ANSWER;

class SynonymTable {
public:
	SynonymTable(SYNONYM_COUNTER& synonymCounter = SYNONYM_COUNTER());

	SYNONYM_TABLE& getSynTable();
	void setSynTable(SYNONYM_TABLE& tableToSet, SYNONYM_INDICES& synonyms);

	/* Merges list of possible values for one synonym into the table */
	bool mergeWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Merges list of possible values for two synonyms into the table */
	bool mergeWith(SYNONYM_INDEX firstSyn, SYNONYM_INDEX secondSyn, VALUE_PAIRS& valuesPair);

	/* Retrieves the values from the table corresponding to the synonym */
	VALUES getSynonymValuesOf(SYNONYM_INDEX synonym);
	/* Retrieves the ordered values from the table corresponding to both synonyms */
	VALUE_PAIRS getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	/* Retrieves the ordered values from the table corresponding to the list of synonyms */
	VALUE_LISTS getSynonymValuesOf(SYNONYM_INDEX_LIST& synonyms);
	/* Checks if table contains the synonym */
	bool containsSynonym(SYNONYM_INDEX synonym);

	void clear();

protected:
	SYNONYM_TABLE synTable;
	SYNONYM_COUNTER synonymCounter;
	SYNONYM_INDICES existingSynonyms;

	/* Updates the synonym counter after merge */
	virtual bool updateSynonymCounter(SYNONYM_INDEX synonym) = 0;
	/* Updates the synonym counter after merge */
	virtual bool updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym) = 0;
	/* Removes a column from the table corresponding to the synonym */
	void removeSynonym(SYNONYM_INDEX synonym);
	/* Removes two columns from the table corresponding to the two synonyms */
	void removeSynonyms(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);

private:
	/* Intersects list of possible values for one synonym into the table (Synonym exists in the table) */
	void intersectWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Intersects list of possible values for two synonym into the table (Both synonyms exist in the table) */
	void intersectWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuesPair);
	/* Unions list of possible values for one synonym into the table (Synonym does not exist in the table) */
	void unionWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Unions list of possible values for two synonyms into the table (Both synonyms do not exist in the table) */
	void unionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& valuesPair);
	/* Intersects then unions list of possible values for two synonyms into the table 
	(One synonym exists and one synonym does not exist in the table) */
	void intersectUnionWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUES_MAP& valuesMap);
	/* Safely retrieves the value of the synonym of a given row in the table */
	VALUE getSynonymValueAt(SYNONYM_TABLE_ROW& row, SYNONYM_INDEX synonym);
	/* Converts paired values into a map */
	VALUES_MAP mapValuePairs(VALUE_PAIRS& valuesPair, bool isMaptoSecond);
};

class SelectSynonymTable : public SynonymTable {
public:
	SelectSynonymTable(SYNONYM_COUNTER& synonymCounter = SYNONYM_COUNTER(), SYNONYM_INDICES& selectSynonyms = SYNONYM_INDICES());
	
	SYNONYM_INDICES& getSelectSynonyms();
	void clear();

private:
	SYNONYM_INDICES selectSynonyms;
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX synonym);
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
};

class TempSynonymTable : public SynonymTable {
public:
	TempSynonymTable(SYNONYM_COUNTER& synonymCounter = SYNONYM_COUNTER());

private:
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX synonym);
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
};

class ResultSynonymTable : public SynonymTable {
public:
	ResultSynonymTable(ELEMENT_LIST& selectElements = ELEMENT_LIST());


	/* Initialises the select elements */
	void init(ELEMENT_LIST& selectElements);
	/* Merges another synonym table into the table */
	void mergeTable(SelectSynonymTable& selectTable);
	/* Retrives the select synonyms absent in the table */
	SYNONYM_INDICES getMissingSelectSynonyms();
	/* Retrieves the list of intermediate results */
	INDEXED_INTERMEDIATE_ANSWER getIntermediateAnswer();
	void clear();

private:
	ELEMENT_LIST selectElements;

	/* Unions another synonym table into the table */
	void superUnion(SYNONYM_TABLE& synonymTable);
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX synonym);
	/* Updates the synonym counter after merge */
	bool updateSynonymCounter(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
};
