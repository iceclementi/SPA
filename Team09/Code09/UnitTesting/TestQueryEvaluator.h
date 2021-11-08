#pragma once

#include "Common.h"
#include "QueryEvaluator.h"
#include "QueryOptimiser.h"

typedef unordered_map<STMT_NUM, PAIR<VAR_NAME, PATTERN>> ASSIGN_PATTERN_TABLE;

PROC_NAMES procTableStub_QE = { "main" };
VAR_NAMES varTableStub_QE = { "var" };
CONSTS constTableStub_QE = { "0", "1", "2", "3", "4", "5" };

STMT_NUMS readStmtNumsStub_QE = { "2" };
STMT_NUMS printStmtNumsStub_QE = { "3" };
STMT_NUMS assignStmtNumsStub_QE = { "1", "10", "5", "7", "9" };
STMT_NUMS whileStmtNumsStub_QE = { "4", "8" };
STMT_NUMS ifStmtNumsStub_QE = { "6" };
STMT_NUMS callStmtNumsStub_QE = { "2" };

RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> fTableStub_QE = {
	{ "1", { "2" } },
	{ "2", { "3" } },
	{ "3", { "4" } },
	{ "4", { "6" } },
	{ "7", { "8" } }
};
RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> fStarTableStub_QE = {
	{ "1", { "2", "3", "4", "6" } },
	{ "2", { "3", "4", "6" } },
	{ "3", { "4", "6" } },
	{ "4", { "6" } },
	{ "7", { "8" } }
};
RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> pTableStub_QE = {
	{ "4", { "5" } },
	{ "6", { "10", "7", "8" } },
	{ "8", { "9" } }
};
RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> pStarTableStub_QE = {
	{ "4", { "5" } },
	{ "6", { "10", "7", "8", "9" } },
	{ "8", { "9" } }
};
RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> usesProcTableStub_QE = {
	{ "main", { "var" } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> usesStmtTableStub_QE = {
	{ "3", { "var" } },
	{ "4", { "var" } },
	{ "6", { "var" } },
	{ "8", { "var" } }
};
RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> modifiesProcTableStub_QE = {
	{ "main", { "var" } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> modifiesStmtTableStub_QE = {
	{ "1", { "var" } },
	{ "10", { "var" } },
	{ "2", { "var" } },
	{ "4", { "var" } },
	{ "5", { "var" } },
	{ "6", { "var" } },
	{ "7", { "var" } },
	{ "8", { "var" } },
	{ "9", { "var" } }
};
ASSIGN_PATTERN_TABLE assignPatternStub_QE = {
	{ "1", { "var", " 0 " } },
	{ "10", { "var", " 2  2  + " } },
	{ "5", { "var", " 1  1  + " } },
	{ "7", { "var", " 3  3  +  4  +  5  + " } },
	{ "9", { "var", " 1  1  + " } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> whilePatternStub_QE = {
	{ "4", { "var" } },
	{ "8", { "var" } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> ifPatternStub_QE = {
	{ "6", { "var" } }
};
