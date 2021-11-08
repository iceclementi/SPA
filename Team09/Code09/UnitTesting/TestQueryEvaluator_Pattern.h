#pragma once

#include "Common.h"
#include "QueryEvaluator.h"

typedef unordered_map<STMT_NUM, PAIR<VAR_NAME, PATTERN>> ASSIGN_PATTERN_TABLE;

PROC_NAMES procTableStub_Pat = { "procA" };
VAR_NAMES varTableStub_Pat = { 
	"a", "b", "e", "d", "c", "f", "loops", "g", "j", "k", "i", "h", "n", "u", "x", "p", "w", 
	"abcdefghijklmnopqrstuvwxyz", "ifs", "r", "q", "y", "s" 
};
CONSTS constTableStub_Pat = { "1", "100", "987654321", "2", "0", "1234567890", "10", "123456789012345678901234567890", "123", "3", "1000", "50" };

STMT_NUMS readStmtNumsStub_Pat = { "18", "9", "66", "1", "2", "28", "6", "16", "31", "35", "36", "46", "49", "63", "78", "84" };
STMT_NUMS printStmtNumsStub_Pat = { "8", "37", "29", "85", "21", "13", "20", "34", "43", "58", "60", "61", "62", "70", "76", "86", "87", "89" };
STMT_NUMS assignStmtNumsStub_Pat = { "26", "72", "7", "10", "19", "12", "22", "15", "32", "39", "40", "42", "51", "56", "57", "59", "64", "68", "74", "80", "82", "88" };
STMT_NUMS whileStmtNumsStub_Pat = { "11", "4", "33", "38", "30", "25", "23", "55", "54", "53", "52", "50", "48", "47", "45", "44", "41" };
STMT_NUMS ifStmtNumsStub_Pat = { "17", "81", "24", "14", "83", "27", "67", "5", "3", "79", "77", "75", "73", "71", "69", "65" };

ASSIGN_PATTERN_TABLE assignPatternStub_Pat = {
	{ "26", { "a", " b  1  c  *  + " } },
	{ "72", { "a", " a  1  + " } },
	{ "7", { "e", " e  1  1  /  - " } },
	{ "10", { "e", " e  1  1  /  + " } },
	{ "19", { "e", " e  100  987654321  /  + " } },
	{ "12", { "b", " d  e  * " } },
	{ "22", { "e", " a  b  -  c  +  d  - " } },
	{ "15", { "c", " 100 " } },
	{ "32", { "a", " f " } },
	{ "39", { "e", " e  1  + " } },
	{ "40", { "loops", " 10 " } },
	{ "42", { "a", " b  c  + " } },
	{ "51", { "g", " a  b  *  c  /  d  +  e  -  f  % " } },
	{ "56", { "loops", " loops  10  * " } },
	{ "57", { "loops", " loops  100  - " } },
	{ "59", { "d", " 987654321 " } },
	{ "64", { "b", " b  10  / " } },
	{ "68", { "d", " n  u  * " } },
	{ "74", { "p", " 0 " } },
	{ "80", { "f", " 1 " } },
	{ "82", { "i", " abcdefghijklmnopqrstuvwxyz " } },
	{ "88", { "e", " a  b  100  -  50  2  *  +  +  c  1  *  -  d  2  /  2  *  - " } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> whilePatternStub_Pat = {
	{ "11", { "e" } },
	{ "4", { "e" } },
	{ "33", { "c" } },
	{ "38", { "e" } },
	{ "30", { "d", "a" } },
	{ "25", { "f" } },
	{ "23", { "a", "b", "c", "d" } },
	{ "55", { "j", "k" } },
	{ "54", { "i", "j" } },
	{ "53", { "h", "i" } },
	{ "52", { "g", "h" } },
	{ "50", { "f", "g" } },
	{ "48", { "e", "f" } },
	{ "47", { "d", "e" } },
	{ "45", { "c", "d" } },
	{ "44", { "b", "c" } },
	{ "41", { "a", "b" } }
};
RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> ifPatternStub_Pat = {
	{ "17", { "a" ,"e" } },
	{ "81", { "abcdefghijklmnopqrstuvwxyz" } },
	{ "24", { "e" } },
	{ "14", { "e" ,"c" } },
	{ "83", { "ifs" } },
	{ "27", { "f" ,"d" } },
	{ "67", { "f" ,"j" ,"s" ,"h" } },
	{ "5", { "e" } },
	{ "3", { "a" ,"b" ,"c" ,"d" } },
	{ "79", { "r" ,"h" } },
	{ "77", { "e" ,"w" } },
	{ "75", { "u" ,"x" } },
	{ "73", { "q" } },
	{ "71", { "d" ,"a" } },
	{ "69", { "y" ,"f" ,"x" } },
	{ "65", { "f" ,"s" } }
};
