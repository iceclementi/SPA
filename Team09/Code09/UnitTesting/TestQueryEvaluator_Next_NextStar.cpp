#include "stdafx.h"
#include "TestQueryEvaluator_Next_NextStar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Next_NextStar)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_N) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_N) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_N) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_N) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_N) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_N) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : callStmtNumsStub_N) {
				pkb.addStmt(DESIGN_ENTITY::CALL, value);
			}
			for (auto& value : whileStmtNumsStub_N) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_N) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : nTableStubN) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::NEXT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : nStarTableStubN) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::NEXT_STAR, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateNext
		TEST_METHOD(evaluateNext_INT_INT_Pass)
		{
			//Next(int, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "8", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_INT_Fail_1)
		{
			//Next(int, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_INT_Fail_2)
		{
			//Next(int, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "6", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_Underscore_Pass)
		{
			//Next(int, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_Underscore_Fail)
		{
			//Next(int, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_Syn_Pass_1)
		{
			//Next(int, assignSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_Syn_Pass_2)
		{
			//Next(int, proglineSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_Syn_Pass_3)
		{
			//Next(int, callStmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "c", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_INT_Syn_Fail)
		{
			//Next(int, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "9", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Underscore_INT_Pass)
		{
			//Next(_, 4)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Underscore_INT_Fail)
		{
			//Next(_, 2)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);;

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Underscore_Underscore_Pass)
		{
			//Next(_,_)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Underscore_Syn_Pass_1)
		{
			//Next(_, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Underscore_Syn_Pass_2)
		{
			//Next(_, readSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Underscore_Syn_Pass_3)
		{
			//Next(_, printSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Integer_Pass_1)
		{
			//Next(_, WhileSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Integer_Pass_2)
		{
			//Next(_, IfSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "15", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Underscore_Pass_1)
		{
			//Next(Progline, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Underscore_Pass_2)
		{
			//Next(stmtSyn, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Stmt_Syn_Stmt_Syn_Pass)
		{
			//Next(stmtSyn, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Stmt_Syn_While_Syn_Pass)
		{
			//Next(stmtSyn, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Syn_Pass_2)
		{
			//Next(progline, progline)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n1", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Syn_Pass_3)
		{
			//Next(assignSyn, callSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "c", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Syn_Pass_4)
		{
			//Next(readSyn, assignSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Syn_Pass)
		{
			//Next(WhileSyn, WhileSyn) //line 4 while stmt
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w1", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(4);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNext_Syn_Syn_Fail)
		{
			//Next(ifSyn, ifSyn) 
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs1", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(4);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

#pragma region evaluateNextStar

		TEST_METHOD(evaluateNextStar_INT_INT_Pass)
		{
			//Next*(int, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "7", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_INT_Fail_1)
		{
			//Next*(int, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "3", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_INT_Fail_2)
		{
			//Next*(int, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "9", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "8", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_Underscore_Pass)
		{
			//Next*(int, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_Syn_Pass_1)
		{
			//Next*(int, assignSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_Syn_Pass_2)
		{
			//Next*(int, proglineSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_Syn_Pass_3)
		{
			//Next*(int, readStmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_INT_Syn_Fail)
		{
			//Next*(int, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "9", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Underscore_INT_Pass)
		{
			//Next*(_, 4)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Underscore_INT_Fail)
		{
			//Next*(_, 2)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Underscore_Underscore_Pass)
		{
			//Next*(_,_)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Underscore_Syn_Pass_1)
		{
			//Next*(_, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(4);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Underscore_Syn_Pass_2)
		{
			//Next*(_, readSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Underscore_Syn_Pass_3)
		{
			//Next*(_, printSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 6, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(6);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Integer_Pass_1)
		{
			//Next*(WhileSyn, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Integer_Pass_2)
		{
			//Next*(IfSyn, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "15", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Underscore_Pass_1)
		{
			//Next*(Progline, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Underscore_Pass_2)
		{
			//Next*(stmtSyn, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Syn_Pass_1)
		{
			//Next*(stmtSyn, stmtSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Syn_Pass_2)
		{
			//Next*(progline, progline)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n1", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Syn_Pass_3)
		{
			//Next*(assignSyn, callSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "c", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Syn_Pass_4)
		{
			//Next*(readSyn, assignSyn)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Syn_Pass)
		{
			//Next*(WhileSyn, WhileSyn) //line 4 while stmt
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w1", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateNextStar_Syn_Syn_Fail)
		{
			//Next*(ifSyn, ifSyn) //line 4 while stmt
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs1", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::NEXT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(4);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}

