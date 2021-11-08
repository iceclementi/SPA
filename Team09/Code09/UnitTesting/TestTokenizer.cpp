#include "stdafx.h"
#include "CppUnitTest.h"
#include "Tokenizer.cpp"
#include <iostream>
#include <fstream>
#include "MockToken.h"
#define TEST_CASE_DIRECTORY GetDirectoryName(__FILE__)

string GetDirectoryName(string path) {
	const size_t last_slash_idx = path.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		return path.substr(0, last_slash_idx + 1);
	}
	return "";
}

using namespace std;
using namespace Tokenizer;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestTokennizer)
	{

	public:

		TEST_METHOD(getType_String_Keyword)
		{
			string proc = "procedure";
			TOKEN_TYPE typeProc = TOKEN_TYPE::PROCEDURE;
			Assert::AreEqual(getType(proc) == typeProc, true);

			string read = "read";
			TOKEN_TYPE typeRead = TOKEN_TYPE::READ;
			Assert::AreEqual(getType(read) == typeRead, true);

			string print = "print";
			TOKEN_TYPE typePrint = TOKEN_TYPE::PRINT;
			Assert::AreEqual(getType(print) == typePrint, true);

			string call = "call";
			TOKEN_TYPE typeCall = TOKEN_TYPE::CALL;
			Assert::AreEqual(getType(call) == typeCall, true);

			string wild = "while";
			TOKEN_TYPE typeWhile = TOKEN_TYPE::WHILE;
			Assert::AreEqual(getType(wild) == typeWhile, true);

			string eve = "if";
			TOKEN_TYPE typeIf = TOKEN_TYPE::IF;
			Assert::AreEqual(getType(eve) == typeIf, true);

			string then = "then";
			TOKEN_TYPE typeThen = TOKEN_TYPE::THEN;
			Assert::AreEqual(getType(then) == typeThen, true);

			string elsa = "else";
			TOKEN_TYPE typeElse = TOKEN_TYPE::ELSE;
			Assert::AreEqual(getType(elsa) == typeElse, true);
		}

		TEST_METHOD(getType_String_Variable)
		{
			TOKEN_TYPE typeVariable = TOKEN_TYPE::VARIABLE;
			
			string test1 = "Procedure";
			Assert::AreEqual(getType(test1) == typeVariable, true);

			string test2 = "Read";
			Assert::AreEqual(getType(test2)== typeVariable, true);

			string test3 = "Call";
			Assert::AreEqual(getType(test3) == typeVariable, true);

			string test4 = "While";
			Assert::AreEqual(getType(test4) == typeVariable, true);

			string test5 = "IF";
			Assert::AreEqual(getType(test5) == typeVariable, true);

			string test6 = "thEn";
			Assert::AreEqual(getType(test6) == typeVariable, true);

			string test7 = "elsE";
			Assert::AreEqual(getType(test7) == typeVariable, true);

			string test8 = "testing123";
			Assert::AreEqual(getType(test8) == typeVariable, true);
		}

		TEST_METHOD(getType_Char)
		{
			char test1 = '+';
			TOKEN_TYPE plus = TOKEN_TYPE::OPERATOR_1;
			Assert::AreEqual(getType(test1) == plus, true);
			char test2 = '-';
			TOKEN_TYPE minus = TOKEN_TYPE::OPERATOR_1;
			Assert::AreEqual(getType(test2) == minus, true);
			char test3 = '*';
			TOKEN_TYPE mul = TOKEN_TYPE::OPERATOR_2;
			Assert::AreEqual(getType(test3) == mul, true);
			char test4 = '/';
			TOKEN_TYPE div = TOKEN_TYPE::OPERATOR_2;
			Assert::AreEqual(getType(test4) == div, true);
			char test5 = '%';
			TOKEN_TYPE mod = TOKEN_TYPE::OPERATOR_2;
			Assert::AreEqual(getType(test5) == mod, true);
			char test6 = '(';
			TOKEN_TYPE lBrac = TOKEN_TYPE::LEFTBRACKET;
			Assert::AreEqual(getType(test6) == lBrac, true);
			char test7 = ')';
			TOKEN_TYPE rBrac = TOKEN_TYPE::RIGHTBRACKET;
			Assert::AreEqual(getType(test7) == rBrac, true);
			char test8 = '{';
			TOKEN_TYPE lBra = TOKEN_TYPE::LEFTBRACE;
			Assert::AreEqual(getType(test8) == lBra, true);
			char test9 = '}';
			TOKEN_TYPE rBra = TOKEN_TYPE::RIGHTBRACE;
			Assert::AreEqual(getType(test9) == rBra, true);
			char test10 = ';';
			TOKEN_TYPE semicolon = TOKEN_TYPE::SEMICOLON;
			Assert::AreEqual(getType(test10) == semicolon, true);
			char test11 = '=';
			TOKEN_TYPE assign = TOKEN_TYPE::ASSIGNMENT;
			Assert::AreEqual(getType(test11) == assign , true);

			
			
			vector<char> invalidSymbols = {'@' ,'#' ,'$' ,'^','`' ,'~' ,'_' ,'[' ,']', 
										   '?' ,'|' , '.', ',' ,'\'' ,'\"', '\\'};
			
			for (char invalid : invalidSymbols) {
				try {
					getType(invalid);
				}
				catch (const char*& e) {
					// Catches the assertion exception, and the test passes
					Assert::AreEqual(e, "unexpected symbol");
				}
			}
			
			
			

		}

		//DIGIT+ first digit cannot be 0
		TEST_METHOD(isNumber) {
			string test1 = "1234";
			Assert::AreEqual(Tokenizer::isNumber(test1), true);

			string test2 = "123456789012112312432542354364578689709";
			Assert::AreEqual(Tokenizer::isNumber(test2), true);

			string test3 = "98766554";
			Assert::AreEqual(Tokenizer::isNumber(test3), true);

			string test4 = "1";
			Assert::AreEqual(Tokenizer::isNumber(test4), true);

			string test5 = "0";
			Assert::AreEqual(Tokenizer::isNumber(test5), true);

			string test7 = "01";
			Assert::AreEqual(Tokenizer::isNumber(test7), false);

			string test8 = "1asd";
			Assert::AreEqual(Tokenizer::isNumber(test8), false);

			string test9 = "@98754";
			Assert::AreEqual(Tokenizer::isNumber(test9), false);

			string test10 = "987@54";
			Assert::AreEqual(Tokenizer::isNumber(test10), false);

		}

		// LETTER (LETTER | DIGIT)*
		TEST_METHOD(isName) {
			string test1 = "t";
			Assert::AreEqual(Tokenizer::isName(test1), true);

			string test2 = "test";
			Assert::AreEqual(Tokenizer::isName(test2), true);

			string test3 = "test1";
			Assert::AreEqual(Tokenizer::isName(test3), true);

			string test4 = "t1est1asd";
			Assert::AreEqual(Tokenizer::isName(test4), true);

			string test5 = "1test";
			Assert::AreEqual(Tokenizer::isName(test5), false);

			string test6 = "@test";
			Assert::AreEqual(Tokenizer::isName(test6), false);

			string test7 = "te@st";
			Assert::AreEqual(Tokenizer::isName(test7), false);
		}

		TEST_METHOD(determineIdentifier) {
			string name = "name";
			Token t1 = Tokenizer::determineIdentifier(name, true);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::VARIABLE == t1.type, true);
			Assert::AreEqual("name" == t1.value, true);

			string procedure = "procedure";
			Token t2 = Tokenizer::determineIdentifier(procedure, false);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::PROCEDURE == t2.type, true);
			Assert::AreEqual("procedure" == t2.value, true);
			isVariable = false;

			string constant = "12345";
			Token t3 = Tokenizer::determineIdentifier(constant, false);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONSTANT == t3.type, true);
			Assert::AreEqual("12345" == t3.value, true);

			string invalid = "1@asd";
			try {
				Tokenizer::determineIdentifier(invalid, false);
			}
			catch (const char* e) {
				Assert::AreEqual(e == "wrong syntax for variable", true);
			}
		}

		TEST_METHOD(getDelimiterToken) {
			char op1 = '+';
			Token t1 = Tokenizer::getDelimiterToken(op1);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::OPERATOR_1 == t1.type, true);
			Assert::AreEqual("+" == t1.value, true);
			
			char op2 = '/';
			Token t2= Tokenizer::getDelimiterToken(op2);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::OPERATOR_2 == t2.type, true);
			Assert::AreEqual("/" == t2.value, true);

			char leftBra = '{';
			Token t4 = Tokenizer::getDelimiterToken(leftBra);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::LEFTBRACE == t4.type, true);
			Assert::AreEqual("{" == t4.value, true);

			char rightBra = ')';
			Token t5 = Tokenizer::getDelimiterToken(rightBra);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::RIGHTBRACKET == t5.type, true);
			Assert::AreEqual(")" == t5.value, true);

			char assign = '=';
			Token t6 = Tokenizer::getDelimiterToken(assign);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::ASSIGNMENT == t6.type, true);
			Assert::AreEqual("=" == t6.value, true);
		}

		TEST_METHOD(getConditionToken) {
			string gt = ">";
			Token t1 = Tokenizer::getConditionalToken(gt);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONDITIONAL == t1.type, true);
			Assert::AreEqual(">" == t1.value, true);

			string gte = ">=";
			Token t2 = Tokenizer::getConditionalToken(gte);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONDITIONAL == t2.type, true);
			Assert::AreEqual(">=" == t2.value, true);

			string lt = "<";
			Token t3 = Tokenizer::getConditionalToken(lt);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONDITIONAL == t3.type, true);
			Assert::AreEqual("<" == t3.value, true);
			
			string lte = "<=";
			Token t4 = Tokenizer::getConditionalToken(lte);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONDITIONAL == t4.type, true);
			Assert::AreEqual("<=" == t4.value, true);

			string eq = "==";
			Token t5 = Tokenizer::getConditionalToken(eq);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONDITIONAL == t5.type, true);
			Assert::AreEqual("==" == t5.value, true);

			string neq = "!=";
			Token t6 = Tokenizer::getConditionalToken(neq);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::CONDITIONAL == t6.type, true);
			Assert::AreEqual("!=" == t6.value, true);
		}

		TEST_METHOD(getLogicalToken) {
			string and = "&&";
			Token t1 = Tokenizer::getLogicalToken(and);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::LOGICAL == t1.type, true);
			Assert::AreEqual("&&" == t1.value, true);

			string or = "||";
			Token t2 = Tokenizer::getLogicalToken(or);
			Assert::AreEqual(Tokenizer::TOKEN_TYPE::LOGICAL == t2.type, true);
			Assert::AreEqual("||" == t2.value, true);
			
		}

		TEST_METHOD(tokenize) {
			string test1 = std::string(TEST_CASE_DIRECTORY) + "TOKENIZE_TEST1.txt";
			std::ifstream t1;
			t1.open(test1);
			std::istream& fileStream1 = t1;

			//abit cheating but test if tokenizer can correctly identify all the symbols at one shot and
			//correctly identify keywords as variables in both conditional and assignment
			queue<Token> check1 = MockToken::create_test1();
			queue<Token> check2 = Tokenizer::tokenize(fileStream1);

			Assert::AreEqual(check1.size() == check2.size(), true);

			for (unsigned int i = 0; i < check1.size(); i++) {
				Assert::AreEqual(check1.front().type == check2.front().type, true);
				Assert::AreEqual(check1.front().value == check2.front().value, true);
				check1.pop();
				check2.pop();
			}

			
			string test2 = std::string(TEST_CASE_DIRECTORY) + "TOKENIZE_TEST2.txt";
			std::ifstream t2;
			t2.open(test2);
			std::istream& fileStream2 = t2;

			//test for whitespace
			queue<Token> check3 = MockToken::create_test1();
			queue<Token> check4 = Tokenizer::tokenize(fileStream2);

			Assert::AreEqual(check3.size() == check4.size(), true);
			for (unsigned int i = 0; i < check3.size(); i++) {
				Assert::AreEqual(check3.front().type == check4.front().type, true);
				Assert::AreEqual(check3.front().value == check4.front().value, true);
				check3.pop();
				check4.pop();
			}
			
		}

		TEST_METHOD(tokenize_wrong_logical) {
			string test1 = std::string(TEST_CASE_DIRECTORY) + "TOKENIZE_TEST3.txt";
			std::ifstream t1;
			t1.open(test1);
			std::istream& fileStream1 = t1;

			try {
				Tokenizer::tokenize(fileStream1);
			}
			catch (const char*& e) {
				// Catches the assertion exception, and the test passes
				Assert::AreEqual(e, "wrong syntax for logical");
			}
		}

		TEST_METHOD(tokenize_wrong_syntax) {
			string test1 = std::string(TEST_CASE_DIRECTORY) + "TOKENIZE_TEST4.txt";
			std::ifstream t1;
			t1.open(test1);
			std::istream& fileStream1 = t1;

			try {
				Tokenizer::tokenize(fileStream1);
			}
			catch (string e) {
				// Catches the assertion exception, and the test passes
				string error = "\nunknown character @ in program";
				Assert::AreEqual(e, error);
			}
		}

		TEST_METHOD(tokenize_random_symbol) {
			string test1 = std::string(TEST_CASE_DIRECTORY) + "TOKENIZE_TEST5.txt";
			std::ifstream t1;
			t1.open(test1);
			std::istream& fileStream1 = t1;

			try {
				Tokenizer::tokenize(fileStream1);
			}
			catch (string e) {
				// Catches the assertion exception, and the test passes
				string error = "\nunknown character $ in program";
				Assert::AreEqual(e, error);
			}
		}


	};
}