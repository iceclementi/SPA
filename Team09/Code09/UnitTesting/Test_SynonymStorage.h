#pragma once

#include "Common.h"
#include "SynonymStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

SYNONYM_INDICES selectSynonyms = { 102 };
SYNONYM_COUNTER synonymCounter = { 
	{ 101, 2 }, { 102, 1 }, { 103, 2 }, { 104, 2 }, { 105, 1 }, { 106, 2 }, 
	{ 107, 1 }, { 108, 2 }, { 109, 2 }, { 110, 1 }, { 111, 1 } 
};

VALUES singleTableStub_a = { 1, 2, 3 };
VALUES singleTableStub_b = { 1, 2, 3 };
VALUES singleTableStub_c = { 1, 2 };
VALUES singleTableStub_d = { 124, 125, 126 };
VALUES singleTableStub_e = { 124, 125 };
VALUES singleTableStub_f = { 101, 102 };
VALUES singleTableStub_g = { 103, 104 };

VALUE_PAIRS doubleTableStub_ab = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 3, 1 }, { 3, 3 } };
VALUE_PAIRS doubleTableStub_ac = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 } };
VALUE_PAIRS doubleTableStub_bc = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 } };
VALUE_PAIRS doubleTableStub_de = { { 124, 124 }, { 125, 124 }, { 125, 125 }, { 126, 125 } };

VALUES emptyValuesStub_ss;
VALUE_PAIRS emptyValuePairsStub_ss;

ELEMENT_LIST elementListExcess = {
	QueryElement(NO_ERROR, 103, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE),
	QueryElement(NO_ERROR, 108, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 113, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NAME)
};
ELEMENT_LIST elementListConnected = {
	QueryElement(NO_ERROR, 101, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE),
	QueryElement(NO_ERROR, 103, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 102, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NAME)
};
ELEMENT_LIST elementListConnectedDuplicate = {
	QueryElement(NO_ERROR, 101, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE),
	QueryElement(NO_ERROR, 102, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 101, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NAME),
	QueryElement(NO_ERROR, 103, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE),
	QueryElement(NO_ERROR, 103, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NAME)
};
ELEMENT_LIST elementListNotConnected = {
	QueryElement(NO_ERROR, 102, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NONE),
	QueryElement(NO_ERROR, 104, DESIGN_ENTITY::CONSTANT, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 105, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NAME),
	QueryElement(NO_ERROR, 107, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE)
};
ELEMENT_LIST elementListNotConnectedDuplicate = {
	QueryElement(NO_ERROR, 101, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE),
	QueryElement(NO_ERROR, 106, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 102, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NAME),
	QueryElement(NO_ERROR, 101, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 107, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::INTEGER),
	QueryElement(NO_ERROR, 103, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NAME)
};
