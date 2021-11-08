#include "stdafx.h"
#include "TestQueryEvaluator_Follows_FollowsStar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Follows_FollowsStar)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_F) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_F) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_F) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_F) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_F) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_F) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_F) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_F) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : fTableStub_F) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::FOLLOWS, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : fStarTableStub_F) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::FOLLOWS_STAR, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateFollows

		TEST_METHOD(evaluateFollows_int_int_Pass)
		{
			//Follows(int,int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_SynStmt_Pass)
		{
			//Follows(int,stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_SynRead_Pass)
		{
			//Follows(int,read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_SynPrint_Pass)
		{
			//Follows(int,print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_SynAssign_Pass)
		{
			//Follows(int,assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "3", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_SynWhile_Pass)
		{
			//Follows(int,while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_SynIf_Pass)
		{
			//Follows(int,if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "11", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "if", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_int_Underscore_Pass)
		{
			//Follows(int,_)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynStmt_int_Pass)
		{
			//Follows(stmt, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "7", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynRead_int_Pass)
		{
			//Follows(read, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "7", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynPrint_int_Pass)
		{
			//Follows(print, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "8", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(4);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynAssign_int_Pass)
		{
			//Follows(assign, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynWhile_int_Pass)
		{
			//Follows(while, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "11", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynIf_int_Pass)
		{
			//Follows(if, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "i", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "27", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynStmt_SynStmt_Pass)
		{
			//Follows(stmt, stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s2", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynStmt_SynRead_Pass)
		{
			//Follows(stmt, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynStmt_SynPrint_Pass)
		{
			//Follows(stmt, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynAssign_SynPrint_Pass)
		{
			//Follows(assign, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynRead_SynIf_Pass)
		{
			//Follows(read, if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynIf_SynWhile_Pass)
		{
			//Follows(if, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynStmt_Underscore_Pass)
		{
			//Follows(stmt, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynRead_Underscore_Pass)
		{
			//Follows(read, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynAssign_Underscore_Pass)
		{
			//Follows(assign, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynWhile_Underscore_Pass)
		{
			//Follows(while, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_SynIf_Underscore_Pass)
		{
			//Follows(if, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "fs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_Underscore_int_Pass)
		{
			//Follows(_,int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_Underscore_SynStmt_Pass)
		{
			//Follows(_,stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_Underscore_SynSPrint_Pass)
		{
			//Follows(_,print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollows_Underscore_Underscore_Pass)
		{
			//Follows(_,_)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

#pragma region evaluateFollowsStar

		TEST_METHOD(evaluateFollowsStar_int_int_Pass)
		{
			//FollowsStar(int,int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "3", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_SynStmt_Pass)
		{
			//FollowsStar(int,stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_SynRead_Pass)
		{
			//FollowsStar(int,read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_SynPrint_Pass)
		{
			//FollowsStar(int,print)
			//select p such that Follows*(1,p)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_SynAssign_Pass)
		{
			//FollowsStar(int,assign)
			//select a such that Follows*(1, a)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_SynWhile_Pass)
		{
			//FollowsStar(int,while)
			//select w such that Follows*(1, w)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_SynIf_Pass)
		{
			//FollowsStar(int,if)
			//select if such that Follows*(1,if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_int_UnderScore_Pass)
		{
			//FollowsStar(int,_)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynStmt_int_Pass)
		{
			//FollowsStar(stmt, int)
			//select s such that Follows*(s, 8)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "8", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynRead_int_Pass)
		{
			//FollowsStar(read, int)
			//select r such that Follows*(r, 19)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "19", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(4);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynPrint_int_Pass)
		{
			//FollowsStar(print, int)
			//select p such that Follows*(p, 19)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "19", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynAssign_int_Pass)
		{
			//FollowsStar(assign, int)
			//select a such that Follows*(a, 5)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynWhile_int_Pass)
		{
			//FollowsStar(while, int)
			//select w such that Follows*(w, 19)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "19", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynIf_int_Pass)
		{
			//FollowsStar(if, int)
			//select if such that Follows*(if, 27)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 6, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "27", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(6);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynStmt_SynStmt_Pass)
		{
			//FollowsStar(stmt, stmt)
			//select s1 such that Follows*(s1, s2)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s2", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynStmt_SynWhile_Pass)
		{
			//FollowsStar(stmt, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynStmt_SynIf_Pass)
		{
			//FollowsStar(stmt, if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynStmt_SynRead_Pass)
		{
			//FollowsStar(stmt, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynWhile_SynRead_Pass)
		{
			//FollowsStar(while, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynWhile_SynPrint_Pass)
		{
			//FollowsStar(while, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynWhile_SynAssign_Pass)
		{
			//FollowsStar(while, assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynWhile_SynWhile_Pass)
		{
			//FollowsStar(while, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w1", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynAssign_SynRead_Pass)
		{
			//FollowsStar(assign, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynAssign_SynAssign_Pass)
		{
			//FollowsStar(assign, assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(5);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynStmt_Underscore_Pass)
		{
			//FollowsStar(stmt, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_SynPrint_Underscore_Pass)
		{
			//FollowsStar(print, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_Underscore_int_Pass)
		{
			//Follows*(_,int)
			//select r such that Follows*(_, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_Underscore_SynStmt_Pass)
		{
			//FollowsStar(_, stmt)
			//select s such that Follows*(_, stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_Underscore_SynAssign_Pass)
		{
			//FollowsStar(_, assign)
			//select a such that Follows*(_, a)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_Underscore_SynWhile_Pass)
		{
			//FollowsStar(_, while)
			//select w such that Follows*(_, w)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_Underscore_SynIf_Pass)
		{
			//FollowsStar(_, if)
			//select if such that Follows*(_, if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateFollowsStar_Underscore_Underscore_Pass)
		{
			//Follows*(_,_)
			//select random such that Follows*(_, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}
