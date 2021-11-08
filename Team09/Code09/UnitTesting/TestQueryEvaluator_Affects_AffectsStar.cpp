#include "stdafx.h"
#include "TestQueryEvaluator_Affects_AffectsStar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Affects_AffectsStar)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			//string fileName = "../Tests/Calls_Many.txt";
			for (auto& value : procTableStub_A) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_A) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_A) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			
			for (auto& value : assignStmtNumsStub_A) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}

			for (auto& value : allStmtNumsStub_A) {
				pkb.addStmt(DESIGN_ENTITY::STATEMENT, value);
			}
			

			for (auto& valueGroup : aTableStub_A) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : aStarTableStub_A) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_STAR, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateAffects
		TEST_METHOD(evaluateAffects_INT_INT_Pass_1)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "26", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "27", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffects_INT_INT_Pass_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "33", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "33", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffects_INT_INT_Fail)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "26", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "29", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffects_INT_INT_Fail_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "32", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "32", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffects_INT_SYN_Pass_1)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1", 
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffects_INT_SYN_Pass_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", 
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffects_SYN_SYN_Pass_1)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1", 
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s2",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffects_SYN_SYN_Pass_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1", 
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a2",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffects_SYN_SYN_Pass_3)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffects_UNDERSCORE_INT_Pass)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "27", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffects_UNDERSCORE_UNDERSCORE_Pass)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

#pragma region evaluateAffectsStar

		TEST_METHOD(evaluateAffectsStar_INT_INT_Pass_1)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "26", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "26", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffectsStar_INT_INT_Pass_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "33", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "33", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffectsStar_INT_INT_Fail)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "26", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "29", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffectsStar_INT_INT_Fail_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "32", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "32", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffectsStar_INT_SYN_Pass_1)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffectsStar_INT_SYN_Pass_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffectsStar_SYN_SYN_Pass_1)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s2",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffectsStar_SYN_SYN_Pass_2)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a2",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateAffectsStar_SYN_SYN_Pass_3)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffectsStar_UNDERSCORE_INT_Pass)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "27", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffectsStar_UNDERSCORE_SYN_Pass)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a1", 
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}
		TEST_METHOD(evaluateAffectsStar_UNDERSCORE_UNDERSCORE_Pass)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::AFFECTS_T, firstArg, secondArg);
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}

