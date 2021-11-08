#pragma once

#include "Common.h"
#include "Tokenizer.h"

using namespace std;
using namespace Tokenizer;

typedef queue<Token> TOKENS;
typedef stack<Token> OPSTACK;
typedef deque<Token> TOKEN_LIST;

namespace ExpressionParser {
	class ArithmeticExpressionParser {
	private:
		static TOKENS expression;
		static string expressionString;
		static VARS variables;
		static VARS constants;
		static void parseExpr();
		static void parseExprAlt();
		static void parseTerm();
		static void parseTermAlt();
		static void parseFactor();
		static Token expects(TOKEN_TYPE type);
		static Token peekNextToken();
		static void init(TOKENS& expression);

	public:
		static PAIR<VARS> parse(TOKENS& expression);
	};

	class ConditionalExpressionParser {
	private:
		static TOKEN_LIST expression;
		static string expressionString;
		static VARS variables;
		static VARS constants;
		static void parseExpr(int begin, int end);
		static void parseRelExpr(int begin, int end);
		static void parseRelFactor(int begin, int end);
		static bool isNegateStatement(int begin, int end);
		static int findLogicalIndex(int begin, int end);
		static int findConditionalIndex(int begin, int end);
		static void init(TOKENS &expression);

	public:
		static PAIR<VARS> parse(TOKENS& expression);
		static string getExpression();
	};
}

class RPNConverter {
private:
	static TOKENS expression;
	static OPSTACK ops;
	static string output;
	static int getPrecedence(Token op);
	static Token popExpression();
	static Token popOpStack();
	static void appendOutput(string str);
public:
	static string convert(TOKENS expression);
	static string convert(string expressionStr);
};
