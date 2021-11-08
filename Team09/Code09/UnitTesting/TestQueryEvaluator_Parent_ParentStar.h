#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

PROC_NAMES procTableStub_P = { "main" };
VAR_NAMES varTableStub_P = { "var", "x" };
CONSTS constTableStub_P = { "0", "1", "10", "11", "12", "2", "3", "4", "5", "6", "7", "8", "9" };

STMT_NUMS readStmtNumsStub_P = { "16", "3", "7" };
STMT_NUMS printStmtNumsStub_P = { "17", "4", "8" };
STMT_NUMS assignStmtNumsStub_P = { "1", "10", "12", "13", "15", "19", "2", "21", "22", "23", "6" };
STMT_NUMS whileStmtNumsStub_P = { "18", "5", "9" };
STMT_NUMS ifStmtNumsStub_P = { "11", "14", "20" };

RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> pTableStub_P = {
	{ "11", { "12", "13" } },
	{ "14", { "15", "16", "17", "18", "20", "23" } },
	{ "18", { "19" } },
	{ "20", { "21", "22" } },
	{ "5", { "11", "6", "7", "8", "9" } },
	{ "9", { "10" } }
};
RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> pStarTableStub_P = {
	{ "11", { "12", "13" } },
	{ "14", { "15", "16", "17", "18", "19", "20", "21", "22", "23" } },
	{ "18", { "19" } },
	{ "20", { "21", "22" } },
	{ "5", { "10", "11", "12", "13", "6", "7", "8", "9" } },
	{ "9", { "10" } }
};
