#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

PROC_NAMES procTableStub_N = { "procA", "procB", "procC", "procD", "procE" };
VAR_NAMES varTableStub_N = { "a", "b", "c", "f", "h", "i", "l", "m", "n", "p", "q", "v", "x", "y", "z" };
CONSTS constTableStub_N = { "0", "1", "2", "4", "5" };

STMT_NUMS readStmtNumsStub_N = { "16", "7" };
STMT_NUMS printStmtNumsStub_N = { "14" };
STMT_NUMS assignStmtNumsStub_N = { "1", "12", "13", "2", "6", "8", "9" };
STMT_NUMS callStmtNumsStub_N{ "11", "15", "17", "18", "19", "20", "3", "5" };
STMT_NUMS whileStmtNumsStub_N = { "4" };
STMT_NUMS ifStmtNumsStub_N = { "10" };

RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> nTableStubN = {
{ "10", { "11", "15" } },
{ "11", { "12" } },
{ "12", { "13" } },
{ "13", { "14" } },
{ "14", { "17" } },
{ "15", { "16" } },
{ "16", { "17" } },
{ "18", { "19" } },
{ "19", { "20" } },
{ "2", { "3" } },
{ "4", { "5", "8" } },
{ "5", { "6" } },
{ "6", { "7" } },
{ "7", { "4" } },
{ "8", { "9" } }
};
RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> nStarTableStubN = {
{ "10", { "11", "12", "13", "14", "15", "16", "17" }},
{ "11", { "12", "13", "14", "17" } },
{ "12", { "13", "14", "17" } },
{ "13", { "14", "17" }},
{ "14", { "17" } },
{ "15", { "16", "17" }},
{ "16", { "17" } },
{ "18", { "19", "20" }},
{ "19", { "20" } },
{ "2", { "3" } },
{ "4", { "4", "5", "6", "7", "8", "9" }},
{ "5", { "4", "5", "6", "7", "8", "9" } },
{ "6", { "4", "5", "6", "7", "8", "9" } },
{ "7", { "4", "5", "6", "7", "8", "9" } },
{ "8", { "9" } }
};
