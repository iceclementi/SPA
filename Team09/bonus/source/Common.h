#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// Enum to describe a design entity
enum class DESIGN_ENTITY {
	STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROG_LINE, PROCEDURE, NONE
};

// Enum to describe a relationship type
enum class RELATIONSHIP_TYPE {
	FOLLOWS, FOLLOWS_STAR, PARENT, PARENT_STAR, USES_STMT, USES_PROC, MODIFIES_STMT, MODIFIES_PROC,
	CALLS, CALLS_STAR, NEXT, NEXT_STAR, NEXT_BIP, NEXT_BIP_STAR, AFFECTS, AFFECTS_STAR, AFFECTS_BIP,
	AFFECTS_BIP_STAR, WHILE_PATTERN, IF_PATTERN, INVALID
};

// Custom hashing function for pairs
namespace std {
    template <typename T1, typename T2>
    struct hash<pair<T1, T2>>
    {
		size_t operator()(const pair<T1, T2>& pair) const {
			auto firstHash = hash<T1>{}(pair.first);
			auto secondHash = hash<T2>{}(pair.second);
			return firstHash ^ (secondHash << 1);
		}
    };
}

template <typename K, typename V>
V getValueWithKey(unordered_map<K, V>& keyToIndex, K key) {
	auto it = keyToIndex.find(key);
	return it != keyToIndex.end() ? it->second : V();
}

template <typename T>
using S = unordered_set<T>;
template <typename T1, typename T2>
using M = unordered_map<T1, T2>;
template <typename T1, typename T2 = T1>
using PAIR = pair<T1, T2>;
template <typename T1, typename T2 = T1>
using PAIRS = S<PAIR<T1, T2>>;

typedef string VAR;
typedef S<VAR> VARS;
typedef vector<VAR> VAR_LIST;
typedef string STMT_NUM;
typedef S<STMT_NUM> STMT_NUMS;
typedef string VAR_NAME;
typedef S<VAR_NAME> VAR_NAMES;
typedef string PROC_NAME;
typedef S<PROC_NAME> PROC_NAMES;
typedef string CONST;
typedef S<CONST> CONSTS;
typedef string PROG_LINE;
typedef S<PROG_LINE> PROG_LINES;
typedef string PATTERN;

typedef string SYNONYM;
typedef int SYNONYM_INDEX;
typedef S<SYNONYM_INDEX> SYNONYM_INDICES;
typedef vector<SYNONYM_INDEX> SYNONYM_INDEX_LIST;

typedef int INDEX;
typedef S<INDEX> INDICES;
typedef vector<INDEX> INDEX_LIST;
typedef int VALUE;
typedef vector<VALUE> VALUE_LIST;
typedef S<VALUE> VALUES;
typedef pair<VALUE, VALUE> VALUE_PAIR;
typedef S<VALUE_PAIR> VALUE_PAIRS;
typedef pair<VALUES, VALUES> VALUES_PAIR;
typedef set<VALUE_LIST> VALUE_LISTS;

typedef vector<VALUE_LIST> INDEXED_INTERMEDIATE_ANSWER;
typedef vector<VAR_LIST> STRING_INTERMEDIATE_ANSWER;
typedef string RESULT;
typedef S<RESULT> RESULTS;
typedef list<RESULT> ANSWER;

const string TRUE = "TRUE";
const string FALSE = "FALSE";
