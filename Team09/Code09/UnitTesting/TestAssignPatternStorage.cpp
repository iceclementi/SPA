#include "stdafx.h"
#include "TestAssignPatternStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestAssignPatternStorage)
	{
	public:
		AssignPatternStorage aPatStore;

		TEST_METHOD_INITIALIZE(aPatStore_Init)
		{
			aPatStore.addPattern(1, 5, "a + b");
			aPatStore.addPattern(2, 6, "c * d");
			aPatStore.addPattern(3, 7, "e / f");
			aPatStore.addPattern(4, 8, "g - h");
		}

		TEST_METHOD_CLEANUP(basicStore_Cleanup)
		{
			aPatStore.clear();
		}

		TEST_METHOD(getAssignVarStore_Pass)
		{
			Assert::IsTrue(aPatStore.getAssignVarStore().getForwardTable() == assignVarTableStub);
		}

		TEST_METHOD(getAssignPatternStore_Pass)
		{
			Assert::IsTrue(aPatStore.getAssignPatternStore().getForwardTable() == assignPatTableStub);
		}

		TEST_METHOD(addPattern_Pass)
		{
			aPatStore.addPattern(9, 10, "i % j");
			Assert::IsTrue(aPatStore.getIndexedVarOf(9) == 10);
			Assert::IsTrue(aPatStore.getPatternOf(9) == "i % j");
		}

		TEST_METHOD(getIndexedStmtsWithVar_Pass)
		{
			Assert::IsTrue(aPatStore.getIndexedStmtsWithVar(7) == indexedStmtsWithVarStub);
			Assert::IsTrue(aPatStore.getIndexedStmtsWithVar(9) == emptyIndices);
		}

		TEST_METHOD(getIndexedStmtsWithPattern_Pass)
		{
			Assert::IsTrue(aPatStore.getIndexedStmtsWithPattern("g - h", true) == indexedStmtsWithPatternStub);
			Assert::IsTrue(aPatStore.getIndexedStmtsWithPattern("g", false) == indexedStmtsWithPatternStub);
			Assert::IsTrue(aPatStore.getIndexedStmtsWithPattern("k", false) == emptyIndices);
		}

		TEST_METHOD(matchesVariableIn_Pass)
		{
			Assert::IsTrue(aPatStore.matchesVariableIn(1, 5));
			Assert::IsTrue(aPatStore.matchesVariableIn(2, 6));
			Assert::IsTrue(aPatStore.matchesVariableIn(3, 7));
			Assert::IsTrue(aPatStore.matchesVariableIn(4, 8));

			Assert::IsFalse(aPatStore.matchesVariableIn(1, 6));
			Assert::IsFalse(aPatStore.matchesVariableIn(2, 9));
			Assert::IsFalse(aPatStore.matchesVariableIn(0, 5));
			Assert::IsFalse(aPatStore.matchesVariableIn(-1, 1));
			Assert::IsFalse(aPatStore.matchesVariableIn(0, 0));
		}

		TEST_METHOD(matchesPatternIn_Pass)
		{
			Assert::IsTrue(aPatStore.matchesPatternIn(1, "a + b", true));
			Assert::IsTrue(aPatStore.matchesPatternIn(2, "c * d", true));
			Assert::IsTrue(aPatStore.matchesPatternIn(3, "e / f", true));
			Assert::IsTrue(aPatStore.matchesPatternIn(4, "g - h", true));

			Assert::IsTrue(aPatStore.matchesPatternIn(1, "a", false));
			Assert::IsTrue(aPatStore.matchesPatternIn(2, "c", false));
			Assert::IsTrue(aPatStore.matchesPatternIn(3, "e", false));
			Assert::IsTrue(aPatStore.matchesPatternIn(4, "g", false));

			Assert::IsFalse(aPatStore.matchesPatternIn(1, "a", true));
			Assert::IsFalse(aPatStore.matchesPatternIn(2, "a + b", true));
			Assert::IsFalse(aPatStore.matchesPatternIn(2, "a", false));
			Assert::IsFalse(aPatStore.matchesPatternIn(5, "a", false));
			Assert::IsFalse(aPatStore.matchesPatternIn(4, "k", false));
		}

		TEST_METHOD(getTableSize_Pass)
		{
			Assert::IsTrue(aPatStore.getTableSize() == assignVarTableStub.size());
		}

		TEST_METHOD(isEmpty_Pass)
		{
			Assert::IsFalse(aPatStore.isEmpty());
			aPatStore.clear();
			Assert::IsTrue(aPatStore.isEmpty());
		}

		TEST_METHOD(clear_Pass)
		{
			aPatStore.clear();
			Assert::IsTrue(aPatStore.isEmpty());
		}
	};
}
