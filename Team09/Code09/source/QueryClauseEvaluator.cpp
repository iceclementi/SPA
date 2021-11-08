#include "QueryClauseEvaluator.h"

#pragma region Such That Clause Evaluator

S<DESIGN_ENTITY> SuchThatClauseEvaluator::statementDEs = {
    DESIGN_ENTITY::STATEMENT, DESIGN_ENTITY::READ, DESIGN_ENTITY::PRINT, DESIGN_ENTITY::ASSIGN,
    DESIGN_ENTITY::CALL, DESIGN_ENTITY::WHILE, DESIGN_ENTITY::IF, DESIGN_ENTITY::PROG_LINE
};

SuchThatClauseEvaluator::SuchThatClauseEvaluator(SuchThatClause& clause) : clause(clause) {}

bool SuchThatClauseEvaluator::evaluateClause() {
    init();
    if (relType == RELATIONSHIP_TYPE::INVALID) return false;

    QueryArgument& firstArgument = clause.firstArg;
    QueryArgument& secondArgument = clause.secondArg;

    switch (firstArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER: case ARGUMENT_TYPE::IDENT: case ARGUMENT_TYPE::UNDERSCORE:
        return evaluateGivenNonSynonymFirstValue(firstArgument.getArgStr(), secondArgument);
    case ARGUMENT_TYPE::SYNONYM:
        return evaluateGivenSynonymFirstValue(firstArgument.getArgElem().getSynIndex(), firstArgument.getArgElem().getSynType(), secondArgument);
    default:
        return false;
    }
}

void SuchThatClauseEvaluator::init() {
    switch (clause.relType) {
    case SUCH_THAT_REL_TYPE::FOLLOWS:
        relType = RELATIONSHIP_TYPE::FOLLOWS;
        break;
    case SUCH_THAT_REL_TYPE::FOLLOWS_T:
        relType = RELATIONSHIP_TYPE::FOLLOWS_STAR;
        break;
    case SUCH_THAT_REL_TYPE::PARENT:
        relType = RELATIONSHIP_TYPE::PARENT;
        break;
    case SUCH_THAT_REL_TYPE::PARENT_T:
        relType = RELATIONSHIP_TYPE::PARENT_STAR;
        break;
    case SUCH_THAT_REL_TYPE::USES:
        if (isOfDEType(DESIGN_ENTITY::PROCEDURE, clause.firstArg)) {
            relType = RELATIONSHIP_TYPE::USES_PROC;
        }
        else if (isOfDEType(DESIGN_ENTITY::STATEMENT, clause.firstArg)) {
            relType = RELATIONSHIP_TYPE::USES_STMT;
        }
        break;
    case SUCH_THAT_REL_TYPE::MODIFIES:
        if (isOfDEType(DESIGN_ENTITY::PROCEDURE, clause.firstArg)) {
            relType = RELATIONSHIP_TYPE::MODIFIES_PROC;
        }
        else if (isOfDEType(DESIGN_ENTITY::STATEMENT, clause.firstArg)) {
            relType = RELATIONSHIP_TYPE::MODIFIES_STMT;
        }
        break;
    case SUCH_THAT_REL_TYPE::CALLS:
        relType = RELATIONSHIP_TYPE::CALLS;
        break;
    case SUCH_THAT_REL_TYPE::CALLS_T:
        relType = RELATIONSHIP_TYPE::CALLS_STAR;
        break;
    case SUCH_THAT_REL_TYPE::NEXT:
        relType = RELATIONSHIP_TYPE::NEXT;
        break;
    case SUCH_THAT_REL_TYPE::NEXT_T:
        relType = RELATIONSHIP_TYPE::NEXT_STAR;
        canHaveRelationshipToSelf = true;
        break;
    case SUCH_THAT_REL_TYPE::AFFECTS:
        relType = RELATIONSHIP_TYPE::AFFECTS;
        canHaveRelationshipToSelf = true;
        break;
    case SUCH_THAT_REL_TYPE::AFFECTS_T:
        relType = RELATIONSHIP_TYPE::AFFECTS_STAR;
        canHaveRelationshipToSelf = true;
        break;
    default:
        return;
    }
}

bool SuchThatClauseEvaluator::evaluateGivenNonSynonymFirstValue(VAR firstValue, QueryArgument& secondArgument) {
    VAR secondValue = secondArgument.getArgStr();
    SYNONYM_INDEX secondSynonym = secondArgument.getArgElem().getSynIndex();
    DESIGN_ENTITY secondDEType = secondArgument.getArgElem().getSynType();
    switch (secondArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER: case ARGUMENT_TYPE::IDENT: case ARGUMENT_TYPE::UNDERSCORE:
        return pkb.hasRelationship(relType, firstValue, secondValue);
    case ARGUMENT_TYPE::SYNONYM:
        return synonymStore.mergeWith(secondSynonym,
            pkb.getSecondValuesOf(relType, secondDEType, firstValue, synonymStore.getSynonymValuesOf(secondSynonym)));
    default:
        return false;
    }
}

bool SuchThatClauseEvaluator::evaluateGivenSynonymFirstValue(SYNONYM_INDEX firstSynonym, DESIGN_ENTITY firstDEType, QueryArgument& secondArgument) {
    VAR secondValue = secondArgument.getArgStr();
    SYNONYM_INDEX secondSynonym = secondArgument.getArgElem().getSynIndex();
    DESIGN_ENTITY secondDEType = secondArgument.getArgElem().getSynType();
    switch (secondArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER: case ARGUMENT_TYPE::IDENT: case ARGUMENT_TYPE::UNDERSCORE:
        return synonymStore.mergeWith(firstSynonym, pkb.getFirstValuesOf(relType, firstDEType, secondValue, synonymStore.getSynonymValuesOf(firstSynonym)));
    case ARGUMENT_TYPE::SYNONYM:
        // Relationship to self
        if (firstSynonym == secondSynonym) {
            return canHaveRelationshipToSelf ? synonymStore.mergeWith(firstSynonym, 
                pkb.getRelationshipToSelfOf(relType, firstDEType, synonymStore.getSynonymValuesOf(firstSynonym))) : false;
        }
        if (synonymStore.containsSynonym(firstSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondSynonym) ?
                synonymStore.mergeWith(firstSynonym, secondSynonym, pkb.getValuePairsOf(relType, firstDEType, secondDEType,
                    synonymStore.getSynonymValuesOf(firstSynonym, secondSynonym))) :
                synonymStore.mergeWith(firstSynonym, secondSynonym,
                    pkb.getValuePairsOf(relType, firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstSynonym), true));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondSynonym) ?
                synonymStore.mergeWith(firstSynonym, secondSynonym,
                    pkb.getValuePairsOf(relType, firstDEType, secondDEType, synonymStore.getSynonymValuesOf(secondSynonym), false)) :
                synonymStore.mergeWith(firstSynonym, secondSynonym, pkb.getValuePairsOf(relType, firstDEType, secondDEType));
        }
    default:
        return false;
    }
}

bool SuchThatClauseEvaluator::isOfDEType(DESIGN_ENTITY deType, QueryArgument& argument) {
    ARGUMENT_TYPE argumentType = argument.getArgType();
    DESIGN_ENTITY synonymType = argument.getArgElem().getSynType();
    switch (deType) {
    case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE:
        return argumentType == ARGUMENT_TYPE::IDENT || (argumentType == ARGUMENT_TYPE::SYNONYM && synonymType == deType);
    case DESIGN_ENTITY::STATEMENT: 
        return argumentType == ARGUMENT_TYPE::INTEGER || (argumentType == ARGUMENT_TYPE::SYNONYM && statementDEs.count(synonymType));
    case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::ASSIGN:
    case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF: case DESIGN_ENTITY::PROG_LINE: case DESIGN_ENTITY::CONSTANT:
        return argumentType == ARGUMENT_TYPE::INTEGER || (argumentType == ARGUMENT_TYPE::SYNONYM && synonymType == deType);
    default:
        return false;
    }
}

#pragma endregion

#pragma region Pattern Clause Evaluator

PatternClauseEvaluator::PatternClauseEvaluator(PatternClause& clause) : clause(clause) {}

bool PatternClauseEvaluator::evaluateClause() {
    if (clause.patternSynType == DESIGN_ENTITY::ASSIGN) return evaluateAssignPattern();

    init();
    return relType != RELATIONSHIP_TYPE::INVALID ? evaluateConditionalPattern() : false;
}

void PatternClauseEvaluator::init() {
    switch (clause.patternSynType) {
    case DESIGN_ENTITY::WHILE:
        relType = RELATIONSHIP_TYPE::WHILE_PATTERN;
        break;
    case DESIGN_ENTITY::IF:
        relType = RELATIONSHIP_TYPE::IF_PATTERN;
        break;
    default:
        return;
    }
}

bool PatternClauseEvaluator::evaluateAssignPattern() {
    QueryArgument& variableArg = clause.arg;
    QueryPattern& pattern = clause.pattern;
    SYNONYM_INDEX assignSynonym = clause.synIndex;

    switch (variableArg.getArgType()) {
        // pattern a ("x", "a + b"), pattern a (_, "a + b"), pattern a ("x", _"a + b"_), pattern a (_, _"a + b"_), pattern a ("x", _), pattern a (_, _)
    case ARGUMENT_TYPE::IDENT: case ARGUMENT_TYPE::UNDERSCORE:
        return evaluateAssignPatternGivenNonSynonym(assignSynonym, variableArg.getArgStr(), pattern);
        // pattern a (v, "a + b"), pattern a (v, _"a + b"_), pattern a (v, _)
    case ARGUMENT_TYPE::SYNONYM:
        return evaluateAssignPatternGivenSynonym(assignSynonym, variableArg.getArgElem().getSynIndex(), pattern);
    default:
        return false;
    }
}

bool PatternClauseEvaluator::evaluateAssignPatternGivenNonSynonym(SYNONYM_INDEX assignSynonym, VAR_NAME varName, QueryPattern patternArgument) {
    PATTERN pattern = patternArgument.patternStr;
    PATTERN_MATCH_TYPE matchType = patternArgument.patternMatchType;
    switch (matchType) {
    case PATTERN_MATCH_TYPE::EXACT_MATCH: case PATTERN_MATCH_TYPE::PARTIAL_MATCH: // pattern a ("x", "a + b"), pattern a (_, "a + b"), pattern a ("x", _"a + b"_), pattern a (_, _"a + b"_)
        return synonymStore.mergeWith(assignSynonym,
            pkb.getAssignOfPattern(varName, pattern, matchType == PATTERN_MATCH_TYPE::EXACT_MATCH, synonymStore.getSynonymValuesOf(assignSynonym)));
    case PATTERN_MATCH_TYPE::UNRESTRICTED: // pattern a ("x", _), pattern a (_, _)
        return synonymStore.mergeWith(assignSynonym, pkb.getAssignWithoutPattern(varName, synonymStore.getSynonymValuesOf(assignSynonym)));
    default:
        return false;
    }
}

bool PatternClauseEvaluator::evaluateAssignPatternGivenSynonym(SYNONYM_INDEX assignSynonym, SYNONYM_INDEX varSynonym, QueryPattern patternArgument) {
    PATTERN pattern = patternArgument.patternStr;
    PATTERN_MATCH_TYPE matchType = patternArgument.patternMatchType;
    switch (matchType) {
    case PATTERN_MATCH_TYPE::EXACT_MATCH: case PATTERN_MATCH_TYPE::PARTIAL_MATCH: // pattern a (v, "a + b"), pattern a (v, _"a + b"_)
        if (synonymStore.containsSynonym(assignSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(varSynonym) ?
                synonymStore.mergeWith(assignSynonym, varSynonym,
                    pkb.getAssignVarOfPattern(pattern, matchType == PATTERN_MATCH_TYPE::EXACT_MATCH, synonymStore.getSynonymValuesOf(assignSynonym, varSynonym))) :
                synonymStore.mergeWith(assignSynonym, varSynonym,
                    pkb.getAssignVarOfPattern(pattern, matchType == PATTERN_MATCH_TYPE::EXACT_MATCH, synonymStore.getSynonymValuesOf(assignSynonym), true));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(varSynonym) ?
                synonymStore.mergeWith(assignSynonym, varSynonym,
                    pkb.getAssignVarOfPattern(pattern, matchType == PATTERN_MATCH_TYPE::EXACT_MATCH, synonymStore.getSynonymValuesOf(varSynonym), false)) :
                synonymStore.mergeWith(assignSynonym, varSynonym, pkb.getAssignVarOfPattern(pattern, matchType == PATTERN_MATCH_TYPE::EXACT_MATCH));
        }
    case PATTERN_MATCH_TYPE::UNRESTRICTED: // pattern a (v, _)
        if (synonymStore.containsSynonym(assignSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(varSynonym) ?
                synonymStore.mergeWith(assignSynonym, varSynonym, pkb.getAssignVarWithoutPattern(synonymStore.getSynonymValuesOf(assignSynonym, varSynonym))) :
                synonymStore.mergeWith(assignSynonym, varSynonym, pkb.getAssignVarWithoutPattern(synonymStore.getSynonymValuesOf(assignSynonym), true));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(varSynonym) ?
                synonymStore.mergeWith(assignSynonym, varSynonym, pkb.getAssignVarWithoutPattern(synonymStore.getSynonymValuesOf(varSynonym), false)) :
                synonymStore.mergeWith(assignSynonym, varSynonym, pkb.getAssignVarWithoutPattern());
        }
    default:
        return false;
    }
}

bool PatternClauseEvaluator::evaluateConditionalPattern() {
    QueryArgument variableArg = clause.arg;
    SYNONYM_INDEX condSynonym = clause.synIndex;
    DESIGN_ENTITY condType = clause.patternSynType;

    switch (variableArg.getArgType()) {
    case ARGUMENT_TYPE::IDENT: case ARGUMENT_TYPE::UNDERSCORE:
        return evaluateConditionalPatternGivenNonSynonym(condSynonym, condType, variableArg.getArgStr());
    case ARGUMENT_TYPE::SYNONYM:
        return evaluateConditionalPatternGivenSynonym(condSynonym, condType, variableArg.getArgElem().getSynIndex());
    default:
        return false;
    }
}

bool PatternClauseEvaluator::evaluateConditionalPatternGivenNonSynonym(SYNONYM_INDEX condSynonym, DESIGN_ENTITY condType, VAR_NAME varName) {
    return synonymStore.mergeWith(condSynonym, pkb.getFirstValuesOf(relType, condType, varName, synonymStore.getSynonymValuesOf(condSynonym)));
}

bool PatternClauseEvaluator::evaluateConditionalPatternGivenSynonym(SYNONYM_INDEX condSynonym, DESIGN_ENTITY condType, SYNONYM_INDEX varSynonym) {
    if (synonymStore.containsSynonym(condSynonym)) { // Table contains both synonyms or only first synonym
        return synonymStore.containsSynonym(varSynonym) ?
            synonymStore.mergeWith(condSynonym, varSynonym,
                pkb.getValuePairsOf(relType, condType, DESIGN_ENTITY::VARIABLE, synonymStore.getSynonymValuesOf(condSynonym, varSynonym))) :
            synonymStore.mergeWith(condSynonym, varSynonym,
                pkb.getValuePairsOf(relType, condType, DESIGN_ENTITY::VARIABLE, synonymStore.getSynonymValuesOf(condSynonym), true));
    }
    else { // Table contains only second synonym or no synonym
        return synonymStore.containsSynonym(varSynonym) ?
            synonymStore.mergeWith(condSynonym, varSynonym,
                pkb.getValuePairsOf(relType, condType, DESIGN_ENTITY::VARIABLE, synonymStore.getSynonymValuesOf(varSynonym), false)) :
            synonymStore.mergeWith(condSynonym, varSynonym, pkb.getValuePairsOf(relType, condType, DESIGN_ENTITY::VARIABLE));
    }
}

#pragma endregion

#pragma region With Clause Evaluator

WithClauseEvaluator::WithClauseEvaluator(WithClause& clause) : clause(clause) {}

bool WithClauseEvaluator::evaluateClause() {
    QueryArgument firstArgument = clause.firstArg;
    QueryArgument secondArgument = clause.secondArg;

    switch (firstArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER:
        return evaluateGivenIntegerFirstArg(firstArgument.getArgStr(), secondArgument);
    case ARGUMENT_TYPE::IDENT:
        return evaluateGivenIdentFirstArg(firstArgument.getArgStr(), secondArgument);
    case ARGUMENT_TYPE::SYNONYM:
        return firstArgument.getArgElem().getSynType() == DESIGN_ENTITY::PROG_LINE ?
            evaluateGivenSynonymFirstArg(firstArgument.getArgElem().getSynIndex(), secondArgument) : false;
    case ARGUMENT_TYPE::ATTRIBUTE_REF:
        return evaluateGivenAttrRefFirstArg(firstArgument.getArgElem().getSynIndex(), firstArgument.getArgElem().getSynType(), firstArgument.getArgElem().getAttrType(), secondArgument);
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenIntegerFirstArg(VAR number, QueryArgument& secondArgument) {
    VAR secondValue = secondArgument.getArgStr();
    SYNONYM_INDEX secondSynonym = secondArgument.getArgElem().getSynIndex();
    DESIGN_ENTITY secondDEType = secondArgument.getArgElem().getSynType();
    switch (secondArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER:
        return number == secondValue;
    case ARGUMENT_TYPE::SYNONYM:
        return secondDEType == DESIGN_ENTITY::PROG_LINE ?
            synonymStore.mergeWith(secondSynonym, pkb.getValuesOf(DESIGN_ENTITY::PROG_LINE, number, synonymStore.getSynonymValuesOf(secondSynonym))) : false;
    case ARGUMENT_TYPE::ATTRIBUTE_REF:
        return secondArgument.getArgElem().getAttrType() == ATTRIBUTE_TYPE::INTEGER ?
            evaluateGivenIntegerAttrRef(number, secondSynonym, secondDEType) : false;
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenIdentFirstArg(VAR identifier, QueryArgument& secondArgument) {
    VAR secondValue = secondArgument.getArgStr();
    SYNONYM_INDEX secondSynonym = secondArgument.getArgElem().getSynIndex();
    DESIGN_ENTITY secondDEType = secondArgument.getArgElem().getSynType();
    switch (secondArgument.getArgType()) {
    case ARGUMENT_TYPE::IDENT:
        return identifier == secondValue;
    case ARGUMENT_TYPE::ATTRIBUTE_REF:
        return secondArgument.getArgElem().getAttrType() == ATTRIBUTE_TYPE::NAME ?
            evaluateGivenIdentAttrRef(identifier, secondSynonym, secondDEType) : false;
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenSynonymFirstArg(SYNONYM_INDEX progSynonym, QueryArgument& secondArgument) {
    VAR secondValue = secondArgument.getArgStr();
    SYNONYM_INDEX secondSynonym = secondArgument.getArgElem().getSynIndex();
    DESIGN_ENTITY secondDEType = secondArgument.getArgElem().getSynType();
    switch (secondArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER:
        return synonymStore.mergeWith(progSynonym, pkb.getValuesOf(DESIGN_ENTITY::PROG_LINE, secondValue, synonymStore.getSynonymValuesOf(progSynonym)));
    case ARGUMENT_TYPE::SYNONYM:
        if (secondDEType != DESIGN_ENTITY::PROG_LINE) {
            return false;
        }
        if (progSynonym == secondSynonym) {
            return synonymStore.mergeWith(progSynonym, pkb.getValuesOf(DESIGN_ENTITY::PROG_LINE, synonymStore.getSynonymValuesOf(progSynonym)));
        }
        if (synonymStore.containsSynonym(progSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondSynonym) ?
                synonymStore.mergeWith(progSynonym, secondSynonym,
                    pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, DESIGN_ENTITY::PROG_LINE, synonymStore.getSynonymValuesOf(progSynonym, secondSynonym))) :
                synonymStore.mergeWith(progSynonym, secondSynonym,
                    pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, DESIGN_ENTITY::PROG_LINE, synonymStore.getSynonymValuesOf(progSynonym)));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondSynonym) ?
                synonymStore.mergeWith(progSynonym, secondSynonym,
                    pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, DESIGN_ENTITY::PROG_LINE, synonymStore.getSynonymValuesOf(secondSynonym))) :
                synonymStore.mergeWith(progSynonym, secondSynonym, pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, DESIGN_ENTITY::PROG_LINE));
        }
    case ARGUMENT_TYPE::ATTRIBUTE_REF:
        return secondArgument.getArgElem().getAttrType() == ATTRIBUTE_TYPE::INTEGER ?
            evaluateGivenSynonymAttrRef(progSynonym, secondSynonym, secondDEType) : false;
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenAttrRefFirstArg(SYNONYM_INDEX refSynonym, DESIGN_ENTITY deType, ATTRIBUTE_TYPE attrType, QueryArgument& secondArgument) {
    VAR secondValue = secondArgument.getArgStr();
    SYNONYM_INDEX secondSynonym = secondArgument.getArgElem().getSynIndex();
    DESIGN_ENTITY secondDEType = secondArgument.getArgElem().getSynType();
    switch (secondArgument.getArgType()) {
    case ARGUMENT_TYPE::INTEGER:
        return attrType == ATTRIBUTE_TYPE::INTEGER ? evaluateGivenIntegerAttrRef(secondValue, refSynonym, deType) : false;
    case ARGUMENT_TYPE::IDENT:
        return attrType == ATTRIBUTE_TYPE::NAME ? evaluateGivenIdentAttrRef(secondValue, refSynonym, deType) : false;
    case ARGUMENT_TYPE::SYNONYM:
        return attrType == ATTRIBUTE_TYPE::INTEGER && secondDEType == DESIGN_ENTITY::PROG_LINE ?
            evaluateGivenSynonymAttrRef(secondSynonym, refSynonym, deType) : false;
    case ARGUMENT_TYPE::ATTRIBUTE_REF:
        return evaluateGivenAttrRefAttrRef(refSynonym, deType, attrType, secondSynonym, secondDEType, secondArgument.getArgElem().getAttrType());
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenIntegerAttrRef(VAR number, SYNONYM_INDEX refSynonym, DESIGN_ENTITY deType) {
    switch (deType) {
    case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::CALL:
    case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF: case DESIGN_ENTITY::ASSIGN: case DESIGN_ENTITY::CONSTANT:
        return synonymStore.mergeWith(refSynonym, pkb.getValuesOf(deType, number, synonymStore.getSynonymValuesOf(refSynonym)));
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenIdentAttrRef(VAR identifier, SYNONYM_INDEX refSynonym, DESIGN_ENTITY deType) {
    switch (deType) {
    case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE:
        return synonymStore.mergeWith(refSynonym, pkb.getValuesOf(deType, identifier, synonymStore.getSynonymValuesOf(refSynonym)));
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
        return synonymStore.mergeWith(refSynonym, pkb.getStmtsWithVar(deType, identifier, synonymStore.getSynonymValuesOf(refSynonym)));
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenSynonymAttrRef(SYNONYM_INDEX progSynonym, SYNONYM_INDEX refSynonym, DESIGN_ENTITY deType) {
    switch (deType) {
    case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::CALL:
    case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF: case DESIGN_ENTITY::ASSIGN: case DESIGN_ENTITY::CONSTANT:
        if (synonymStore.containsSynonym(progSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(refSynonym) ?
                synonymStore.mergeWith(progSynonym, refSynonym, pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, deType, synonymStore.getSynonymValuesOf(progSynonym, refSynonym))) :
                synonymStore.mergeWith(progSynonym, refSynonym, pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, deType, synonymStore.getSynonymValuesOf(progSynonym)));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(refSynonym) ?
                synonymStore.mergeWith(progSynonym, refSynonym, pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, deType, synonymStore.getSynonymValuesOf(refSynonym))) :
                synonymStore.mergeWith(progSynonym, refSynonym, pkb.getValuePairsOf(DESIGN_ENTITY::PROG_LINE, deType));
        }
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenAttrRefAttrRef(SYNONYM_INDEX firstRefSynonym, DESIGN_ENTITY firstDEType, ATTRIBUTE_TYPE firstAttrType,
    SYNONYM_INDEX secondRefSynonym, DESIGN_ENTITY secondDEType, ATTRIBUTE_TYPE secondAttrType) {

    if (firstRefSynonym == secondRefSynonym) { // Same synonym 
        return (firstDEType == secondDEType && firstAttrType == secondAttrType) ?
            evaluateGivenSameAttrRef(firstRefSynonym, firstDEType, firstAttrType) : false;
    }
    if (firstAttrType == ATTRIBUTE_TYPE::INTEGER && secondAttrType == ATTRIBUTE_TYPE::INTEGER) {
        return evaluateGivenIntegerAttrRefs(firstRefSynonym, firstDEType, firstAttrType, secondRefSynonym, secondDEType, secondAttrType);
    }
    if (firstAttrType == ATTRIBUTE_TYPE::NAME && secondAttrType == ATTRIBUTE_TYPE::NAME) {
        return evaluateGivenNameAttrRefs(firstRefSynonym, firstDEType, firstAttrType, secondRefSynonym, secondDEType, secondAttrType);
    }
    return false;
}

bool WithClauseEvaluator::evaluateGivenSameAttrRef(SYNONYM_INDEX refSynonym, DESIGN_ENTITY deType, ATTRIBUTE_TYPE attrType) {
    switch (deType) {
    case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF: case DESIGN_ENTITY::ASSIGN:
        return attrType == ATTRIBUTE_TYPE::INTEGER ?
            synonymStore.mergeWith(refSynonym, pkb.getValuesOf(deType, synonymStore.getSynonymValuesOf(refSynonym))) : false;
    case DESIGN_ENTITY::CONSTANT:
        return attrType == ATTRIBUTE_TYPE::INTEGER ?
            synonymStore.mergeWith(refSynonym, pkb.getValuesOf(deType, synonymStore.getSynonymValuesOf(refSynonym))) : false;
    case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE:
        return attrType == ATTRIBUTE_TYPE::NAME ?
            synonymStore.mergeWith(refSynonym, pkb.getValuesOf(deType, synonymStore.getSynonymValuesOf(refSynonym))) : false;
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
        return (attrType == ATTRIBUTE_TYPE::INTEGER || attrType == ATTRIBUTE_TYPE::NAME) ?
            synonymStore.mergeWith(refSynonym, pkb.getValuesOf(deType, synonymStore.getSynonymValuesOf(refSynonym))) : false;
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenIntegerAttrRefs(SYNONYM_INDEX firstRefSynonym, DESIGN_ENTITY firstDEType, ATTRIBUTE_TYPE firstAttrType,
    SYNONYM_INDEX secondRefSynonym, DESIGN_ENTITY secondDEType, ATTRIBUTE_TYPE secondAttrType) {
    switch (firstDEType) {
    case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF: case DESIGN_ENTITY::ASSIGN:
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::CONSTANT:
        return evaluateGivenIntegerAttrRefStmt(firstRefSynonym, firstDEType, firstAttrType, secondRefSynonym, secondDEType, secondAttrType);
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenIntegerAttrRefStmt(SYNONYM_INDEX firstRefSynonym, DESIGN_ENTITY firstDEType, ATTRIBUTE_TYPE firstAttrType,
    SYNONYM_INDEX secondRefSynonym, DESIGN_ENTITY secondDEType, ATTRIBUTE_TYPE secondAttrType) {
    switch (secondDEType) {
    case DESIGN_ENTITY::STATEMENT: case DESIGN_ENTITY::WHILE: case DESIGN_ENTITY::IF: case DESIGN_ENTITY::ASSIGN:
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT: case DESIGN_ENTITY::CONSTANT:
        if (synonymStore.containsSynonym(firstRefSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getValuePairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym, secondRefSynonym))) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getValuePairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym)));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getValuePairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(secondRefSynonym))) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym, pkb.getValuePairsOf(firstDEType, secondDEType));
        }
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenNameAttrRefs(SYNONYM_INDEX firstRefSynonym, DESIGN_ENTITY firstDEType, ATTRIBUTE_TYPE firstAttrType,
    SYNONYM_INDEX secondRefSynonym, DESIGN_ENTITY secondDEType, ATTRIBUTE_TYPE secondAttrType) {
    switch (firstDEType) {
    case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE:
        return evaluateGivenNameAttrRefProcVar(firstRefSynonym, firstDEType, firstAttrType, secondRefSynonym, secondDEType, secondAttrType);
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
        return evaluateGivenNameAttrRefStmt(firstRefSynonym, firstDEType, firstAttrType, secondRefSynonym, secondDEType, secondAttrType);
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenNameAttrRefProcVar(SYNONYM_INDEX firstRefSynonym, DESIGN_ENTITY firstDEType, ATTRIBUTE_TYPE firstAttrType,
    SYNONYM_INDEX secondRefSynonym, DESIGN_ENTITY secondDEType, ATTRIBUTE_TYPE secondAttrType) {
    switch (secondDEType) {
    case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE:
        if (synonymStore.containsSynonym(firstRefSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getValuePairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym, secondRefSynonym))) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getValuePairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym)));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getValuePairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(secondRefSynonym))) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym, pkb.getValuePairsOf(firstDEType, secondDEType));
        }
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
        if (synonymStore.containsSynonym(firstRefSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(secondRefSynonym, firstRefSynonym,
                    pkb.getStmtVarVarPairsOf(secondDEType, firstDEType, synonymStore.getSynonymValuesOf(secondRefSynonym, firstRefSynonym))) :
                synonymStore.mergeWith(secondRefSynonym, firstRefSynonym,
                    pkb.getStmtVarVarPairsOf(secondDEType, firstDEType, synonymStore.getSynonymValuesOf(firstRefSynonym), false));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(secondRefSynonym, firstRefSynonym,
                    pkb.getStmtVarVarPairsOf(secondDEType, firstDEType, synonymStore.getSynonymValuesOf(secondRefSynonym), true)) :
                synonymStore.mergeWith(secondRefSynonym, firstRefSynonym, pkb.getStmtVarVarPairsOf(secondDEType, firstDEType));
        }
    default:
        return false;
    }
}

bool WithClauseEvaluator::evaluateGivenNameAttrRefStmt(SYNONYM_INDEX firstRefSynonym, DESIGN_ENTITY firstDEType, ATTRIBUTE_TYPE firstAttrType,
    SYNONYM_INDEX secondRefSynonym, DESIGN_ENTITY secondDEType, ATTRIBUTE_TYPE secondAttrType) {
    switch (secondDEType) {
    case DESIGN_ENTITY::PROCEDURE: case DESIGN_ENTITY::VARIABLE:
        if (synonymStore.containsSynonym(firstRefSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getStmtVarVarPairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym, secondRefSynonym))) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getStmtVarVarPairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym), true));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getStmtVarVarPairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(secondRefSynonym), false)) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym, pkb.getStmtVarVarPairsOf(firstDEType, secondDEType));
        }
    case DESIGN_ENTITY::CALL: case DESIGN_ENTITY::READ: case DESIGN_ENTITY::PRINT:
        if (synonymStore.containsSynonym(firstRefSynonym)) { // Table contains both synonyms or only first synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getStmtVarPairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym, secondRefSynonym))) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getStmtVarPairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(firstRefSynonym), true));
        }
        else { // Table contains only second synonym or no synonym
            return synonymStore.containsSynonym(secondRefSynonym) ?
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym,
                    pkb.getStmtVarPairsOf(firstDEType, secondDEType, synonymStore.getSynonymValuesOf(secondRefSynonym), false)) :
                synonymStore.mergeWith(firstRefSynonym, secondRefSynonym, pkb.getStmtVarPairsOf(firstDEType, secondDEType));
        }
    default:
        return false;
    }
}

#pragma endregion

#pragma region Select Clause Evaluator

SelectClauseEvaluator::SelectClauseEvaluator(QuerySelectResult& selectResult) {
    selectElements = selectResult.getSelectElementList();
}

bool SelectClauseEvaluator::evaluateClause() {
    mergeRemainingSelectSynonymValues();
    return true;
}

ANSWER SelectClauseEvaluator::getAnswer() {
    if (selectElements.empty()) return ANSWER({ TRUE });

    initStringIntermediateAnswer();

    for (size_t i = 0; i < selectElements.size(); ++i) {
        ATTRIBUTE_TYPE attrType = selectElements[i].getAttrType();
        DESIGN_ENTITY deType = selectElements[i].getSynType();
        if (attrType == ATTRIBUTE_TYPE::NAME && 
            (deType == DESIGN_ENTITY::READ || deType == DESIGN_ENTITY::PRINT || deType == DESIGN_ENTITY::CALL)) {
            convertToStringStmtVar(selectElements[i].getSynType(), i);
        }
    }

    return formatAnswer();
}

void SelectClauseEvaluator::mergeRemainingSelectSynonymValues() {
    SYNONYM_INDICES missingSynoyms = synonymStore.getMissingSelectSynonyms();
    for (auto& selectElement : selectElements) {
        SYNONYM_INDEX selectSynonym = selectElement.getSynIndex();
        if (missingSynoyms.count(selectSynonym)) {
            synonymStore.mergeWith(selectSynonym, pkb.getIndexedDEValuesOf(selectElement.getSynType()));
            missingSynoyms.erase(selectSynonym);
        }
    }
}

void SelectClauseEvaluator::initStringIntermediateAnswer() {
    INDEXED_INTERMEDIATE_ANSWER& indexedIntermediateAnswer = synonymStore.getIntermediateAnswer();
    // Intialise to same number of rows
    intermediateAnswer = STRING_INTERMEDIATE_ANSWER(indexedIntermediateAnswer.size());
    // Convert index to string value in column order
    for (size_t columnIndex = 0; columnIndex < selectElements.size(); ++columnIndex) {
        DESIGN_ENTITY deType = selectElements[columnIndex].getSynType();
        for (size_t rowIndex = 0; rowIndex < indexedIntermediateAnswer.size(); ++rowIndex) {
            intermediateAnswer[rowIndex].push_back(pkb.getStringValueOfDE(indexedIntermediateAnswer[rowIndex][columnIndex]));
        }
    }
}

void SelectClauseEvaluator::convertToStringStmtVar(DESIGN_ENTITY deType, INDEX index) {
    for (auto& valueList : intermediateAnswer) {
        VAR var = pkb.getStringStmtVarOf(deType, valueList[index]);
        valueList[index] = var;
    }
}

ANSWER SelectClauseEvaluator::formatAnswer() {
    RESULTS results;
    for (auto& valueList : intermediateAnswer) {
        results.insert(convertToResultString(valueList));
    }
    return ANSWER(results.begin(), results.end());
}

RESULT SelectClauseEvaluator::convertToResultString(VAR_LIST& valueList) {
    RESULT result;
    for (auto& value : valueList) {
        result += value + " ";
    }
    result.pop_back(); // Remove extra space
    return result;
}

#pragma endregion
