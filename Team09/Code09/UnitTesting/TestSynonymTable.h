#pragma once

#include "SynonymTable.h"

using namespace std;


SYN_TABLE table;
SYN_INDEX_MAP synonymToIndex;
INDEX currentIndex = 0;

SYN_TABLE table_Stub = {};


SYN_INDEX_MAP synonymToIndex_Stub = { 
	{'s', 1}, 
	{'r', 1}, 
	{'p', 1},
	{'c', 1}, 
	{'w', 1}, 
	{'i', 1},
	{'a', 1}, 
	{'v', 1},
	{'d', 1},
	{'q', 1},
};


