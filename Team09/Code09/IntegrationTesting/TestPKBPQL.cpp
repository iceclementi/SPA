#include "stdafx.h"
#include "TestPKBPQL.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestPKBPQL
{
	TEST_CLASS(TestPKBPQL)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			// Add the rest of the stubs
			for (auto& value : procTableStub) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : fTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::FOLLOWS, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : fStarTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::FOLLOWS_STAR, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : pTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::PARENT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : pStarTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::PARENT_STAR, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : usesProcTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::USES_PROC, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : usesStmtTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::USES_STMT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : modifiesProcTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::MODIFIES_PROC, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : modifiesStmtTableStub) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::MODIFIES_STMT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : assignPatternStub) {
				pkb.addPattern(valueGroup.first, valueGroup.second.first, valueGroup.second.second);
			}
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateQuery

		TEST_METHOD(evaluateQuery)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			Select pd such that Follows(1, 2) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "um", "main" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryFollows)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			Select v such that Follows(1, 2) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "a",  "b",  "c",  "d",  "e",  "f",  "g",  "var",  "x" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryFollowsStar)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			Select w1 such that Follows*(w1, _) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "5", "8" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryParent)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			Select s such that Parent(s, _) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "5",  "8",  "12",  "19",  "27",  "32",  "39" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryParentStar1)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			Select r such that Parent*(5, r) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "6",  "9",  "13" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQuery_ParentStar2)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			 Select a such that Parent* (w, a)  )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "11",  "14",  "16",  "28" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQuery_ParentStar3)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
				Select w2 such that Parent* (w1, w2))";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "8" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryUses_assign)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
				Select a such that Uses(a, "a") )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "33",  "34",  "36",  "38",  "40",  "43",  "45" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryUses_print)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
				Select p such that Uses(p, v) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "3",  "7",  "10",  "15",  "17",  "22",  "26",  "31",  "46",  "47",  "48" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryUses_stmt)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			 Select s such that Uses(s, "b") )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "32",  "33",  "36",  "39",  "40",  "43",  "47" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryUses_procedure)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
				Select pd such that Uses(pd, v) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "um",  "main" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryModifies)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			 Select s such that Modifies(s, "c") )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "32",  "33",  "36",  "39",  "40",  "43" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryAssignPattern1)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			  Select a pattern a ("x", _) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "4",  "11",  "25",  "28" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryAssignPattern2)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			  Select a pattern a (v, _"1"_) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "1",  "14",  "20",  "29",  "41",  "42",  "45" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQueryAssignPattern3)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
				Select a pattern a (v, _"0"_) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "4",  "11",  "25",  "28" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQuery_SuchThatFollows_AssignPattern)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			 Select a such that Follows (w, a) pattern a ("x", _) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "11" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQuery_AssignPattern_SuchThatFollows_)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			 Select a pattern a ("x", _) such that Follows (w, a)  )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "11" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQuery_SuchThatUses_AssignPattern)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
				Select a such that Uses (a, v) pattern a (v, _) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "43",  "45" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateQuery_SuchThatParentStar_AssignPattern)
		{
			string queryString = R"(variable v, v1, v2; procedure pd, pd1, pd2; call cl; read r, r1, r2; 
				print p, p1, p2; assign a, a1, a2; while w, w1, w2; 
				if ifs, ifs1, ifs2; stmt s, s1, s2; constant c, c1, c2;
			Select a such that Parent* (w, a) pattern a (v, _) )";
			QueryObject queryObject = QueryPreprocessor(queryString).parseQuery();
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);
			
			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "11",  "14",  "16",  "28" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}
	
#pragma endregion

	};
}

