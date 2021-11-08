#include "PatternClause.h"

PatternClause::PatternClause() : patternSynType(DESIGN_ENTITY::NONE), synIndex(NO_SYNONYM_INDEX) {}

PatternClause::PatternClause(DESIGN_ENTITY patternSynType, SYNONYM_INDEX synIndex, QueryArgument arg, QueryPattern pattern)
	: patternSynType(patternSynType), synIndex(synIndex), arg(arg), pattern(pattern) {}
