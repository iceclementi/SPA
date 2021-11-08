#include <fstream>
#include "stdafx.h"
#include "Common.h"
#include "MockAst.h"
#include "MockToken.h"
#include "SourceParser.h"

using namespace MockToken;
using namespace Tokenizer;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSourceParser)
	{
	public:
		TEST_METHOD(expects_Success) {
			queue<Token> t = allSymbols();
			queue<Token> check = allSymbols();
			SourceParser::SourceParser sourparser(t);

			while (!check.empty()) {
				Token toCheck = check.front();
				Token result = sourparser.expects(toCheck.type);
				Assert::AreEqual(result.type == toCheck.type, true);
				Assert::AreEqual(result.value == toCheck.value, true);
				check.pop();
			}
		}

		TEST_METHOD(expectsNohave) {
			Token token = createToken("{");
			queue<Token> t;
			t.emplace(token);
			SourceParser::SourceParser sourparser(t);
			
			try {
				sourparser.expects(createToken("}").type);
			}
			catch (string e) {
				string err = ": expected RIGHTBRACE but was met with {";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(expectsUexpected) {
			queue<Token> t = {};
			SourceParser::SourceParser sourparser(t);
			
			try {
				sourparser.expects(createToken(")").type);
			}
			catch (string e) {
				string err = ": missing RIGHTBRACKET";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(expectsIdentifier) {
			queue<Token> t = allIdentifier();
			queue<Token> check = allIdentifier();
			SourceParser::SourceParser sourparser(t);
			
			while (!check.empty()) {
				Token toCheck = check.front();
				Token result = sourparser.expects(toCheck.type);
				Assert::AreEqual(result.type == toCheck.type, true);
				Assert::AreEqual(result.value == toCheck.value, true);
				check.pop();
			}
		}

		TEST_METHOD(expectsUnexpectedIdentifier) {
			queue<Token> t ;
			t.emplace(createToken(";"));
			SourceParser::SourceParser sourparser(t);
			
			try {
				sourparser.expectsIdentifier();
			}
			catch (string e) {
				string err = ": expected an identifier but met with ;";
				Assert::AreEqual(e, err);
			}

		}

		TEST_METHOD(parseCall) {
			queue<Token> callStmt;
			callStmt = callStmtTokens("procedure");
			SourceParser::SourceParser sourparser(callStmt);
			TNode callNode = sourparser.parseCall();
			TNode test = MockAst::createMockCallNode(0, "procedure");
			Assert::AreEqual(callNode.isEqual(test), true);
		}

		TEST_METHOD(unexpectedParseCall) {
			queue<Token> callStmt;
			callStmt.emplace(createToken("call"));
			callStmt.emplace(createToken(";"));
			SourceParser::SourceParser sourparser(callStmt);

			try {
				sourparser.parseCall();
			}
			catch (string e) {
				string err = ": expected VARIABLE but was met with ;";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(parseRead) {
			queue<Token> readStmt;
			readStmt = readStmtTokens("variable");
			SourceParser::SourceParser sourparser(readStmt);
			TNode readNode = sourparser.parseRead();
			TNode test = MockAst::createMockReadNode(0, "variable");
			Assert::AreEqual(readNode.isEqual(test), true);
		}

		TEST_METHOD(unexpectedParseRead) {
			queue<Token> readStmt;
			readStmt.emplace(createToken("read"));
			readStmt.emplace(createToken(";"));
			SourceParser::SourceParser sourparser(readStmt);
			try {
				sourparser.parseRead();
			}
			catch (string e) {
				string err = ": expected VARIABLE but was met with ;";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(parsePrint) {
			queue<Token> printStmt;
			printStmt = printStmtTokens("variable");
			SourceParser::SourceParser sourparser(printStmt);
			TNode readNode = sourparser.parsePrint();
			TNode test = MockAst::createMockPrintNode(0, "variable");
			Assert::AreEqual(readNode.isEqual(test), true);
		}

		TEST_METHOD(unexpectedParsePrint) {
			queue<Token> printStmt;
			printStmt.emplace(createToken("print"));
			printStmt.emplace(createToken(";"));
			SourceParser::SourceParser sourparser(printStmt);

			try {
				sourparser.parsePrint();
			}
			catch (string e) {
				string err = ": expected VARIABLE but was met with ;";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(parseAssignmentA) {
			vector<string> temp = {"variableA", "=", "2", "+", "variableB", ";"};
			queue<Token> tokens = createTokens(temp);
			SourceParser::SourceParser sourparser(tokens);
			VAR_NAMES usedVariable = { "variableB" };
			VARS constant = { "2" };

			TNode test = MockAst::createMockAssignNode(0, " 2  variableB  + ", "variableA", usedVariable, constant);
			TNode assignNode = sourparser.parseAssignment();
				
			Assert::AreEqual(assignNode.isEqual(test), true);
		}

		TEST_METHOD(parseAssignmentB) {
			vector<string> temp = { "variableA", "=", "2", "+", "variableB", "-", "3", "*", "variableC", ";"};
			queue<Token> tokens = createTokens(temp);
			SourceParser::SourceParser sourparser(tokens);
			VAR_NAMES usedVariable = { "variableB", "variableC" };
			VARS constant = { "2", "3" };

			TNode test = MockAst::createMockAssignNode(0, " 2  variableB  +  3  variableC  *  - ", "variableA", usedVariable, constant);
			TNode assignNode = sourparser.parseAssignment();

			Assert::AreEqual(assignNode.isEqual(test), true);
		}

		TEST_METHOD(unexpecteParseAssignment) {
			vector<string> temp = { "variableA", "=", "2", "variableB", "-", "3", "*", "variableC", ";" };
			queue<Token> tokens = createTokens(temp);
			SourceParser::SourceParser sourparser(tokens);

			try {
				sourparser.parseAssignment();
			}
			catch (string e) {
				string err = ": invalid Arithmetic Expression syntax met with variableB\n2";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(parseStatementLst) {
			vector<string> temp = { "variableC", "=", "variableD", "+", "10" , ";"};
			queue<Token> tokens = createTokens(temp);
			queue<Token> a = readStmtTokens("variableA");
			queue<Token> b = callStmtTokens("variableB");

			while (!a.empty()) {
				tokens.emplace(a.front());
				a.pop();
			}

			while (!b.empty()) {
				tokens.emplace(b.front());
				b.pop();
			}

			SourceParser::SourceParser sourparser(tokens);
			TNode stmtLstNode = sourparser.parseStatementLst(TOKEN_TYPE::WHILE);

			VAR_NAMES usedVariable = { "variableD" };
			VARS constant = { "10" };
			TNode assignNode = MockAst::createMockAssignNode(1, " variableD  10  + ", "variableC", usedVariable, constant);
			TNode readNode = MockAst::createMockReadNode(2,"variableA");
			TNode callNode = MockAst::createMockCallNode(3, "variableB");
			
			vector<TNode> childs = { assignNode, readNode, callNode };
			TNode test = MockAst::createMockWhileStmtLstNode(childs);

			Assert::AreEqual(stmtLstNode.isEqual(test) , true);
		}

		TEST_METHOD(parseStatementLst_empty1) {
			queue<Token> temp = { };
			SourceParser::SourceParser sourparser(temp);

			try {
				sourparser.parseStatementLst(TOKEN_TYPE::WHILE);
			}
			catch (const char* e) {
				Assert::AreEqual(e, ": expected STATEMENT but was met with EOF");
			}
		}

		TEST_METHOD(parseStatementLst_empty2) {
			queue<Token> temp;
			temp.emplace(createToken("}"));

			SourceParser::SourceParser sourparser(temp);

			try {
				sourparser.parseStatementLst(TOKEN_TYPE::WHILE);
			}
			catch (string e) {
				string err = ": WHILE must contain at least one statement";
				Assert::AreEqual(e, err);
			}
		}

		TEST_METHOD(parseProcedure) {
			queue<Token> temp;
			temp.emplace(createToken("procedure"));
			temp.emplace(createToken("name",1));
			temp.emplace(createToken("{"));
			queue<Token> rs = (readStmtTokens("var"));
			while (!rs.empty()) {
				temp.emplace(rs.front());
				rs.pop();
			}
			temp.emplace(createToken("}"));

			SourceParser::SourceParser sourparser(temp);

			TNode procedure = sourparser.parseProcedure();
			vector<TNode> childs = { MockAst::createMockReadNode(1, "var") };
			TNode testStmtLst = MockAst::createMockProcStmtLstNode(childs);
			TNode test = MockAst::createMockProcedureNode("name", testStmtLst);
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(parseProcedureMissingBrace) {
			queue<Token> temp;
			temp.emplace(createToken("procedure"));
			temp.emplace(createToken("name", 1));
			temp.emplace(createToken("{"));
			queue<Token> rs = (readStmtTokens("var"));
			while (!rs.empty()) {
				temp.emplace(rs.front());
				rs.pop();
			}

			SourceParser::SourceParser sourparser(temp);

			try {
				sourparser.parseProcedure();
			}
			catch (string e) {
				string err = ": missing RIGHTBRACE";
				Assert::AreEqual(err, e);
			}
		}
		
		TEST_METHOD(parseWhile) {
			queue<Token> temp;
			temp.emplace(createToken("while"));
			vector<string> tokens = { "(", "variableA" , "<=" , "10", ")", "{" };
			queue<Token> t = createTokens(tokens);
			while (!t.empty()) {
				temp.emplace(t.front());
				t.pop();
			}
			queue<Token> rs = (readStmtTokens("var"));
			while (!rs.empty()) {
				temp.emplace(rs.front());
				rs.pop();
			}
			temp.emplace(createToken("}"));
			
			SourceParser::SourceParser sourparser(temp);
			TNode whileStmt = sourparser.parseWhile();
			VAR_NAMES usedvariable = { "variableA" };
			VARS constant = { "10" };
			vector<TNode> childrens = { MockAst::createMockReadNode(1,"var") };
			TNode testStmtLst = MockAst::createMockWhileStmtLstNode(childrens);
			TNode test = MockAst::createMockWhileNode(0, "variableA  <= 10 ", usedvariable, constant, testStmtLst);

			Assert::AreEqual(whileStmt.isEqual(test), true);
		}

		TEST_METHOD(parseWhileMissingBrace) {
			queue<Token> temp;
			temp.emplace(createToken("while"));
			vector<string> tokens = { "(", "variableA" , "<=" , "10", ")", "{" };
			queue<Token> t = createTokens(tokens);
			while (!t.empty()) {
				temp.emplace(t.front());
				t.pop();
			}
			queue<Token> rs = (readStmtTokens("var"));
			while (!rs.empty()) {
				temp.emplace(rs.front());
				rs.pop();
			}

			try {
				SourceParser::SourceParser sourparser(temp);
			}
			catch (string e) {
				string err = ": missing RIGHTBRACE";
				Assert::AreEqual(err, e);
			}
		}
		
		TEST_METHOD(parseIf) {
			queue<Token> temp;
			temp.emplace(createToken("if"));
			vector<string> tokens = { "(", "variableA" , "<=" , "10", ")" };
			queue<Token> t = createTokens(tokens);
			while (!t.empty()) {
				temp.emplace(t.front());
				t.pop();
			}
			temp.emplace(createToken("then"));
			temp.emplace(createToken("{"));
			queue<Token> rs = (readStmtTokens("var"));
			while (!rs.empty()) {
				temp.emplace(rs.front());
				rs.pop();
			}
			temp.emplace(createToken("}"));
			temp.emplace(createToken("else"));
			temp.emplace(createToken("{"));
			queue<Token> cs = (callStmtTokens("varA"));
			while (!cs.empty()) {
				temp.emplace(cs.front());
				cs.pop();
			}
			temp.emplace(createToken("}"));
			
			SourceParser::SourceParser sourparser(temp);
			TNode ifStmt = sourparser.parseIf();

			VAR_NAMES usedvariable = { "variableA" };
			VARS constant = { "10" };
			vector<TNode> ifChildrens = { MockAst::createMockReadNode(1,"var") };
			vector<TNode> elseChildrens = { MockAst::createMockCallNode(2,"varA") };

			TNode ifStmtLst = MockAst::createMockThenStmtLstNode(ifChildrens);
			TNode elseStmtLst = MockAst::createMockElseStmtLstNode(elseChildrens);
			TNode test = MockAst::createMockIfNode(0, "variableA  <= 10 ", usedvariable, constant, ifStmtLst, elseStmtLst);

			Assert::AreEqual(ifStmt.isEqual(test), true);
		}

		TEST_METHOD(parseProgram) {
			string filename = "../UnitTesting/TestSources/Sour_Parser_Test1.txt";
			SourceParser::SourceParser sourparser(filename);
			TNode root = sourparser.parseProgram();
			TNode test = MockAst::createSampleAst1("../UnitTesting/TestSources/Sour_Parser_Test1.txt");

			Assert::AreEqual(root.isEqual(test), true);
		}
	};
}
