#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

PROC_NAMES procTableStub_U = { "um", "um2" };
VAR_NAMES varTableStub_U = { "A", "B", "C", "D", "E", "F", "G", "a", "b", "c", "d", "e", "f", "g" };
CONSTS constTableStub_U = { "0", "1", "2", "3", "4", "5", "6", "7", "8" };

STMT_NUMS readStmtNumsStub_U = { "14", "15", "16", "2", "22", "25", "28" };
STMT_NUMS printStmtNumsStub_U = { "11", "12", "13", "21", "24", "27", "3", "30" };
STMT_NUMS assignStmtNumsStub_U = { "1", "10", "18", "19", "20", "23", "26", "29", "31", "32", "35", "37", "5", "6", "7", "8", "9" };
STMT_NUMS whileStmtNumsStub_U = { "17", "33", "36" };
STMT_NUMS ifStmtNumsStub_U = { "34", "4" };

RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> usesProcTableStub_U = {
	{ "um", { "a", "b", "c" } },
	{ "um2", { "A", "B", "C", "D", "E", "F" } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> usesStmtTableStub_U = {
	{ "3", { "c" } },
	{ "4", { "a", "b", "c" } },
	{ "5", { "a", "b" } },
	{ "6", { "a" } },
	{ "7", { "c" } },
	{ "8", { "a", "b" } },
	{ "9", { "c" } },
	{ "10", { "a" } },
	{ "11", { "a" } },
	{ "12", { "b" } },
	{ "13", { "c" } },
	{ "17", { "A", "C", "D" } },
	{ "18", { "C" } },
	{ "19", { "D" } },
	{ "20", { "C", "D" } },
	{ "21", { "A" } },
	{ "23", { "B", "E", "F" } },
	{ "24", { "B" } },
	{ "26", { "E" } },
	{ "27", { "E" } },
	{ "29", { "B", "C", "E" } },
	{ "30", { "B" } },
	{ "31", { "A", "B", "F" } },
	{ "32", { "C" } },
	{ "33", { "A", "B", "E", "F" } },
	{ "34", { "A", "B", "E", "F" } },
	{ "35", { "B" } },
	{ "36", { "A", "E", "F" } },
	{ "37", { "E", "F" } },
};
