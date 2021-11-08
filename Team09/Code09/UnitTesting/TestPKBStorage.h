#pragma once

#include "Common.h"
#include "PKBStorage.h"

RELATIONSHIP_TABLE<VALUE, VALUE> forwardTableStub = {
	{ "one",	{ "two", "three" } },
	{ "two",	{ "three", "five"} },
	{ "three",	{ "four" } },
	{ "four",	{ "five" } },
};
RELATIONSHIP_TABLE<VALUE, VALUE> backwardTableStub = {
	{ "two",	{ "one" } },
	{ "three",	{ "one", "two"} },
	{ "four",	{ "three" } },
	{ "five",	{ "two", "four" } }
};
RELATIONSHIP_TABLE<VALUE, VALUE> tableStubEmpty = {};
