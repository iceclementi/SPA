#include "ExpressionParser.h"

namespace ExpressionParser {
	
	/********** Arithmetic Expression Parser ***********/

	TOKENS ArithmeticExpressionParser::expression = TOKENS();
	string ArithmeticExpressionParser::expressionString = "";
	VARS ArithmeticExpressionParser::variables = VARS();
	VARS ArithmeticExpressionParser::constants = VARS();

	PAIR<VARS> ArithmeticExpressionParser::parse(TOKENS &tokens) {
		init(tokens);
		parseExpr();

		if (!expression.empty()) {
			string unresolvedSymbols = "";
			while (!expression.empty()) {
				unresolvedSymbols.append(expression.front().value);
				expression.pop();
			}
			throw ": unresolved symbols '" + unresolvedSymbols + "' at end of expression";
		}

		return { variables, constants };
	}

	void ArithmeticExpressionParser::init(TOKENS &tokens) {
		expression = tokens;
		expressionString.clear();
		constants.clear();
		variables.clear();
	}

	void ArithmeticExpressionParser::parseExpr() {
		parseTerm();
		parseExprAlt();
	}

	void ArithmeticExpressionParser::parseExprAlt() {
		Token nextToken = peekNextToken();
		switch (nextToken.type) {
		case TOKEN_TYPE::OPERATOR_1:
			expressionString.append(expects(TOKEN_TYPE::OPERATOR_1).value);
			parseTerm();
			parseExprAlt();
			break;
		case TOKEN_TYPE::RIGHTBRACKET: case TOKEN_TYPE::NONE: // End
			break;
		default:
			throw ": invalid Arithmetic Expression syntax met with " + nextToken.value + "\n" + expressionString;
		}
	}

	void ArithmeticExpressionParser::parseTerm() {
		parseFactor();
		parseTermAlt();
	}

	void ArithmeticExpressionParser::parseTermAlt() {
		Token nextToken = peekNextToken();
		switch (nextToken.type) {
		case TOKEN_TYPE::OPERATOR_2:
			expressionString.append(expects(TOKEN_TYPE::OPERATOR_2).value);
			parseFactor();
			parseTermAlt();
			break;
		case TOKEN_TYPE::OPERATOR_1: case TOKEN_TYPE::RIGHTBRACKET: case TOKEN_TYPE::NONE: // End
			break;
		default:
			throw ": invalid Arithmetic Expression syntax met with " + nextToken.value + "\n" + expressionString;
		}
	}

	void ArithmeticExpressionParser::parseFactor() {
		Token nextToken = peekNextToken();
		switch (nextToken.type) {
		case TOKEN_TYPE::VARIABLE:
			expressionString.append(expects(TOKEN_TYPE::VARIABLE).value);
			variables.insert(nextToken.value);
			break;
		case TOKEN_TYPE::CONSTANT:
			expressionString.append(expects(TOKEN_TYPE::CONSTANT).value);
			constants.insert(nextToken.value);
			break;
		case TOKEN_TYPE::LEFTBRACKET:
			expressionString.append(expects(TOKEN_TYPE::LEFTBRACKET).value);
			parseExpr();
			expressionString.append(expects(TOKEN_TYPE::RIGHTBRACKET).value);
			break;
		case TOKEN_TYPE::NONE:
			throw ": expected VARIABLE or CONSTANT";
		default:
			throw ": expected VARIABLE or CONSTANT but was met with " + nextToken.value + "\n" + expressionString;
		}
	}

	Token ArithmeticExpressionParser::peekNextToken() {
		if (expression.empty()) {
			return { TOKEN_TYPE::NONE, "" };
		}

		return expression.front();
	}

	Token ArithmeticExpressionParser::expects(TOKEN_TYPE type) {
		auto e = enumToString.find(type);
		string expectedType = e->second;
		if (expression.empty()) {
			throw ": missing " + expectedType;
		}
		else {
			Token nextToken = expression.front();
			if (nextToken.type != type) {
				throw ": expected " + expectedType + " but was met with " + nextToken.value;
			}
			else {
				expression.pop();
				return nextToken;
			}
		}
	}


	/********** Conditional Expression Parser ***********/

	TOKEN_LIST ConditionalExpressionParser::expression = TOKEN_LIST();;
	string ConditionalExpressionParser::expressionString = "";
	VARS ConditionalExpressionParser::variables = VARS();
	VARS ConditionalExpressionParser::constants = VARS();

	string ConditionalExpressionParser::getExpression() {
		return expressionString;
	}

	PAIR<VARS> ConditionalExpressionParser::parse(TOKENS &tokens) {
		init(tokens);
		parseExpr(0, expression.size() - 1);

		return { variables, constants };
	}

	void ConditionalExpressionParser::init(TOKENS& tokens) {
		expression.clear();

		while (!tokens.empty()) {
			expression.push_back(tokens.front());
			tokens.pop();
		}

		// Remove starting and ending brackets
		expression.pop_front();
		expression.pop_back();

		expressionString.clear();
		constants.clear();
		variables.clear();
	}

	void ConditionalExpressionParser::parseExpr(int begin, int end) {
		if (isNegateStatement(begin, end)) {
			// Ignore beginning '!(' and ending ')'
			expressionString.append("!( ");
			parseExpr(begin + 2, end - 1);
			expressionString.append(" ) ");
		}
		else {
			int logicalOpIndex = findLogicalIndex(begin, end);
			if (logicalOpIndex < 0) {
				parseRelExpr(begin, end);
			}
			else {
				// Split and parse both conditionals separately
				expressionString.append(" ( ");
				parseExpr(begin + 1, logicalOpIndex - 2);
				expressionString.append(" ) " + expression.at(logicalOpIndex).value + " ( ");
				parseExpr(logicalOpIndex + 2, end - 1);
				expressionString.append(" ) ");
			}
		}
	}

	void ConditionalExpressionParser::parseRelExpr(int begin, int end) {
		int conditionalOpIndex = findConditionalIndex(begin, end);
		if (conditionalOpIndex < 0) {
			throw ": missing relative expression in CONDITIONAL\n" + expressionString;
		}

		parseRelFactor(begin, conditionalOpIndex - 1);
		expressionString.append(" " + expression.at(conditionalOpIndex).value + " ");
		parseRelFactor(conditionalOpIndex + 1, end);
	}

	void ConditionalExpressionParser::parseRelFactor(int begin, int end) {
		TOKEN_LIST arithmeticExprList(expression.begin() + begin, expression.begin() + end + 1);
		TOKENS arithmeticExpr(arithmeticExprList);

		for (int i = begin; i <= end; ++i) {
			expressionString.append(expression.at(i).value + " ");
		}

		try {
			auto variablesConstants = ArithmeticExpressionParser::parse(arithmeticExpr);
			variables.insert(variablesConstants.first.begin(), variablesConstants.first.end());
			constants.insert(variablesConstants.second.begin(), variablesConstants.second.end());
		}
		catch (exception) {
			throw ": expected VARIABLE, CONSTANT or EXPRESSION\n" + expressionString;
		}
	}

	bool ConditionalExpressionParser::isNegateStatement(int begin, int end) {
		if (end - begin + 1 < 3) {
			return false;
		}

		if (expression.at(begin).type != TOKEN_TYPE::NOT) {
			return false;
		}
		else if ((expression.at(begin + 1).type == TOKEN_TYPE::LEFTBRACKET)
			&& (expression.at(end).type == TOKEN_TYPE::RIGHTBRACKET)) {
			return true;
		}
		else {
			throw ": syntax error with brackets in CONDITIONAL";
		}
	}

	int ConditionalExpressionParser::findLogicalIndex(int begin, int end) {
		if (end - begin + 1 < 5) return -1;
		if ((expression.at(begin).type != TOKEN_TYPE::LEFTBRACKET) || (expression.at(end).type != TOKEN_TYPE::RIGHTBRACKET)) return -1;

		int level = 0;
		for (int i = begin; i < end; ++i) {
			switch (expression.at(i).type) {
			case TOKEN_TYPE::LEFTBRACKET:
				++level;
				break;
			case TOKEN_TYPE::RIGHTBRACKET:
				if (--level < 0) {
					throw ": syntax error with brackets in CONDITIONAL";
				}
				break;
			case TOKEN_TYPE::LOGICAL:
				if (level != 0) break;
				
				if (expression.at(i - 1).type != TOKEN_TYPE::RIGHTBRACKET || expression.at(i + 1).type != TOKEN_TYPE::LEFTBRACKET) {
					throw ": syntax error with brackets in CONDITIONAL";
				}

				for (int j = i + 1; j <= end; ++j) {
					if (expression.at(j).type == TOKEN_TYPE::LEFTBRACKET) {
						++level;
					}
					else if (expression.at(j).type == TOKEN_TYPE::RIGHTBRACKET && (--level < 0)) {
						throw ": syntax error with brackets in CONDITIONAL";
					}
				}

				if (level == 0) {
					return i;
				}
				else {
					throw ": syntax error with brackets in CONDITIONAL";
				}

				break;
			default:
				break;
			}
		}

		return -1;
	}

	int ConditionalExpressionParser::findConditionalIndex(int begin, int end) {
		if (end - begin + 1 < 3) {
			return -1;
		}

		for (int i = begin; i <= end; ++i) {
			if (expression.at(i).type == TOKEN_TYPE::CONDITIONAL) {
				return i;
			}
		}

		return -1;
	}
}

/********** RPN Converter ***********/

TOKENS RPNConverter::expression = TOKENS();
OPSTACK RPNConverter::ops = OPSTACK();
string RPNConverter::output = "";

int RPNConverter::getPrecedence(Token op) {
	switch (op.type) {
	case TOKEN_TYPE::OPERATOR_1:
		return 1;
	case TOKEN_TYPE::OPERATOR_2:
		return 2;
	default:
		return -1;
	}
}

Token RPNConverter::popExpression() {
	Token token = expression.front();
	expression.pop();
	return token;
}

Token RPNConverter::popOpStack() {
	Token token = ops.top();
	ops.pop();
	return token;
}

void RPNConverter::appendOutput(string str) {
	output.append(" " + str + " ");
}

string RPNConverter::convert(TOKENS toConvert) {
	expression = toConvert;
	output.clear();
	ops = OPSTACK();

	while (!expression.empty()) {
		auto current = popExpression();

		switch (current.type) {
		case TOKEN_TYPE::VARIABLE: case TOKEN_TYPE::CONSTANT:
			appendOutput(current.value);
			break;
		case TOKEN_TYPE::LEFTBRACKET:
			ops.push(current);
			break;
		case TOKEN_TYPE::RIGHTBRACKET:
			while (!ops.empty() &&
				ops.top().type != TOKEN_TYPE::LEFTBRACKET) {
				appendOutput(popOpStack().value);
			}

			ops.pop();
			break;
		default: // + - * / % operators
			while (!ops.empty() &&
				getPrecedence(current) <= getPrecedence(ops.top())) {
				appendOutput(popOpStack().value);
			}

			ops.push(current);
			break;
		}
	}

	while (!ops.empty()) {
		appendOutput(popOpStack().value);
	}

	return output;
}

string RPNConverter::convert(string str) {
	try {
		TOKENS tokens = tokenize(str);
		TOKENS tokensCopy(tokens);

		ExpressionParser::ArithmeticExpressionParser::parse(tokens);
		return convert(tokensCopy);
	}
	catch (string& e) {
		cout << e << endl;
		return "";
	}
	catch (const char*& e) {
		cout << e << endl;
		return "";
	}
	catch (...) {
		cout << "Error with arithmetic parsing" << endl;
		return "";
	}
}
