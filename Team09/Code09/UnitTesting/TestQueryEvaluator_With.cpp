#include "stdafx.h"
#include "TestQueryEvaluator_With.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_With)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_WITH) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_WITH) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_WITH) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}
			for (auto& value : allStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::STATEMENT, value);
			}
			for (auto& value : readStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}
			for (auto& value : callStmtNumsStub_WITH) {
				pkb.addStmt(DESIGN_ENTITY::CALL, value);
			}
			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region Evaluate If Pattern
		/*
		procedure.procName, call.procName, variable.varName, read.varName, print.varName: NAME
		constant.value : INTEGER			
		stmt.stmt#, read.stmt#, print.stmt#, call.stmt#, while.stmt#, if.stmt#, assign.stmt# : INTEGER
		*/

		TEST_METHOD(evaluateWith_Pass)
		{
			//Select pd with 1 = 1
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::IDENT, "DoesitMatch", QueryElement());
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::IDENT, "DoesitMatch", QueryElement());
			WithClause withClause (arg1, arg2);
			Assert::IsTrue(WithClauseEvaluator(withClause).evaluateClause());
		}
		TEST_METHOD(evaluateWith_attrREF_procName_INT_Pass)
		{
			//p.procName = QWERTYUIOPASDFGHJKLZXCVBNM1234567890 
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "p.procName", 
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NAME));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::IDENT, "QWERTYUIOPASDFGHJKLZXCVBNM1234567890", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_attrREF_varName_INT_Pass)
		{
			//v.varName =  
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "v.varName",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NAME));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::IDENT, "qwertyuiopasdfghjklzxcvbnm1234567890", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_attrREF_stmt_INT_Pass)
		{
			//a.stmt# = 1
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "a.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "30", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_attrREF_value_INT_Pass)
		{
			//c.value = 
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "c.value",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::CONSTANT, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "123456789012345678901234567890", QueryElement());
			WithClause wc(arg1, arg2);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());	
		}

		TEST_METHOD(evaluateWith_SYN_INT_Pass)
		{
			//n = 10
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			WithClause wc(arg1, arg2);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_SYN_INT_Fail)
		{
			//n = 3203
			// SYNONYM must be progline if used alone, without attr reference
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "3203", QueryElement());
			WithClause wc(arg1, arg2);
			Assert::IsFalse(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_SYN_INT_Fail2)
		{
			// SYNONYM must be progline if used alone, without attr 
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			WithClause wc(arg1, arg2);
			Assert::IsFalse(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_attrRef_attrRef_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "p.procName",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NAME));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "v.varName",
				QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NAME));
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			wc.addClauseSynonym(2);
			Assert::IsFalse(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_readStmtattrRef_INT_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "read.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::READ, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_printStmtattrRef_INT_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "print.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PRINT, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "20", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_assignStmtattrRef_INT_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "assign.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "30", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_whileStmtattrRef_INT_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "while.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::WHILE, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "3", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_IftmtattrRef_INT_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "ifs.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::IF, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "4", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

		TEST_METHOD(evaluateWith_callStmtattrRef_INT_Pass)
		{
			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "while.stmt#",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
			Assert::IsTrue(WithClauseEvaluator(wc).evaluateClause());
		}

#pragma endregion

	};
}
