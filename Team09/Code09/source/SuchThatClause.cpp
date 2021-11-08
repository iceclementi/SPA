#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() : relType(SUCH_THAT_REL_TYPE::INVALID) {}

SuchThatClause::SuchThatClause(SUCH_THAT_REL_TYPE relType, QueryArgument firstArg, QueryArgument secondArg) 
	: relType(relType), firstArg(firstArg), secondArg(secondArg) {}
