#pragma once

#include <PKBStorage.h>

RELATIONSHIP_TABLE<INDEX, INDEX> assignVarTableStub =
{
	{1, {5}},
	{2, {6}},
	{3, {7}},
	{4, {8}}
};

RELATIONSHIP_TABLE<INDEX, PATTERN> assignPatTableStub =
{
	{1, {"a + b"}},
	{2, {"c * d"}},
	{3, {"e / f"}},
	{4, {"g - h"}}
};

INDICES indexedStmtsWithVarStub = {3};

INDICES indexedStmtsWithPatternStub = {4};

INDICES emptyIndices = {};