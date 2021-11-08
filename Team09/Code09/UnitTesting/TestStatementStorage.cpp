#include "stdafx.h"
#include "TestStatementStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestStatementStorage)
	{
	public:
		StatementStorage stmtStore;

		TEST_METHOD_INITIALIZE(stmtStore_Init)
		{
			stmtStore.addStmt(DESIGN_ENTITY::ASSIGN, "1");
			stmtStore.addStmt(DESIGN_ENTITY::ASSIGN, "2");
			stmtStore.addStmt(DESIGN_ENTITY::ASSIGN, "3");
			stmtStore.addStmt(DESIGN_ENTITY::PRINT, "4");
			stmtStore.addStmt(DESIGN_ENTITY::READ, "5");

			stmtStore.addStmtVar(DESIGN_ENTITY::ASSIGN, "1", "a");
			stmtStore.addStmtVar(DESIGN_ENTITY::ASSIGN, "2", "b");
			stmtStore.addStmtVar(DESIGN_ENTITY::ASSIGN, "3", "c");
		}

		TEST_METHOD_CLEANUP(basicStore_Cleanup)
		{
			stmtStore.clear();
		}

		TEST_METHOD(addStmt_Pass)
		{
			stmtStore.addStmt(DESIGN_ENTITY::CALL, "6");
			Assert::IsTrue(stmtStore.isOfType(DESIGN_ENTITY::CALL, stmtStore.getIndexOf("6")));
		}

		TEST_METHOD(getIndexedStmtsOf_Pass)
		{
			Assert::IsTrue(stmtStore.getIndexedStmtsOf(DESIGN_ENTITY::ASSIGN) == stmtIndicesStub);
			Assert::IsTrue(stmtStore.getIndexedStmtsOf(DESIGN_ENTITY::WHILE) == emptyIndicesStub);
		}

		TEST_METHOD(isOfType_Pass)
		{
			Assert::IsTrue(stmtStore.isOfType(DESIGN_ENTITY::ASSIGN, stmtStore.getIndexOf("1")));
			Assert::IsTrue(stmtStore.isOfType(DESIGN_ENTITY::ASSIGN, stmtStore.getIndexOf("2")));
			Assert::IsTrue(stmtStore.isOfType(DESIGN_ENTITY::ASSIGN, stmtStore.getIndexOf("3")));
			Assert::IsTrue(stmtStore.isOfType(DESIGN_ENTITY::PRINT, stmtStore.getIndexOf("4")));
			Assert::IsTrue(stmtStore.isOfType(DESIGN_ENTITY::READ, stmtStore.getIndexOf("5")));
		}

		TEST_METHOD(getStmtVarsOf_Fail)
		{
			Assert::IsTrue(stmtStore.getStmtVarsOf(DESIGN_ENTITY::IF) == emptyStmtVarStub);
		}

		TEST_METHOD(addStmtVar_Pass)
		{
			stmtStore.addStmtVar(DESIGN_ENTITY::PRINT, "7", "d");
			Assert::IsTrue(stmtStore.getIndexedVarOf(DESIGN_ENTITY::PRINT, stmtStore.getIndexOf("7")) == stmtStore.getIndexOf("d"));
		}

		TEST_METHOD(getIndexedVarsOf_Fail)
		{
			Assert::IsTrue(stmtStore.getIndexedVarsOf(DESIGN_ENTITY::IF) == emptyIndicesStub);
		}

		TEST_METHOD(getIndexedStmtsWithVar_Fail)
		{
			Assert::IsTrue(stmtStore.getIndexedStmtsWithVar(DESIGN_ENTITY::IF, 0) == emptyIndicesStub);
		}

		TEST_METHOD(getTableSize_Pass)
		{
			Assert::IsTrue(stmtStore.getTableSize() == 10);
		}

		TEST_METHOD(getTableSizeOf_Pass)
		{
			Assert::IsTrue(stmtStore.getTableSizeOf(DESIGN_ENTITY::ASSIGN) == 3);
		}

		TEST_METHOD(isEmpty_Pass) 
		{
			Assert::IsFalse(stmtStore.isEmpty());
			stmtStore.clear();
			Assert::IsTrue(stmtStore.isEmpty());
		}

		TEST_METHOD(clear_Pass)
		{
			stmtStore.clear();
			Assert::IsTrue(stmtStore.isEmpty());
		}
	};
}