#include "stdafx.h"
#include "TestQueryEvaluator_Calls_CallsStar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Calls_CallsStar)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			//string fileName = "../Tests/Calls_Many.txt";
			for (auto& value : procTableStub_C) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_C) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_C) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_C) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_C) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_C) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : callStmtNumsStub_C) {
				pkb.addStmt(DESIGN_ENTITY::CALL, value);
			}
			for (auto& value : whileStmtNumsStub_C) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_C) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : cTableStub_C) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::CALLS, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : cStarTableStub_C) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::CALLS_STAR, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateCalls
		TEST_METHOD(evaluateCalls_IDENT_IDENT_Pass)
		{
			//Calls("procA", "procB")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procB", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_IDENT_IDENT_Fail)
		{
			//Calls("procA", "procE")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procC", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procE", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_IDENT_IDENT_Cyclic_Fail)
		{
			//Calls("procA", "procA")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_IDENT_underscore_Pass)
		{
			//Calls("procA", "_")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procB", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_IDENT_underscore_Fail)
		{
			//Calls("procA", "_")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procE", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_IDENT_Syn_Pass)
		{
			//Calls("procA", pd)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_IDENT_Syn_Fail)
		{
			//Calls("procE", pd)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procE", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}
	
		TEST_METHOD(evaluateCalls_Underscore_Underscore_Pass)
		{
			//Calls(_, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_Underscore_IDENT_Pass)
		{
			//Calls(_, "pd")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procC", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_Underscore_IDENT_Fail)
		{
			//Calls(_, "pd")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_Underscore_Syn_Pass)
		{
			//Calls(_, pd)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_Syn_IDENT_Pass)
		{
			//Calls(pd, "procC")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procC", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_Syn_underscore_Pass)
		{
			//Calls(pd, "_")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCalls_Syn_Syn_Pass)
		{
			//Calls(pd1, pd2)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd1", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd2", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

#pragma region evaluateCallsStar
		TEST_METHOD(evaluateCallsStar_IDENT_IDENT_Pass)
		{
			//Calls*("procC", "procE")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procC", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procE", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_IDENT_IDENT_Fail)
		{
			//Calls*("procB", "procA")
			SuchThatClause stc;
			stc.relType = SUCH_THAT_REL_TYPE::CALLS_T;
			stc.firstArg.setArgType(ARGUMENT_TYPE::IDENT);
			stc.firstArg.setArgStr("procB");
			stc.secondArg.setArgType(ARGUMENT_TYPE::IDENT);
			stc.secondArg.setArgStr("procA");
			
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_IDENT_IDENT_Cyclic_Fail)
		{
			//Calls*("procA", "procA")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_IDENT_underscore_Pass)
		{
			//Calls*("procA", "_")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_IDENT_underscore_Fail)
		{
			//Calls*("procE", "_")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procE", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			
			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());	
		}

		TEST_METHOD(evaluateCallsStar_IDENT_Syn_Pass)
		{
			//Calls*("procC", pd)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procC", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_IDENT_Syn_Fail)
		{
			//Calls*("procE", pd)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procE", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_Underscore_Underscore_Pass)
		{
			//Calls*(_, _)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_Underscore_IDENT_Pass)
		{
			//Calls*(_, "procB")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procB", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());	
		}

		TEST_METHOD(evaluateCallsStar_Underscore_IDENT_Fail)
		{
			//Calls*(_, "procA")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procA", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);

			Assert::IsFalse (SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_Underscore_Syn_Pass)
		{
			//Calls*(_, pd)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			
			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_Syn_IDENT_Pass)
		{
			//Calls*(pd, "procC")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "procC", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_Syn_underscore_Pass)
		{
			//Calls*(pd, "_")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateCallsStar_Syn_Syn_Pass)
		{
			//Calls*(pd1, pd2)
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd1", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd2", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::CALLS_T, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}
