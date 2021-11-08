#pragma once

#include "SuchThatClause.h"
#include "QueryOptimiser.h"

static int BASE_PRIORITY_VALUE = 100;

static unordered_map<SUCH_THAT_REL_TYPE, int> singleSynonymSuchThatClausePriorityValueMap = {
	{ SUCH_THAT_REL_TYPE::FOLLOWS, 3 },
	{ SUCH_THAT_REL_TYPE::FOLLOWS_T, 12 },
	{ SUCH_THAT_REL_TYPE::PARENT, 4 },
	{ SUCH_THAT_REL_TYPE::PARENT_T, 13 },
	{ SUCH_THAT_REL_TYPE::MODIFIES, 6 },
	{ SUCH_THAT_REL_TYPE::USES, 7 },
	{ SUCH_THAT_REL_TYPE::CALLS, 1 },
	{ SUCH_THAT_REL_TYPE::CALLS_T, 11 },
	{ SUCH_THAT_REL_TYPE::NEXT, 5 },
	{ SUCH_THAT_REL_TYPE::NEXT_T, 27 },
	{ SUCH_THAT_REL_TYPE::AFFECTS, 28 },
	{ SUCH_THAT_REL_TYPE::AFFECTS_T, 29 },
	{ SUCH_THAT_REL_TYPE::NEXTBIP, 30 },
	{ SUCH_THAT_REL_TYPE::NEXTBIP_T, 31 },
	{ SUCH_THAT_REL_TYPE::AFFECTSBIP, 32 },
	{ SUCH_THAT_REL_TYPE::AFFECTSBIP_T, 33 }
};

static unordered_map<DESIGN_ENTITY, int> singleSynonymPatternClausePriorityValueMap = {
	{ DESIGN_ENTITY::ASSIGN, 8 },
	{ DESIGN_ENTITY::WHILE, 10 },
	{ DESIGN_ENTITY::IF, 9 }
};

static unordered_map< CLAUSE_TYPE, int> singleSynonymWithClausePriorityValueMap = {
	{ CLAUSE_TYPE::WITH, 2 }
};

static unordered_map<SUCH_THAT_REL_TYPE, int> doubleSynonymSuchThatClausePriorityValueMap = {
	{ SUCH_THAT_REL_TYPE::FOLLOWS, 16 },
	{ SUCH_THAT_REL_TYPE::FOLLOWS_T, 25 },
	{ SUCH_THAT_REL_TYPE::PARENT, 17 },
	{ SUCH_THAT_REL_TYPE::PARENT_T, 26 },
	{ SUCH_THAT_REL_TYPE::MODIFIES, 19 },
	{ SUCH_THAT_REL_TYPE::USES, 20 },
	{ SUCH_THAT_REL_TYPE::CALLS, 14 },
	{ SUCH_THAT_REL_TYPE::CALLS_T, 24 },
	{ SUCH_THAT_REL_TYPE::NEXT, 18 },
	{ SUCH_THAT_REL_TYPE::NEXT_T, 34 },
	{ SUCH_THAT_REL_TYPE::AFFECTS, 35 },
	{ SUCH_THAT_REL_TYPE::AFFECTS_T, 36 },
	{ SUCH_THAT_REL_TYPE::NEXTBIP, 37 },
	{ SUCH_THAT_REL_TYPE::NEXTBIP_T, 38 },
	{ SUCH_THAT_REL_TYPE::AFFECTSBIP, 39 },
	{ SUCH_THAT_REL_TYPE::AFFECTSBIP_T, 40 }
};

static unordered_map<DESIGN_ENTITY, int> doubleSynonymPatternClausePriorityValueMap = {
	{ DESIGN_ENTITY::ASSIGN, 21 },
	{ DESIGN_ENTITY::WHILE, 23 },
	{ DESIGN_ENTITY::IF, 22 }
};

static unordered_map< CLAUSE_TYPE, int> doubleSynonymWithClausePriorityValueMap = {
	{ CLAUSE_TYPE::WITH, 15 }
};


static unordered_map<SUCH_THAT_REL_TYPE, int> noSynonymSuchThatClausePriorityValueMap = {
	{ SUCH_THAT_REL_TYPE::FOLLOWS, 3 },
	{ SUCH_THAT_REL_TYPE::FOLLOWS_T, 12 },
	{ SUCH_THAT_REL_TYPE::PARENT, 4 },
	{ SUCH_THAT_REL_TYPE::PARENT_T, 13 },
	{ SUCH_THAT_REL_TYPE::MODIFIES, 6 },
	{ SUCH_THAT_REL_TYPE::USES, 7 },
	{ SUCH_THAT_REL_TYPE::CALLS, 1 },
	{ SUCH_THAT_REL_TYPE::CALLS_T, 11 },
	{ SUCH_THAT_REL_TYPE::NEXT, 5 },
	{ SUCH_THAT_REL_TYPE::NEXT_T, 14 },
	{ SUCH_THAT_REL_TYPE::AFFECTS, 15 },
	{ SUCH_THAT_REL_TYPE::AFFECTS_T, 16 },
	{ SUCH_THAT_REL_TYPE::NEXTBIP, 17 },
	{ SUCH_THAT_REL_TYPE::NEXTBIP_T, 18 },
	{ SUCH_THAT_REL_TYPE::AFFECTSBIP, 19 },
	{ SUCH_THAT_REL_TYPE::AFFECTSBIP_T, 20 }
};

static unordered_map<DESIGN_ENTITY, int> noSynonymPatternClausePriorityValueMap = {
	{ DESIGN_ENTITY::ASSIGN, 8 },
	{ DESIGN_ENTITY::WHILE, 10 },
	{ DESIGN_ENTITY::IF, 9 }
};

static unordered_map< CLAUSE_TYPE, int> noSynonymWithClausePriorityValueMap = {
	{ CLAUSE_TYPE::WITH, 2 }
};

namespace OptimizerUtility {
	int getPriorityOfhasSynonymSuchThatClause(SuchThatClause suchThatClause, int numOfSynonyms);
	int getPriorityOfhasSynonymPatternClause(PatternClause patternClause, int numOfSynonyms);
	int getPriorityOfhasSynonymWithClause(WithClause withClause, int numOfSynonyms);

	int getPriorityOfNoSynonymSuchThatClause(SuchThatClause suchThatClause);
	int getPriorityOfNoSynonymPatternClause(PatternClause patternClause);
	int getPriorityOfNoSynonymWithClause(WithClause withClause);
};