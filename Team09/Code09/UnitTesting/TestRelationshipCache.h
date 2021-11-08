#pragma once

#include "RelationshipCache.h"

RELATIONSHIP_TABLE<INDEX, INDEX> nStarTableStub = {
	{ 1, { 2, 3, 4, 5 } },
	{ 2, { 3, 4, 5 } },
	{ 3, { 4, 5 } },
	{ 4, { 5 } }
};
RELATIONSHIP_TABLE<INDEX, INDEX> nStarTableStub1 = {
	{ 2, { 3 } }
};
RELATIONSHIP_TABLE<INDEX, INDEX> nStarTableStub2 = {
	{ 3, { 4 } }
};
RELATIONSHIP_TABLE<INDEX, INDEX> nStarTableStubEmpty = {};
