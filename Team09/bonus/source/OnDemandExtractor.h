#pragma once

#include "Common.h"
#include "PKB.h"

class NextStarExtractor;
class AffectsRelationsExtractor;

class OnDemandExtractor {
public:
	OnDemandExtractor();

	/* Extracts the Next* relationship for the given program line */
	void extractNextStar(INDEX progLine, RelationshipCache& cache);
	/* Extracts the Next* relationship for ALL program lines */
	void extractNextStar(RelationshipCache& cache);
	/* Extracts the Affects relationship for the given program line */
	void extractAffects(INDEX progLine, RelationshipCache& cache);
	/* Extracts the Affects relationship for ALL program lines */
	void extractAffects(RelationshipCache& cache);
	/* Extracts the Affects Star relationship for ALL program lines */
	void extractAffectsStar(RelationshipCache& cache);
	/* Extratcs the a row for a program line */
	void extractsAffectsStar(INDEX prog_line ,RelationshipCache& cache);
	/* Extratcs the a column for a program line */
	void extractsReverseAffectsStar(RelationshipCache& cache, INDEX prog_line);

/*-------------------------------------*\
|               EXTENSION               |
\*-------------------------------------*/

	/* Extracts the NextBip relationship for ALL program lines */
	void extractNextBip(RelationshipBipCache& cache);
	/* Extracts the NextBip* relationship for ALL program lines */
	void extractNextBipStar(RelationshipBipCache& cache);
	/* Extracts the AffectsBip relationship for ALL program lines */
	void extractAffectsBip();
	/* Extracts the AffectsBip* relationship for ALL program lines */
	void extractAffectsBipStar();

protected:
	PKB pkb;
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

	/* Copies the Next* relationships to from one program line to another */
	void copyNextStar(INDEX from, INDEX to);
	/* Replace cycle with values from the program line */
	void replaceCycle(INDEX cycleProgLine);
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

/*-------------------------------------*\
|               EXTENSION               |
\*-------------------------------------*/

class NextBipExtractor : public OnDemandExtractor {
public:
	NextBipExtractor(RelationshipBipCache& cache);

	/* Extracts the NextBip relationship for ALL program lines */
	void extractNextBip();

private:
	RelationshipStorage<INDEX, INDEX>& nStore;
	RelationshipBipCache& nBipCache;
	StatementStorage& stmtStore;

	/* Create a mapping for each procedure to the first and last lines in the procedure */
	void initProcedures();
	/* Extracts the intermediate NextBip relationship in the given procedure */
	void extractNextBip(INDEX procedure, INDEX progLine, INDICES& calledProcedures);
	/* Create an ordering of the procedures in the program according to calling dependency */
	INDEX_LIST generateOrderedProcedures();
	/* Retrives the procedure containing the given line */
	INDEX getProcedureContainingLine(INDEX line);
	/* Replaces the call statement with a returning line if it is the last line */
	void replaceLastLinesOf(INDEX procedure);
};

class NextBipStarExtractor : public OnDemandExtractor {
public:
	NextBipStarExtractor(RelationshipBipCache& cache);

	/* Extracts the Next* relationship for ALL program lines */
	void extractNextBipStar();

private:
	RelationshipStorage<INDEX, INDEX>& nStore;
	RelationshipBipCache& nBipStarCache;
	StatementStorage& stmtStore;
	RelationshipStorage<INDEX, INDEX> cycleTracker;
	INDEX startLine;
	INDICES extractedProgLines;

	/* Extracts the Next Bip* relationship for the given procedure */
	void extractNextBipStar(INDEX procedure);
	/* Extracts the Next Bip* relationship for the given program line in the procedure */
	void extractNextBipStar(INDEX procedure, INDEX progLine);
	/* Extracts the Next Bip* relationship for the given program line */
	void recurseExtractNextBipStar(INDEX progLine);
	/* Copies the Next Bip* relationships to from one program line to another */
	void copyNextBipStar(INDEX from, INDEX to);
	/* Copies the Next Bip* relationships to from one procedure to a program line */
	void copyProcedureToLine(INDEX procedure, INDEX progLine);
	/* Copies the Next Bip* relationships to from one program line to a procedure */
	void copyLineToProcedure(INDEX progLine, INDEX procedure);
	/* Replace cycle with values from the program line */
	void replaceCycle(INDEX cycleProgLine);
	/* Create an ordering of the procedures in the program according to calling dependency */
	INDEX_LIST generateOrderedProcedures();
};

class AffectsBipExtractor : public OnDemandExtractor {
public:
	AffectsBipExtractor();

	/* Extracts both AffectsBip and AffectsBip* relationship */
	void extractAffectsBip();

private:
	map<PROC_NAME, TNode> procMap;
	NODE_LIST getProcs();
	void traverseAffects(TNode& node, unordered_map<VALUE, unordered_set<pair<int,int>>>* modifiedHistory, 
		vector<pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>>>* affected, int currentBranch);
	VALUES getModifies(TNode* node);
	VALUES getUses(TNode* node);
	void helperAffectsStar(vector<pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>>>* affected);
	int branchCount;
};
