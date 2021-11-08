#include "SourceParser.h"

namespace SourceParser {

	static unordered_map<Tokenizer::TOKEN_TYPE, TNode::NODE_TYPE> const tokenToNode =
	{
		{TOKEN_TYPE::PROCEDURE, TNode::NODE_TYPE::procedureStmtLst},
		{TOKEN_TYPE::WHILE, TNode::NODE_TYPE::whileStmtLst},
		{TOKEN_TYPE::IF, TNode::NODE_TYPE::thenStmtLst},
		{TOKEN_TYPE::ELSE, TNode::NODE_TYPE::elseStmtLst}
	};

	static unordered_map< TNode::NODE_TYPE, string> const nodeToString =
	{
		{TNode::NODE_TYPE::procedureStmtLst, "procedureStmtLst"},
		{TNode::NODE_TYPE::whileStmtLst, "whileStmtLst"},
		{TNode::NODE_TYPE::thenStmtLst, "thenStmtLst"},
		{TNode::NODE_TYPE::elseStmtLst, "elseStmtLst"},
		{TNode::NODE_TYPE::callStmt, "CallStmt"},
		{TNode::NODE_TYPE::printStmt, "PrintStmt"},
		{TNode::NODE_TYPE::readStmt, "ReadStmt"}
	};

	//constructor for parser class
	SourceParser::SourceParser(string fileName) : fileName(fileName) {
		tokens = initTokens();
	};

	//constructor for testing
	SourceParser::SourceParser(queue<Token> tokens) : tokens(tokens) {};

	TOKENS SourceParser::initTokens() {
		std::ifstream file;
		file.open(fileName);
		std::istream& fileStream = file;

		try {
			return tokenize(fileStream);
		}
		catch (string& e) {
			cout << e << endl;
		}
		catch (const char*& e) {
			cout << e << endl;
		}

		return TOKENS();
	}

	// Consumes the current token if token == expected
	// Throws error for unexpected tokens
	Token SourceParser::expects(TOKEN_TYPE type) {
		auto e = enumToString.find(type);
		string expectedType = e->second;
		if (tokens.empty()) {
			throw ": missing " + expectedType;
		}
		else {
			Token cur = tokens.front();
			if (cur.type != type) {
				throw ": expected " + expectedType + " but was met with " + cur.value;
			}
			else {
				tokens.pop();
				return cur;
			}
		}
	}

	// Consumes the current token if token == identifier
	// Throws error for unexpected tokens
	Token SourceParser::expectsIdentifier() {
		Token cur = tokens.front();
		switch (cur.type) {

		case TOKEN_TYPE::CALL: case TOKEN_TYPE::PRINT:
		case TOKEN_TYPE::READ: case TOKEN_TYPE::WHILE:
		case TOKEN_TYPE::IF: case TOKEN_TYPE::VARIABLE:
			return cur;
		default:
			throw ": expected an identifier but met with " + cur.value;
			break;
		}
	}

	// Parse procedure according to the syntax grammar below
	// 'procedure' 'variable' '{' 'stmtLst' '}'
	// Checks if procedure is empty
	TNode SourceParser::parseProcedure() {
		expects(TOKEN_TYPE::PROCEDURE);

		string procedureName = expects(TOKEN_TYPE::VARIABLE).value;
        currentProc = procedureName;
        checkDuplicateProc();
		procList.insert(currentProc);

		TNode procedureNode = TNode(TNode::NODE_TYPE::procedure, procedureName);

		expects(TOKEN_TYPE::LEFTBRACE);
		TNode child = parseStatementLst(TOKEN_TYPE::PROCEDURE);
		expects(TOKEN_TYPE::RIGHTBRACE);

		procedureNode.addChild(child);

		return procedureNode;
	};

	// Parse procedure according to the syntax grammar below
	// stmt+
	TNode SourceParser::parseStatementLst(TOKEN_TYPE containerType) {
		level.append("    ");
		auto e = enumToString.find(containerType);
		string expectedType = e->second;

		if (tokens.empty()) {
			throw ": expected STATEMENT but was met with EOF";
		}

		if (tokens.front().type == TOKEN_TYPE::RIGHTBRACE) {
			auto t = enumToString.find(containerType);
			string containerType = t->second;
			throw  ": " + containerType + " must contain at least one statement";
		}

		TNode::NODE_TYPE type = tokenToNode.find(containerType)->second;
		TNode statementListNode = TNode(type);

		while (tokens.size() != 0 && tokens.front().type != TOKEN_TYPE::RIGHTBRACE) {
			TNode statementChild = parseStatement();
			statementListNode.addChild(statementChild);
		}

		return statementListNode;
	};

	// Parse statement according to their respective data type
	// Call the respective parser according to the keyword type
	TNode SourceParser::parseStatement() {
		int number = ++statementNo;

		TOKEN_TYPE identifier = expectsIdentifier().type;
		auto e = enumToString.find(identifier);
		string expectedType = e->second;

		switch (identifier) {
		case TOKEN_TYPE::CALL:
			return parseCall();
		case TOKEN_TYPE::PRINT:
			return parsePrint();
		case TOKEN_TYPE::READ:
			return parseRead();
		case TOKEN_TYPE::WHILE:
			return parseWhile();
		case TOKEN_TYPE::IF:
			return parseIf();
		case TOKEN_TYPE::VARIABLE:
			return parseAssignment();
		default:
			throw "expect a statement";
		}
	};

	// Parse call statement according to the syntax grammar below
	// 'call' 'variable' ';'
	TNode SourceParser::parseCall() {
		expects(TOKEN_TYPE::CALL);
		string procedureName = expects(TOKEN_TYPE::VARIABLE).value;
		expects(TOKEN_TYPE::SEMICOLON);

		TNode callNode = TNode(TNode::NODE_TYPE::callStmt, statementNo, procedureName);
        insertElement(currentProc, procedureName);
		return callNode;
	};

	// Parse read statement according to the syntax grammar below
	// 'read' 'variable' ';'
	TNode SourceParser::parseRead() {
		expects(TOKEN_TYPE::READ);
		string variableName = expects(TOKEN_TYPE::VARIABLE).value;
		expects(TOKEN_TYPE::SEMICOLON);

		TNode readNode = TNode(TNode::NODE_TYPE::readStmt, statementNo, variableName);
		readNode.addModifiedVariable(variableName);
		return readNode;
	};

	// Parse print statement according to the syntax grammar below
	// 'print' 'variable' ';'
	TNode SourceParser::parsePrint() {
		expects(TOKEN_TYPE::PRINT);
		string variableName = expects(TOKEN_TYPE::VARIABLE).value;
		expects(TOKEN_TYPE::SEMICOLON);

		TNode printNode = TNode(TNode::NODE_TYPE::printStmt, statementNo, variableName);
		printNode.addUsedVariable(variableName);
		return printNode;
	};

	// Parse assign statement according to the syntax grammar below
	// 'variable' '=' 'ArithmeticExpr' ';'
	TNode SourceParser::parseAssignment() {

		string variableName = expects(TOKEN_TYPE::VARIABLE).value;
		expects(TOKEN_TYPE::ASSIGNMENT); 

		TNode assignmentNode = parseArithmeticExpr();
		assignmentNode.addModifiedVariable(variableName);

		expects(TOKEN_TYPE::SEMICOLON);

		return assignmentNode;
	};

	// Parse while statement according to the syntax grammar below
	// 'while' '(' 'cond_expr' ')' '{' 'statementLst' '}'
	TNode SourceParser::parseWhile() {
		expects(TOKEN_TYPE::WHILE);

		TNode whileNode = parseConditionalExpr(TNode::NODE_TYPE::whileStmt, TOKEN_TYPE::LEFTBRACE);

		expects(TOKEN_TYPE::LEFTBRACE);

		TNode::enterContainer(statementNo);
		TNode statementLstNode = parseStatementLst(TOKEN_TYPE::WHILE);
		TNode::exitContainer();

		whileNode.addChild(statementLstNode);

		expects(TOKEN_TYPE::RIGHTBRACE);

		return whileNode;
	};

	// Parse if statement according to the syntax grammar below
	// 'if' '(' 'cond_expr' ')' 'then' '{' 'statementLst' '}' 'else' '{' 'statementLst' '}'
	TNode SourceParser::parseIf() {
		expects(TOKEN_TYPE::IF);

		TNode ifNode = parseConditionalExpr(TNode::NODE_TYPE::ifStmt, TOKEN_TYPE::THEN);

		expects(TOKEN_TYPE::THEN);
		expects(TOKEN_TYPE::LEFTBRACE);

		TNode::enterContainer(statementNo);
		TNode thenStatementLst = parseStatementLst(TOKEN_TYPE::IF);
		expects(TOKEN_TYPE::RIGHTBRACE);
		expects(TOKEN_TYPE::ELSE);
		expects(TOKEN_TYPE::LEFTBRACE);

		TNode elseStatementLst = parseStatementLst(TOKEN_TYPE::ELSE);
		TNode::exitContainer();

		ifNode.addChild(thenStatementLst);
		ifNode.addChild(elseStatementLst);

		expects(TOKEN_TYPE::RIGHTBRACE);

		return ifNode;
	}

	// Parse arithmetic expression in assign statements
	TNode SourceParser::parseArithmeticExpr() {
		TOKENS expression, expressionCopy;

		// Move tokens until SEMICOLON
		while (!tokens.empty() && tokens.front().type != TOKEN_TYPE::SEMICOLON) {
			expression.emplace(tokens.front());
			expressionCopy.emplace(tokens.front());
			tokens.pop();
		}

		auto variablesConstants = ExpressionParser::ArithmeticExpressionParser::parse(expression);
		string rpnExpression = RPNConverter::convert(expressionCopy);

		TNode expressionNode = TNode(TNode::NODE_TYPE::assignStmt, statementNo, rpnExpression);
		expressionNode.addUsedVariables(variablesConstants.first);
		expressionNode.addConstants(variablesConstants.second);

		return expressionNode;
	};

	// Parse conditional statements in while and if statements
	TNode SourceParser::parseConditionalExpr(TNode::NODE_TYPE nodeType, TOKEN_TYPE delimiter) {
		TOKENS expression;

		// Move tokens until delimiter
		while (!tokens.empty()) {
			auto nextToken = tokens.front();
			if (nextToken.type == delimiter) {
				break;
			}
			expression.emplace(nextToken);
			tokens.pop();
		}

		// Check if conditional statement is wrapped in brackets
		if (expression.front().type != TOKEN_TYPE::LEFTBRACKET
			|| expression.back().type != TOKEN_TYPE::RIGHTBRACKET) {
			throw ": error missing BRACKETS wrapping CONDTIONAL statement";
		}

		auto variablesConstants = ExpressionParser::ConditionalExpressionParser::parse(expression);
		string conditionalExpr = ExpressionParser::ConditionalExpressionParser::getExpression();

		TNode conditionalNode = TNode(nodeType, statementNo, conditionalExpr);
		conditionalNode.addUsedVariables(variablesConstants.first);
		conditionalNode.addConstants(variablesConstants.second);

		return conditionalNode;
	};

	// Driver function to be called to start recursive descent
	TNode SourceParser::parseProgram() {
		TNode root = TNode(TNode::NODE_TYPE::program, fileName);

		while (!tokens.empty()) {
			TNode procedureNode = parseProcedure();
			root.addChild(procedureNode);
		}

		checkInvalidCall();
		return root;
	};

	void SourceParser::checkInvalidCall() {
		for (auto const& procedure : procMatrix) {
			VAR key = procedure.first;
            for (auto const& calls : procMatrix[key]) {
				string call = calls.first;
                if (procList.find(call) == procList.end()) {
                    throw "Invalid procedure call to undeclared procedure";
				}
			}
        }
	}

	void SourceParser::checkDuplicateProc() {
		if (procList.find(currentProc) != procList.end()) {
            throw "Duplicate procedure declartion";
		}
	}

    void SourceParser::insertElement(PROC_NAME p1, PROC_NAME p2) {
		if (p1 == p2) {
			throw "invalid procedure call to itself";
		}
        procMatrix[p1][p2] = 0;
		procMatrix[p1][p1] = 0;
    }

	PROC_MATRIX& SourceParser::getMatrix() { 
		return procMatrix;
	}
}

std::ostream& operator<< (std::ostream& out, Token const& data) {
	auto result = enumToString.find(data.type);
	string s = result->second;
	out << "<Type: " << s << ',';
	out << " Content: " << data.value << " >\n";
	return out;
}
