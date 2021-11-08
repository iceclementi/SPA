#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

typedef unordered_map<STMT_NUM, PAIR<VAR_NAME, PATTERN>> ASSIGN_PATTERN_TABLE;


PROC_NAMES procTableStub_WITH = { "QWERTYUIOPASDFGHJKLZXCVBNM1234567890", "A", "H", "J", "I", "E", "B"};
VAR_NAMES varTableStub_WITH = { "qwertyuiopasdfghjklzxcvbnm1234567890", "c", "b", "a", "x", "y", "z"};
CONSTS constTableStub_WITH = { "123456789012345678901234567890", "0", "1", "2", "3", "4", "5", "6", "7",
"10", "11", "12", "13" };

STMT_NUMS allStmtNumsStub_WITH = { "34", "1", "24", "25", "26", "27", "28", "29", "30", "31", "33", "32", "22", 
"23", "21", "2", "7", "12", "11", "10", "16", "18", "17", "15", "14", "13", "9", "8", "6", "5", "19", "4", "3", "20"};
STMT_NUMS readStmtNumsStub_WITH = { "32", "2" };
STMT_NUMS printStmtNumsStub_WITH = { "33", "20" };
STMT_NUMS assignStmtNumsStub_WITH = { "30", "34", "24", "25", "26", "27", "28", "29" };
STMT_NUMS whileStmtNumsStub_WITH = { "11", "10", "8", "17", "14", "5", "13", "3" };
STMT_NUMS ifStmtNumsStub_WITH = { "21", "4", "15", "9", "6" };
STMT_NUMS callStmtNumsStub_WITH = { "31", "1", "22", "18", "23", "7", "12", "16", "19" };
