#pragma once

#include "Common.h"
#include "MiniTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

VALUES singleTableStub = { 101, 102, 103, 104, 105, 106 };
VALUE_PAIRS doubleTableStub = { 
	{ 1, 1 }, { 1, 2 }, { 2, 1 }, { 3, 1 }, { 3, 3 }, 
	{ 4, 1 }, { 4, 5 }, { 5, 2 }, { 5, 3 }, { 5, 5 } 
};
VALUES emptyValuesStub;
VALUE_PAIRS emptyValuePairsStub;
