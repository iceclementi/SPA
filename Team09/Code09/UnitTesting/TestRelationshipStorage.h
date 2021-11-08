#pragma once

#include "Common.h"
#include "PKBStorage.h"

RELATIONSHIP_TABLE<VALUE, VALUE> forwardTableStub = {
	{ 1, { 2, 3 } },
	{ 2, { 3, 5} },
	{ 4, { 5 } },
	{ 3, { 4 } }
};

RELATIONSHIP_TABLE<VALUE, VALUE> backwardTableStub = {
	{ 2, { 1 } },
	{ 3, { 1, 2} },
	{ 4, { 3 } },
	{ 5, { 2, 4 } }
};

S<VALUE> firstValuesStub = { 1, 2, 3, 4 };

S<VALUE> secondValuesStub = {2, 3, 4, 5};

PAIRS<VALUE, VALUE> indexPairsStub = { {1, 2}, {1, 3}, {2, 3}, {2, 5}, {4, 5}, {3, 4} };

RELATIONSHIP_TABLE<VALUE, VALUE> emptyRelationshipTableStub = {};
