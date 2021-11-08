#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

PROC_NAMES procTableStub_C = { "procA", "procB", "procC", "procD", "procE" };
VAR_NAMES varTableStub_C = { "a", "b", "c", "f", "h", "i", "l", "m", "n", "p", "q", "v", "x", "y", "z" };
CONSTS constTableStub_C = { "0", "1", "2", "4", "5" };

STMT_NUMS readStmtNumsStub_C = { "16", "7" };
STMT_NUMS printStmtNumsStub_C = { "14" };
STMT_NUMS assignStmtNumsStub_C = { "1", "12", "13", "2", "6", "8", "9" };
STMT_NUMS callStmtNumsStub_C{ "11", "15", "17", "18", "19", "20", "3", "5" };
STMT_NUMS whileStmtNumsStub_C = { "4" };
STMT_NUMS ifStmtNumsStub_C = { "10" };

RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> cTableStub_C = {
	{ "procA", { "procB", "procC", "procD" } },
	{ "procB", { "procC", "procD", "procE" } },
	{ "procC", { "procD" } },
	{ "procD", { "procE" } }
};
RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> cStarTableStub_C = {
	{ "procA", { "procB", "procC", "procD", "procE" } },
	{ "procB", { "procC", "procD", "procE" } },
	{ "procC", { "procD", "procE" } },
	{ "procD", { "procE" } }
};
