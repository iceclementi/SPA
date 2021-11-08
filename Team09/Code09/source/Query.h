#pragma once

#include "Common.h"

using namespace std;

static string NO_ERROR = "";
static string SYNTAX_ERROR = "PQL query is syntactically invalid!";
static string SEMANTIC_ERROR = "PQL query is semantically invalid!";
static SYNONYM_INDEX NO_SYNONYM_INDEX = -1;

/*
* Stores a mapping of design entity string to design entity enum
*/
static unordered_map<string, DESIGN_ENTITY> mapStrToDesignEntity = {
	{"stmt", DESIGN_ENTITY::STATEMENT},
	{"read", DESIGN_ENTITY::READ},
	{"print", DESIGN_ENTITY::PRINT},
	{"call", DESIGN_ENTITY::CALL},
	{"while", DESIGN_ENTITY::WHILE},
	{"if", DESIGN_ENTITY::IF},
	{"assign", DESIGN_ENTITY::ASSIGN},
	{"variable", DESIGN_ENTITY::VARIABLE},
	{"constant", DESIGN_ENTITY::CONSTANT},
	{"prog_line", DESIGN_ENTITY::PROG_LINE},
	{"procedure", DESIGN_ENTITY::PROCEDURE}
};

/*
* Enum to describe a possible attribute type
*/
enum class ATTRIBUTE_TYPE {
	NAME, INTEGER, NONE
};

/*
* Stores a mapping of attribute name string to attribute type enum
*/
static unordered_map<string, ATTRIBUTE_TYPE> mapStrToAttrType = {
	{"procName", ATTRIBUTE_TYPE::NAME},
	{"varName", ATTRIBUTE_TYPE::NAME},
	{"value", ATTRIBUTE_TYPE::INTEGER},
	{"stmt#", ATTRIBUTE_TYPE::INTEGER},
};

/*
* Stores a mapping of attribute name string to its valid synonym type enums
*/
static unordered_map<string, set<DESIGN_ENTITY>> mapStrToAttrNameSynTypes = {
	{"procName", {DESIGN_ENTITY::PROCEDURE, DESIGN_ENTITY::CALL}},
	{"varName", {DESIGN_ENTITY::VARIABLE, DESIGN_ENTITY::READ, DESIGN_ENTITY::PRINT}},
	{"value", {DESIGN_ENTITY::CONSTANT}},
	{"stmt#", {DESIGN_ENTITY::STATEMENT, DESIGN_ENTITY::READ, DESIGN_ENTITY::PRINT, DESIGN_ENTITY::CALL, DESIGN_ENTITY::WHILE, DESIGN_ENTITY::IF, DESIGN_ENTITY::ASSIGN}},
};
