#pragma once

#include "Common.h"
#include "Tokenizer.h"
#include "TNode.h"
#include "ExpressionParser.h"
#include "DesignExtractor.h"

using namespace Tokenizer;

typedef queue<Token> TOKENS;
typedef unordered_map<string, unordered_map<string, int>> PROC_MATRIX;

namespace SourceParser {
	class SourceParser {
	public:
		string fileName;
		string level = "";
        PROC_MATRIX procMatrix;
        PROC_NAMES procList;
		TOKENS tokens;
        PROC_NAME currentProc = "";
		int statementNo = 0;
		string nesting = "";

		SourceParser(string fileName);
        SourceParser(queue<Token> tokens);

        TNode parseProgram();
        PROC_MATRIX& getMatrix();   
		TOKENS initTokens();
		Token expects(TOKEN_TYPE type);
		Token expectsIdentifier();
		TNode parseProcedure();
		TNode parseStatementLst(TOKEN_TYPE type);
		TNode parseStatement();
		TNode parseCall();
		TNode parseRead();
		TNode parsePrint();
		TNode parseIf();
		TNode parseWhile();
		TNode parseAssignment();
		TNode parseArithmeticExpr();
		TNode parseConditionalExpr(TNode::NODE_TYPE nodeType, TOKEN_TYPE delimiter);
        void insertElement(PROC_NAME p1, PROC_NAME p2);
        void checkInvalidCall();
		void checkDuplicateProc();
	};
}
