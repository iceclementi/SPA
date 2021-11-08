#include "OptimiserUtility.h"

int OptimizerUtility::getPriorityOfhasSynonymSuchThatClause(SuchThatClause suchThatClause, int numOfSynonyms) {
    int priorityValue = BASE_PRIORITY_VALUE;
    switch (suchThatClause.relType) {
    case SUCH_THAT_REL_TYPE::FOLLOWS:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::FOLLOWS] :
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::FOLLOWS];
        break;
    case SUCH_THAT_REL_TYPE::FOLLOWS_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::FOLLOWS_T] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::FOLLOWS_T];
        break;
    case SUCH_THAT_REL_TYPE::PARENT:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::PARENT] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::PARENT];
        break;
    case SUCH_THAT_REL_TYPE::PARENT_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::PARENT_T] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::PARENT_T];
        break;
    case SUCH_THAT_REL_TYPE::MODIFIES:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::MODIFIES] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::MODIFIES];
        break;
    case SUCH_THAT_REL_TYPE::USES:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::USES] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::USES];
        break;
    case SUCH_THAT_REL_TYPE::CALLS:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::CALLS] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::CALLS];
        break;
    case SUCH_THAT_REL_TYPE::CALLS_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::CALLS_T] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::CALLS_T];
        break;
    case SUCH_THAT_REL_TYPE::NEXT:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXT] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXT];
        break;
    case SUCH_THAT_REL_TYPE::NEXT_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXT_T] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXT_T];
        break;
    case SUCH_THAT_REL_TYPE::AFFECTS:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTS] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTS];
        break;
    case SUCH_THAT_REL_TYPE::AFFECTS_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTS_T] : 
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTS_T];
        break;
    case SUCH_THAT_REL_TYPE::NEXTBIP:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXTBIP] :
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXTBIP];
        break;
    case SUCH_THAT_REL_TYPE::NEXTBIP_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXTBIP_T] :
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXTBIP_T];
        break;
    case SUCH_THAT_REL_TYPE::AFFECTSBIP:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTSBIP] :
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTSBIP];
        break;
    case SUCH_THAT_REL_TYPE::AFFECTSBIP_T:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTSBIP_T] :
            priorityValue = doubleSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTSBIP_T];
        break;
    default:
        return priorityValue;
    }
	return priorityValue;
}

int OptimizerUtility::getPriorityOfhasSynonymPatternClause(PatternClause patternClause, int numOfSynonyms) {
    int priorityValue = BASE_PRIORITY_VALUE;
    switch (patternClause.patternSynType) {
    case DESIGN_ENTITY::ASSIGN:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::ASSIGN] : 
            priorityValue = doubleSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::ASSIGN];
        break;
    case DESIGN_ENTITY::WHILE:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::WHILE] : 
            priorityValue = doubleSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::WHILE];
        break;
    case DESIGN_ENTITY::IF:
        (numOfSynonyms == 1) ? priorityValue = singleSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::IF] : 
            priorityValue = doubleSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::IF];
        break;
    default:
        return priorityValue;
    }
    return priorityValue;
}

int OptimizerUtility::getPriorityOfhasSynonymWithClause(WithClause withClause, int numOfSynonyms) {
    int priorityValue = BASE_PRIORITY_VALUE;
    if (numOfSynonyms == 1) {
        priorityValue = singleSynonymWithClausePriorityValueMap[CLAUSE_TYPE::WITH];
    } else {
        priorityValue = doubleSynonymWithClausePriorityValueMap[CLAUSE_TYPE::WITH];
    }
    return  priorityValue;
}

int OptimizerUtility::getPriorityOfNoSynonymSuchThatClause(SuchThatClause suchThatClause) {
    int priorityValue = BASE_PRIORITY_VALUE;
    switch (suchThatClause.relType) {
    case SUCH_THAT_REL_TYPE::FOLLOWS:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::FOLLOWS];
    case SUCH_THAT_REL_TYPE::FOLLOWS_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::FOLLOWS_T];
    case SUCH_THAT_REL_TYPE::PARENT:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::PARENT];
    case SUCH_THAT_REL_TYPE::PARENT_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::PARENT_T];
    case SUCH_THAT_REL_TYPE::MODIFIES:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::MODIFIES];
    case SUCH_THAT_REL_TYPE::USES:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::USES];
    case SUCH_THAT_REL_TYPE::CALLS:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::CALLS];
    case SUCH_THAT_REL_TYPE::CALLS_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::CALLS_T];
    case SUCH_THAT_REL_TYPE::NEXT:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXT];
    case SUCH_THAT_REL_TYPE::NEXT_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXT_T];
    case SUCH_THAT_REL_TYPE::AFFECTS:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTS];
    case SUCH_THAT_REL_TYPE::AFFECTS_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTS_T];
    case SUCH_THAT_REL_TYPE::NEXTBIP:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXTBIP];
    case SUCH_THAT_REL_TYPE::NEXTBIP_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::NEXTBIP_T];
    case SUCH_THAT_REL_TYPE::AFFECTSBIP:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTSBIP];
    case SUCH_THAT_REL_TYPE::AFFECTSBIP_T:
        return noSynonymSuchThatClausePriorityValueMap[SUCH_THAT_REL_TYPE::AFFECTSBIP_T];
    default:
        return priorityValue;
    }
    return priorityValue;
}

int OptimizerUtility::getPriorityOfNoSynonymPatternClause(PatternClause patternClause) {
    int priorityValue = BASE_PRIORITY_VALUE;

    switch (patternClause.patternSynType) {
    case DESIGN_ENTITY::ASSIGN:
        return noSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::ASSIGN];
    case DESIGN_ENTITY::WHILE:
        return noSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::WHILE];
    case DESIGN_ENTITY::IF:
        return noSynonymPatternClausePriorityValueMap[DESIGN_ENTITY::IF];
    default:
        return priorityValue;
    }
    return priorityValue;
}

int OptimizerUtility::getPriorityOfNoSynonymWithClause(WithClause withClause) {
    return  noSynonymWithClausePriorityValueMap[CLAUSE_TYPE::WITH];
}
