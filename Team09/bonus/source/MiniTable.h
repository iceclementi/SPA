#pragma once

#include "Common.h"

class MiniTable {
public:
	/* Retrieves the values of the given synonym */
	virtual VALUES& getSynonymValuesOf(SYNONYM_INDEX synonym) = 0;

protected:
	VALUES emptyValues;
	VALUE_PAIRS emptyValuePairs;
};

class MiniSingleTable : public MiniTable {
public:
	MiniSingleTable();
	MiniSingleTable(SYNONYM_INDEX synonym, VALUES& values);

	/* Retrieves the synonym */
	SYNONYM_INDEX getSynonym();
	/* Retrieves the values of the given synonym */
	VALUES& getSynonymValuesOf(SYNONYM_INDEX synonym);
	/* Intersects list of possible values into the table and keep track of values erased */
	bool intersectWith(SYNONYM_INDEX synonym, VALUES& values, VALUES& erasedValues);

private:
	SYNONYM_INDEX synonym;
	VALUES table;
};

class MiniDoubleTable : public MiniTable {
public:
	MiniDoubleTable();
	MiniDoubleTable(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& values);

	/* Retrieves the synonyms */
	PAIR<SYNONYM_INDEX> getSynonyms();
	/* Retrieves the other synonym in the table */
	SYNONYM_INDEX getOtherSynonym(SYNONYM_INDEX referenceSynonym);
	/* Retrieves the values of the given synonym */
	VALUES& getSynonymValuesOf(SYNONYM_INDEX synonym);
	/* Retrieves the pairs of values of the given synonyms */
	VALUE_PAIRS getSynonymValuesOf(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	/* Retrieves the values of the given synonym when the other synonym is the given value */
	VALUES getSynonymValuesOf(SYNONYM_INDEX synonym, SYNONYM_INDEX otherSynonym, VALUE otherValue);
	/* Retrieves the values of the given synonym when the other synonym is one of the given values */
	VALUES getSynonymValuesOf(SYNONYM_INDEX synonym, SYNONYM_INDEX otherSynonym, VALUES& otherValues);
	/* Intersects list of possible values for one synonym into the table */
	bool intersectWith(SYNONYM_INDEX synonym, VALUES& values);
	/* Intersects list of possible values for both synonym into the table */
	bool intersectWith(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym, VALUE_PAIRS& values);
	/* Removes a list of values of a given synonym from the table */
	void removeSynonymValues(SYNONYM_INDEX synonym, VALUES& valuesToErase);

private:
	M<SYNONYM_INDEX, INDEX> indexedSynonyms;
	VALUE_PAIRS table;
	VALUES firstSynonymValues;
	VALUES secondSynonymValues;

	/* Updates the stored list of values */
	void updateStoredValues();
	/* Retrieves the index of the synonym in the table */
	INDEX getIndexOfSynonym(SYNONYM_INDEX synonym);
};

class TableUtils {
public:
	/* Constructs a permutation of the two list of values */
	static VALUE_PAIRS makeValuePairs(VALUES& firstValues, VALUES& secondValues);
	/* Splits pairs of values into two separate list of values */
	static PAIR<VALUES> splitPairedValues(VALUE_PAIRS& valuePairs);
	/* Forms a pair of synonym ordered in lexographic order */
	static PAIR<SYNONYM_INDEX> orderSynonyms(SYNONYM_INDEX firstSynonym, SYNONYM_INDEX secondSynonym);
	/* Retrieves the value of a pair at the given index */
	static VALUE getValueOfPairAt(VALUE_PAIR pair, INDEX index);
	/* Creates a new list of values by intersecting two lists of values */
	static VALUES intersectValues(VALUES& values, VALUES& intersectValues);
};
