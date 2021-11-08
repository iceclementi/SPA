#include "stdafx.h"
#include "TestQueryEvaluator_Modifies.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Modifies)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_M) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_M) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_M) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_M) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_M) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_M) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_M) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_M) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : modifiesProcTableStub_M) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::MODIFIES_PROC, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : modifiesStmtTableStub_M) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::MODIFIES_STMT, valueGroup.first, value);
				}
			}

			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region evaluateModifies

		TEST_METHOD(evaluateModifies_Int_Ident_Pass)
		{
			/* Select w such that Modifies(26, "G") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "26", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "G", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Int_Ident_Fail)
		{
			// Select ifs such that Modifies(2, "e")
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Int_Syn_Var_Pass)
		{
			/* Select r such that Modifies(34, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "34", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Int_Syn_Var_Fail)
		{
			/* Select p such that Modifies(11, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "11", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Int_Underscore_Pass)
		{
			/* Select a such that Modifies(23, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "23", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Int_Underscore_Fail)
		{
			/* Select c such that Modifies(21, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "21", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Proc_Ident_Pass)
		{
			/* Select a such that Modifies("um", "c") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "c", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Proc_Ident_Fail)
		{
			/* Select c such that Modifies("um2", "E") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "E", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Proc_Syn_Var_Pass)
		{
			/* Select v such that Modifies("um", v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(4);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Proc_Underscore_Pass)
		{
			/* Select pd such that Modifies("um", _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "um", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Stmt_Ident_Pass)
		{
			/* Select s such that Modifies(s, "a") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "a", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Stmt_Ident_Fail)
		{
			/* Select w such that Modifies(w, "F") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "w", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "F", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Proc_Ident_Pass)
		{
			/* Select pd such that Modifies(pd, "C") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "C", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Proc_Ident_Fail)
		{
			/* Select pd such that Modifies(pd, "b") */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::IDENT, "b", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Stmt_Syn_Var_Pass)
		{
			/* Select a such that Modifies(a, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "a", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(3);
			stc.addClauseSynonym(2);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Stmt_Syn_Var_Fail)
		{
			/* Select pd such that Modifies(p, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "p", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			Assert::IsFalse(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Proc_Syn_Var_Pass)
		{
			/* Select v such that Modifies(pd, v) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(2);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Stmt_Underscore_Pass)
		{
			/* Select ifs such that Modifies(ifs, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "ifs", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(3);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

		TEST_METHOD(evaluateModifies_Syn_Proc_Underscore_Pass)
		{
			/* Select pd such that Modifies(pd, _) */
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "pd", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE));
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::MODIFIES, firstArg, secondArg);
			stc.addClauseSynonym(1);

			Assert::IsTrue(SuchThatClauseEvaluator(stc).evaluateClause());
		}

#pragma endregion

	};
}
