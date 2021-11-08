#pragma once

#include "Common.h"
#include "PKBStorage.h"

class OnDemandExtractor;

class RelationshipCache : public RelationshipStorage<INDEX, INDEX> {
public:
	RelationshipCache(RELATIONSHIP_TYPE rType);

	void setPopulated(bool populated);
	bool isPopulated();

	/* Populate a reverse value in the cache */
	void populateReverseCache(INDEX value);
	/* Populates a specific value in the cache */
	virtual void populateCache(INDEX index);
	/* Populates the entire cache */
	virtual void populateCache();
	/* Adds the index of a value whose relationship is fully evaluated into a list */
	void addPopulatedValue(INDEX populatedValue);
	/* Adds the index of a value whose reverse relationship is fully evaluated into a list */
	void addReversePopulatedValue(INDEX reversePopulatedValue);
	/* Verifies if a given value in the cache has been populated */
	bool isValuePopulated(INDEX value);
	/* Verifies if a given value in the cache has been reverse populated */
	bool isValueReversePopulated(INDEX value);

	/* Retrieves all the first values */
	INDICES& getFirstValues();
	/* Retrieves all the second values */
	INDICES& getSecondValues();
	/* Retrieves all the relationships as pairs */
	PAIRS<INDEX> getValuePairs();
	/* Retrieves all the first values of a relationship given the second value */
	INDICES& getFirstValuesOf(INDEX secondValue);
	/* Retrieves all the second values of a relationship given the first value */
	INDICES& getSecondValuesOf(INDEX firstValue);

	/* Verifies if the forward relationship between the first and second values exists */
	bool hasRelationship(INDEX firstValue, INDEX secondValue);
	/* Verifies if any relationship exists with the given second value */
	bool hasRelationshipWithSecondValue(INDEX secondValue);
	/* Verifies if any relationship exists with the given first value */
	bool hasRelationshipWithFirstValue(INDEX firstValue);

	virtual void clear();

protected:
	static OnDemandExtractor ode;
	bool populated;
	INDICES populatedValues;
	INDICES reversePopulatedValues;
	bool isPopulating;
	RELATIONSHIP_TYPE rType;
};

class RelationshipBipCache : public RelationshipCache {
public:
	RelationshipBipCache(RELATIONSHIP_TYPE rType);

	/* Populates the entire cache */
	void populateCache();

	void addFirstLineOfProcedure(INDEX procedure, INDEX progLine);
	void addLastLineOfProcedure(INDEX procedure, INDEX progLine);
	void replaceLastLineOfProcedure(INDEX procedure, INDEX oldProgLine, INDEX newProgLine);
	void addLineToProcedure(INDEX procedure, INDEX progLine);

	INDEX getFirstLineOf(INDEX procedure);
	INDICES& getLastLinesOf(INDEX procedure);
	INDICES& getLinesOf(INDEX procedure);

	void clear();

private:
	struct PROC_STMT_INFO {
		INDEX firstLine;
		INDICES lastLines;
		INDICES lines;
	};
	static M<INDEX, PROC_STMT_INFO> procedureInfo;
};
