#pragma once

#include "Common.h"
#include "PKB.h"

class NextStarExtractor;
class AffectsRelationsExtractor;

class OnDemandExtractor {
public:
	OnDemandExtractor();

	bool populate();

	/* Extracts the Next* relationship for the given program line */
	void extractNextStar(INDEX progLine, RelationshipCache& cache);
	/* Extratcs the Next* relationship for ALL program lines */
	void extractNextStar(RelationshipCache& cache);

	/* Extracts the Affects relationship for the given program line */
	void extractAffects(INDEX progLine, RelationshipCache& cache);
	/* Extratcs the Affects relationship for ALL program lines */
	void extractAffects(RelationshipCache& cache);

	/* Extratcs the Affects Star relationship for ALL program lines */
	void extractAffectsStar(RelationshipCache& cache);
	/* Extratcs the a row for a program line */
	void extractsAffectsStar(INDEX prog_line ,RelationshipCache& cache);
	/* Extratcs the a column for a program line */
	void extractsReverseAffectsStar(RelationshipCache& cache, INDEX prog_line);


protected:
	PKB pkb;
	// keep track of highest program line traversed
	PROG_LINE highestNum;
	// true if populate has been called before
	bool populated;
	bool affectsStarPopulated;
	bool affectsPopulated;
};

class NextStarExtractor : public OnDemandExtractor {
public:
	NextStarExtractor(RelationshipCache& cache);

	/* Extracts the Next* relationship for the given program line */
	void extractNextStar(INDEX progLine);
	/* Extracts the Next* relationship for ALL program lines */
	void extractNextStar();

private:
	RelationshipStorage<INDEX, INDEX>& nStore;
	RelationshipCache& nStarCache;
	RelationshipStorage<INDEX, INDEX> cycleTracker;
	INDICES extractedProgLines;

	void copyNextStar(INDEX from, INDEX to);
	void replaceCycle(INDEX cycleProgLine);

	void printStatus(INDEX index);
	VAR getProgString(INDEX index);
};


class AffectsExtractor : public OnDemandExtractor {
public:

	AffectsExtractor(RelationshipCache& cache);

	// Extracts the affects relationship for ALL program lines 
	void extractAffects();
	/* Extracts the affects relationship for the given program line */
	void extractAffects(INDEX progLine);

private:
	RelationshipCache& aCache;
	TNode root;

	bool isAssignment(INDEX i);
	bool isRead(INDEX i);
	bool isCall(INDEX i);
	VALUES getModifies(TNode* node);
	VALUES getUses(TNode* node);
	void traverseAffects(TNode& proc, unordered_map<VALUE, unordered_set<int>>* modifedHistory);
	void traverseAffects(INDEX s);
};

class AffectsStarExtractor : public OnDemandExtractor {

public:

	AffectsStarExtractor(RelationshipCache& cache);

	//Extratcs affects* relationship for ALL program lines
	void extractAffectsStar();
	void extractAffectsStar(INDEX prog_line);
	void extractReverseAffectsStar(INDEX prog_line);

private:
	RelationshipCache& asCache;
	void traverseAffectsStar();
	void traverseAffectsStar(INDEX prog_line);
	void traverseReverseAffectsStar(INDEX prog_line);
};
