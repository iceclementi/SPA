#pragma once

#include <fstream>
#include <sstream>
#include <regex>
#include "Common.h"

namespace Tokenizer {

	enum class TOKEN_TYPE {
		PROCEDURE, READ, PRINT, CALL, WHILE, IF, THEN, ELSE, OPERATOR_1, OPERATOR_2,
		NOT, LOGICAL, CONDITIONAL, LEFTBRACE, RIGHTBRACE, LEFTBRACKET, RIGHTBRACKET,
		SEMICOLON, ASSIGNMENT, VARIABLE, CONSTANT, NONE
	};

	static unordered_map<std::string, Tokenizer::TOKEN_TYPE> const mapToEnum =
	{
		{"procedure", TOKEN_TYPE::PROCEDURE},
		{"read", TOKEN_TYPE::READ},
		{"print", TOKEN_TYPE::PRINT},
		{"call", TOKEN_TYPE::CALL},
		{"while", TOKEN_TYPE::WHILE},
		{"if", TOKEN_TYPE::IF},
		{"then", TOKEN_TYPE::THEN},
		{"else", TOKEN_TYPE::ELSE},
		{"+", TOKEN_TYPE::OPERATOR_1},
		{"-", TOKEN_TYPE::OPERATOR_1},
		{"*", TOKEN_TYPE::OPERATOR_2},
		{"/", TOKEN_TYPE::OPERATOR_2},
		{"%", TOKEN_TYPE::OPERATOR_2},
		{"!", TOKEN_TYPE::NOT},
		{"&&", TOKEN_TYPE::LOGICAL},
		{"||", TOKEN_TYPE::LOGICAL},
		{">", TOKEN_TYPE::CONDITIONAL},
		{"<", TOKEN_TYPE::CONDITIONAL},
		{">=", TOKEN_TYPE::CONDITIONAL},
		{"<=", TOKEN_TYPE::CONDITIONAL},
		{"==", TOKEN_TYPE::CONDITIONAL},
		{"!=", TOKEN_TYPE::CONDITIONAL},
		{"{", TOKEN_TYPE::LEFTBRACE},
		{"}", TOKEN_TYPE::RIGHTBRACE},
		{"(", TOKEN_TYPE::LEFTBRACKET},
		{")", TOKEN_TYPE::RIGHTBRACKET},
		{";", TOKEN_TYPE::SEMICOLON},
		{"=", TOKEN_TYPE::ASSIGNMENT}
	};

	static unordered_map<Tokenizer::TOKEN_TYPE, std::string> const enumToString =
	{
		{ TOKEN_TYPE::PROCEDURE, "PROCEDURE" },
		{ TOKEN_TYPE::READ, "READ" },
		{ TOKEN_TYPE::PRINT, "PRINT" },
		{ TOKEN_TYPE::CALL, "CALL" },
		{ TOKEN_TYPE::WHILE, "WHILE" },
		{ TOKEN_TYPE::IF, "IF" },
		{ TOKEN_TYPE::THEN, "THEN" },
		{ TOKEN_TYPE::ELSE, "ELSE" },
		{ TOKEN_TYPE::OPERATOR_1, "OPERATOR_1" },
		{ TOKEN_TYPE::OPERATOR_2, "OPERATOR_2" },
		{ TOKEN_TYPE::NOT, "NOT" },
		{ TOKEN_TYPE::LOGICAL, "LOGICAL" },
		{ TOKEN_TYPE::CONDITIONAL, "CONDITIONAL" },
		{ TOKEN_TYPE::LEFTBRACE, "LEFTBRACE" },
		{ TOKEN_TYPE::RIGHTBRACE, "RIGHTBRACE" },
		{ TOKEN_TYPE::LEFTBRACKET, "LEFTBRACKET" },
		{ TOKEN_TYPE::RIGHTBRACKET, "RIGHTBRACKET" },
		{ TOKEN_TYPE::SEMICOLON, "SEMICOLON" },
		{ TOKEN_TYPE::ASSIGNMENT, "ASSIGNMENT" },
		{ TOKEN_TYPE::VARIABLE, "VARIABLE" },
		{ TOKEN_TYPE::CONSTANT, "CONSTANT" }
	};

	struct Token {
		TOKEN_TYPE type;
		string value;
	};

	static bool isVariable = false;
	queue<Token> tokenize(istream& stream, bool isExpression);
	/* Parses source program */
	queue<Token> tokenize(istream& stream);
	/* Parses only arithmetic expression */
	queue<Token> tokenize(string& str);
	TOKEN_TYPE getType(string s);
	TOKEN_TYPE getType(char c);
	bool isNumber(string toCheck);
	bool isName(string toCheck);
	Token determineIdentifier(string s, bool isAssignment);
	Token getDelimiterToken(char c);
	Token getConditionalToken(string s);
	Token getLogicalToken(string s);
}
