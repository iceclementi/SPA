#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

PROC_NAMES procTableStub_M = { "um", "um2" };
VAR_NAMES varTableStub_M = { "A", "B", "C", "D", "E", "F", "G", "a", "b", "c", "d", "e", "f", "g" };
CONSTS constTableStub_M = { "0", "1", "2", "3", "4", "5", "6", "7", "8" };

STMT_NUMS allStmtNumsStub_M = {
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
	"11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
	"21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
	"31", "32", "33", "34", "35", "36", "37"
};
STMT_NUMS readStmtNumsStub_M = { "14", "15", "16", "2", "22", "25", "28" };
STMT_NUMS printStmtNumsStub_M = { "11", "12", "13", "21", "24", "27", "3", "30" };
STMT_NUMS assignStmtNumsStub_M = { "1", "10", "18", "19", "20", "23", "26", "29", "31", "32", "35", "37", "5", "6", "7", "8", "9" };
STMT_NUMS whileStmtNumsStub_M = { "17", "33", "36" };
STMT_NUMS ifStmtNumsStub_M = { "34", "4" };

RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> modifiesProcTableStub_M = {
	{ "um", { "a", "c", "d", "e", "f", "g" } },
	{ "um2", { "A", "B", "C", "D", "F", "G" } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> modifiesStmtTableStub_M = {
	{ "1", { "a" } },
	{ "10", { "e" } },
	{ "14", { "e" } },
	{ "15", { "f" } },
	{ "16", { "g" } },
	{ "17", { "A", "C", "D" } },
	{ "18", { "C" } },
	{ "19", { "D" } },
	{ "2", { "d" } },
	{ "20", { "A" } },
	{ "22", { "B" } },
	{ "23", { "B" } },
	{ "25", { "A" } },
	{ "26", { "G" } },
	{ "28", { "B" } },
	{ "29", { "B" } },
	{ "31", { "G" } },
	{ "32", { "F" } },
	{ "33", { "C", "D" } },
	{ "34", { "C", "D" } },
	{ "35", { "C" } },
	{ "36", { "D" } },
	{ "37", { "D" } },
	{ "4", { "c", "d", "e", "f" } },
	{ "5", { "c" } },
	{ "6", { "d" } },
	{ "7", { "e" } },
	{ "8", { "c" } },
	{ "9", { "f" } }
};
