#pragma once

#include "Tokenizer.h"
#include <queue>

using namespace Tokenizer;

namespace MockToken {
	Token createToken(string v);
	Token createToken(string v, bool isVariable);
	queue<Token> create_test1();
	queue<Token> allSymbols();
	queue<Token> allIdentifier();
	queue<Token> callStmtTokens(string procedure);
	queue<Token> readStmtTokens(string modifiedVariable);
	queue<Token> printStmtTokens(string usedVariable);
	queue<Token> createTokens(vector<string> expr);
}
