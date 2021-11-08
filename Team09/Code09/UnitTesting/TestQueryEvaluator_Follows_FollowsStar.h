#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

PROC_NAMES procTableStub_F = { "main" };
VAR_NAMES varTableStub_F = { "var", "x" };
CONSTS constTableStub_F = { "0", "1", "10", "11", "12", "2", "3", "4", "5", "6", "7", "8", "9" };

STMT_NUMS readStmtNumsStub_F = { "13", "18", "2", "21", "24", "6", "9" };
STMT_NUMS printStmtNumsStub_F = { "10", "15", "17", "22", "26", "3", "7" };
STMT_NUMS assignStmtNumsStub_F = { "1", "11", "14", "16", "20", "23", "25", "28", "4" };
STMT_NUMS whileStmtNumsStub_F = { "27", "5", "8" };
STMT_NUMS ifStmtNumsStub_F = { "12", "19" };

RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> fTableStub_F = {
	{ "1", { "2" } },
	{ "11", { "12" } },
	{ "13", { "14" } },
	{ "14", { "15" } },
	{ "17", { "18" } },
	{ "18", { "19" } },
	{ "19", { "27" } },
	{ "2", { "3" } },
	{ "20", { "21" } },
	{ "21", { "22" } },
	{ "23", { "24" } },
	{ "24", { "25" } },
	{ "25", { "26" } },
	{ "3", { "4" } },
	{ "4", { "5" } },
	{ "5", { "17" } },
	{ "6", { "7" } },
	{ "7", { "8" } },
	{ "8", { "11" } },
	{ "9", { "10" } }
};
RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> fStarTableStub_F = {
	{ "1", { "17", "18", "19", "2", "27", "3", "4", "5" } },
	{ "11", { "12" } },
	{ "13", { "14", "15" } },
	{ "14", { "15" } },
	{ "17", { "18", "19", "27" } },
	{ "18", { "19", "27" } },
	{ "19", { "27" } },
	{ "2", { "17", "18", "19", "27", "3", "4", "5" } },
	{ "20", { "21", "22" } },
	{ "21", { "22" } },
	{ "23", { "24", "25", "26" } },
	{ "24", { "25", "26" } },
	{ "25", { "26" } },
	{ "3", { "17", "18", "19", "27", "4", "5" } },
	{ "4", { "17", "18", "19", "27", "5" } },
	{ "5", { "17", "18", "19", "27" } },
	{ "6", { "11", "12", "7", "8" } },
	{ "7", { "11", "12", "8" } },
	{ "8", { "11", "12" } },
	{ "9", { "10" } }
};