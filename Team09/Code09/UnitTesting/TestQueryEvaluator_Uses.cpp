#include "stdafx.h"
#include "TestQueryEvaluator_Uses.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Uses)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_U) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_U) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_U) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_U) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_U) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_U) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_U) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_U) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : usesProcTableStub_U) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::USES_PROC, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : usesStmtTableStub_U) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::USES_STMT, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateUses

		TEST_METHOD(evaluateUses_Int_Ident_Pass)
		{
			/* Select w such that Uses(12, "b") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "12", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "b", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Int_Ident_Fail)
		{
			// Select ifs such that Uses(10, "c")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "c", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Int_Syn_Var_Pass)
		{
			/* Select r such that Uses(3, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "3", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Int_Syn_Var_Fail)
		{
			/* Select p such that Uses(1, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Int_Underscore_Pass)
		{
			/* Select a such that Uses(4, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Int_Underscore_Fail)
		{
			/* Select c such that Uses(14, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "14", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Proc_Indent_Pass)
		{
			/* Select a such that Uses("um", "a") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "a", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Proc_Indent_Fail)
		{
			/* Select c such that Uses("um2", "G") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "G", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Proc_Syn_Var_Pass)
		{
			/* Select v such that Uses("um", v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Proc_Underscore_Pass)
		{
			/* Select pd such that Uses("um", _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Stmt_Ident_Pass)
		{
			/* Select s such that Uses(s, "F") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "F", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Stmt_Ident_Fail)
		{
			/* Select w such that Uses(w, "e") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Proc_Ident_Pass)
		{
			/* Select pd such that Uses(pd, "A") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "A", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Proc_Ident_Fail)
		{
			/* Select pd such that Uses(pd, "d") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "d", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(4);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Stmt_Syn_Var_Pass)
		{
			/* Select a such that Uses(a, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Stmt_Syn_Var_Fail)
		{
			/* Select pd such that Uses(r, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "r", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Proc_Syn_Var_Pass)
		{
			/* Select v such that Uses(pd, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Stmt_Underscore_Pass)
		{
			/* Select ifs such that Uses(ifs, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(4);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateUses_Syn_Proc_Underscore_Pass)
		{
			/* Select pd such that Uses(pd, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::USES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}
