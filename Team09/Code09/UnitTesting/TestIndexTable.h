#pragma once

#include <PKBStorage.h>

	M<INDEX, VAR> indexToValueStub = 
	{
		{1, "a"},
		{2, "b"},
		{3, "c"},
		{4, "d"},
		{5, "e"}
	};

	M<VAR, INDEX> valueToIndexStub = 
	{
		{"a", 1},
		{"b", 2},
		{"c", 3},
		{"d", 4},
		{"e", 5}
	};

	M<VAR, INDEX> emptyIndexStub = {};
