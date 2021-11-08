#include "stdafx.h"
#include "TestQueryEvaluator_Parent_ParentStar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Parent_ParentStar)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_P) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_P) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_P) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_P) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_P) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_P) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_P) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_P) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : pTableStub_P) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::PARENT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : pStarTableStub_P) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::PARENT_STAR, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateParent

		TEST_METHOD(evaluateParent_int_int_Pass)
		{
			//Parent(4,5)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "6", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_SynStmt_Pass)
		{
			//Parent(int,stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_SynRead_Pass)
		{
			//Parent(int,read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_SynPrint_Pass)
		{
			//Parent(int,print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_SynAssign_Pass)
		{
			//Parent(int,assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_SynWhile_Pass)
		{
			//Parent(int,while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_SynIf_Pass)
		{
			//Parent(int,if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_Underscore_Pass)
		{
			//Parent(int,_)
			QueryArgument firstArg1(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg1(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc1(SUCH_THAT_REL_TYPE::PARENT, firstArg1, secondArg1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc1).evaluateClause());

			QueryArgument firstArg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "14", QueryElement());
			QueryArgument secondArg2(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc2(SUCH_THAT_REL_TYPE::PARENT, firstArg2, secondArg2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc2).evaluateClause());
		}

		TEST_METHOD(evaluateParent_int_Underscore_Fail)
		{
			//Parent(int,_)
			QueryArgument firstArg1(NO_ERROR, ARGUMENT_TYPE::INTEGER, "0", QueryElement());
			QueryArgument secondArg1(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc1(SUCH_THAT_REL_TYPE::PARENT, firstArg1, secondArg1);

			Assert::IsFalse(SuchThatClauseEvaluator(stc1).evaluateClause());

			QueryArgument firstArg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg2(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc2(SUCH_THAT_REL_TYPE::PARENT, firstArg2, secondArg2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc2).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_int_Pass)
		{
			//Parent(s, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "6", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynRead_int_Fail)
		{
			//Parent(read, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "6", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynWhile_int_Pass)
		{
			//Parent(while, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "6", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynIf_int_Pass)
		{
			//Parent(if, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "12", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_SynStmt_Pass)
		{
			//Parent(s,s)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_SynAssign_Pass)
		{
			//Parent(stmt,assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_SynPrint_Pass)
		{
			//Parent(stmt,print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_SynWhile_Pass)
		{
			//Parent(stmt,while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_SynIf_Pass)
		{
			//Parent(stmt,if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynWhile_SynStmt_Pass)
		{
			//Parent(while,stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(4);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynWhile_SynRead_Pass)
		{
			//Parent(while,read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(4);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynWhile_SynWhile_Pass)
		{
			//Parent(w,w)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w1", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(4);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynWhile_SynIf_Pass)
		{
			//Parent(w,if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(4);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynIf_SynStmt_Pass)
		{
			//Parent(if, stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs2", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
	
		TEST_METHOD(evaluateParent_SynIf_SynRead_Pass)
		{
			//Parent(if, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynIf_SynWhile_Pass)
		{
			//Parent(if, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_SynStmt_Underscore_Pass)
		{
			//Parent(stmt, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_Underscore_int_Pass)
		{
			//Parent(_, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "7", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_Underscore_SynStmt_Pass)
		{
			//Parent(_,stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParent_Underscore_Underscore_Pass)
		{
			//Parent(_,_)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

#pragma region evaluateParentStar

		TEST_METHOD(evaluateParentStar_int_int_Pass)
		{
			//Parent*(int, int) 
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_int_SynStmt_Pass)
		{
			//Parent(int, stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_int_SynAssign_Pass)
		{
			//Parent(int, assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_int_SynPrint_Pass)
		{
			//Parent(int, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_int_SynWhile_Pass)
		{
			//Parent(int, assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_int_Underscore_Pass)
		{
			//Parent*(int,_) 
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "5", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_int_Pass)
		{
			//Parent(stmt, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_int_Pass)
		{
			//Parent(if, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_int_Pass)
		{
			//Parent(if, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "21", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_SynStmt_Pass)
		{
			//Parent(stmt, stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_SynRead_Pass)
		{
			//Parent(stmt, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_SynPrint_Pass)
		{
			//Parent(stmt, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_SynAssign_Pass)
		{
			//Parent(stmt, assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_SynIf_Pass)
		{
			//Parent(stmt, if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_SynWhile_Pass)
		{
			//Parent(stmt, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_SynStmt_Pass)
		{
			//Parent(while, stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_SynRead_Pass)
		{
			//Parent(while, read)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_SynPrint_Pass)
		{
			//Parent(while, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_SynAssign_Pass)
		{
			//Parent(while, assign)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_SynWhile_Pass)
		{
			//Parent(while, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w1", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_SynIf_Pass)
		{
			//Parent(while, if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_SynStmt_Pass)
		{
			//Parent(if, stmt)
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(5);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_SynPrint_Pass)
		{
			//Parent(if, Print)
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(5);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_SynAssign_Pass)
		{
			//Parent(if, assign)
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(5);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_SynIf_Pass)
		{
			//Parent(if, if)
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs1", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(5);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_SynWhile_Pass)
		{
			//Parent(if, while)
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(5);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynStmt_Underscore_Pass)
		{
			//Parent(stmt, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynWhile_Underscore_Pass)
		{
			//Parent(while, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_SynIf_Underscore_Pass)
		{
			//Parent(if, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_Underscore_int_Pass)
		{
			//Parent(_, int)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_Underscore_SynStmt_Pass)
		{
			//Parent(_, Stmt)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_Underscore_SynPrint_Pass)
		{
			//Parent(_, print)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_Underscore_SynWhile_Pass)
		{
			//Parent(_, while)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_Underscore_SynIf_Pass)
		{
			//Parent(_, if)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateParentStar_Underscore_Underscore_Pass)
		{
			//Parent(_, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::PARENT_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}
