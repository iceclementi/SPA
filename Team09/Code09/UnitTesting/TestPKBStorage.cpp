#include "stdafx.h"
#include "TestPKBStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPKBStorage)
	{
	public:

		RelationshipStorage<VALUE, VALUE> cStore;
		RelationshipStorage<VALUE, VALUE> cStarStore;

		TEST_METHOD_INITIALIZE(CallsStorage_Init)
		{
			cStore.addRelationship("one", "two");
			cStore.addRelationship("one", "three");
			cStore.addRelationship("two", "three");
			cStore.addRelationship("two", "five");
			cStore.addRelationship("three", "four");
			cStore.addRelationship("four", "five");
			cStarStore.addRelationship("one", "two");
			cStarStore.addRelationship("one", "three");
			cStarStore.addRelationship("one", "four");
			cStarStore.addRelationship("one", "five");
			cStarStore.addRelationship("two", "three");
			cStarStore.addRelationship("two", "four");
			cStarStore.addRelationship("two", "five");
			cStarStore.addRelationship("three", "four");
			cStarStore.addRelationship("three", "five");
			cStarStore.addRelationship("four", "five");
		}

		TEST_METHOD_CLEANUP(CallsStorage_Cleanup)
		{
			cStore.clear();
			cStarStore.clear();
		}

		TEST_METHOD(getCallsTables_Pass)
		{
			Assert::IsTrue(cStore.getForwardTable() == forwardTableStub);
			Assert::IsTrue(cStore.getBackwardTable() == cBackwardTableStub);
		}

		TEST_METHOD(getCallsStarTables_Pass)
		{
			Assert::IsTrue(cStarStore.getForwardTable() == cStarForwardTableStub);
			Assert::IsTrue(cStarStore.getBackwardTable() == cStarBackwardTableStub);
		}

		TEST_METHOD(addCalls_Pass)
		{
			Assert::IsTrue(cStore.addRelationship("five", "six"));
			Assert::IsTrue(cStore.hasForwardRelationship("five", "six"));
			Assert::IsTrue(cStore.hasBackwardRelationship("six", "five"));
		}

		TEST_METHOD(addCallsStar_Pass)
		{
			Assert::IsTrue(cStarStore.addRelationship("five", "six"));
			Assert::IsTrue(cStarStore.hasForwardRelationship("five", "six"));
			Assert::IsTrue(cStarStore.hasBackwardRelationship("six", "five"));
		}

		TEST_METHOD(addCallsDup_Fail)
		{
			Assert::IsFalse(cStore.addRelationship("one", "two"));
			Assert::IsTrue(cStore.hasForwardRelationship("one", "two"));
			Assert::IsTrue(cStore.hasBackwardRelationship("two", "one"));
		}

		TEST_METHOD(addCallsStarDup_Fail)
		{
			Assert::IsFalse(cStarStore.addRelationship("three", "five"));
			Assert::IsTrue(cStarStore.hasForwardRelationship("three", "five"));
			Assert::IsTrue(cStarStore.hasBackwardRelationship("five", "three"));
		}

		TEST_METHOD(calls_called_Fail)
		{
			Assert::IsFalse(cStore.hasForwardRelationship("two", "one"));
			Assert::IsFalse(cStore.hasForwardRelationship("five", "ten"));
			Assert::IsFalse(cStore.hasForwardRelationship("four", ""));
			Assert::IsFalse(cStore.hasForwardRelationship("", "two"));
			Assert::IsFalse(cStore.hasForwardRelationship("", ""));

			Assert::IsFalse(cStore.hasBackwardRelationship("one", "two"));
			Assert::IsFalse(cStore.hasBackwardRelationship("ten", "five"));
			Assert::IsFalse(cStore.hasBackwardRelationship("", "four"));
			Assert::IsFalse(cStore.hasBackwardRelationship("two", ""));
			Assert::IsFalse(cStore.hasBackwardRelationship("", ""));
		}

		TEST_METHOD(callStar_calledStar_Fail)
		{
			Assert::IsFalse(cStore.hasForwardRelationship("two", "one"));
			Assert::IsFalse(cStore.hasForwardRelationship("five", "nine"));
			Assert::IsFalse(cStore.hasForwardRelationship("three", ""));
			Assert::IsFalse(cStore.hasForwardRelationship("", "four"));
			Assert::IsFalse(cStore.hasForwardRelationship("", ""));

			Assert::IsFalse(cStore.hasBackwardRelationship("one", "two"));
			Assert::IsFalse(cStore.hasBackwardRelationship("nine", "five"));
			Assert::IsFalse(cStore.hasBackwardRelationship("", "three"));
			Assert::IsFalse(cStore.hasBackwardRelationship("four", ""));
			Assert::IsFalse(cStore.hasBackwardRelationship("", ""));
		}

		TEST_METHOD(clearCalls_Pass)
		{
			cStore.clear();
			Assert::IsTrue(cStore.getForwardTable().empty());
			Assert::IsTrue(cStore.getBackwardTable().empty());
			Assert::IsTrue(cStore.getFirstValues().empty());
			Assert::IsTrue(cStore.getSecondValues().empty());
		}

		TEST_METHOD(clearCallsStar_Pass)
		{
			cStarStore.clear();
			Assert::IsTrue(cStarStore.getForwardTable().empty());
			Assert::IsTrue(cStarStore.getBackwardTable().empty());
			Assert::IsTrue(cStarStore.getFirstValues().empty());
			Assert::IsTrue(cStarStore.getSecondValues().empty());
		}
	};

	TEST_CLASS(TestPatternStorage)
	{
	public:

		PatternStorage patStore;

		TEST_METHOD_INITIALIZE(patStore_Init)
		{
			patStore.addAssign("1", "x", "x + 1");
			patStore.addAssign("2", "y", "5");
			patStore.addAssign("3", "z", "y * x");
			patStore.addWhile("4", temp1);
			patStore.addIf("5", temp2);
			patStore.addWhile("6", temp3);
			patStore.addIf("7", temp4);
		}

		TEST_METHOD_CLEANUP(patStore_Cleanup)
		{
			patStore.clear();
		}

		TEST_METHOD(containsAssign_Pass)
		{
			Assert::IsTrue(patStore.containsAssign("3"));
		}

		TEST_METHOD(containsAssign_Fail)
		{
			Assert::IsFalse(patStore.containsAssign("4"));
		}

		TEST_METHOD(containsWhile_Pass)
		{
			Assert::IsTrue(patStore.containsWhile("4"));
		}

		TEST_METHOD(containsWhile_Fail)
		{
			Assert::IsFalse(patStore.containsWhile("5"));
		}

		TEST_METHOD(containsIf_Pass)
		{
			Assert::IsTrue(patStore.containsIf("7"));
		}

		TEST_METHOD(containsIf_Fail)
		{
			Assert::IsFalse(patStore.containsIf("1"));
		}

		TEST_METHOD(addAssign_Pass)
		{
			Assert::IsTrue(patStore.addAssign("8", "a", "b 10 - "));
			Assert::IsTrue(patStore.getAssignVarOf("8") == "a");
			Assert::IsTrue(patStore.getPatternOf("8") == "b 10 - ");
		}

		TEST_METHOD(addWhile_Pass)
		{
			VAR_NAMES temp = { "c" };
			patStore.addWhile("9", temp);
			Assert::IsTrue(patStore.getWhileControlVarsOf("9").count("c"));
		}

		TEST_METHOD(addIf_Pass)
		{
			VAR_NAMES temp = { "d" };
			patStore.addIf("10", temp);
			Assert::IsTrue(patStore.getIfControlVarsOf("10").count("d"));
		}

		TEST_METHOD(getAssignTable_Pass)
		{
			Assert::IsTrue(patStore.getAssignTable() == assignTableStub);
		}

		TEST_METHOD(getWhileTable_Pass)
		{
			Assert::IsTrue(patStore.getWhileTable() == whileTableStub);
		}

		TEST_METHOD(getIfTable_Pass)
		{
			Assert::IsTrue(patStore.getIfTable() == ifTableStub);
		}

		TEST_METHOD(getLHSOf_Pass)
		{
			Assert::IsTrue(patStore.getAssignVarOf("2") == varNameStub);
		}

		TEST_METHOD(getRHSOf_Pass)
		{
			Assert::IsTrue(patStore.getPatternOf("1") == patternStub);
		}

		TEST_METHOD(getWhileControlVarsOf_Pass)
		{
			Assert::IsTrue(patStore.getWhileControlVarsOf("4") == whileVarList);
		}

		TEST_METHOD(getIfControlVarsOf_Pass)
		{
			Assert::IsTrue(patStore.getIfControlVarsOf("7") == ifVarList);
		}

		TEST_METHOD(getAssignTableSize_Pass)
		{
			Assert::IsTrue(patStore.getAssignTableSize() == assignTableStub.size());
		}

		TEST_METHOD(getWhileTableSize_Pass)
		{
			Assert::IsTrue(patStore.getWhileTableSize() == whileTableStub.size());
		}

		TEST_METHOD(getIfTableSize_Pass)
		{
			Assert::IsTrue(patStore.getIfTableSize() == ifTableStub.size());
		}

		TEST_METHOD(clear_Pass)
		{
			patStore.clear();
			Assert::IsTrue(patStore.getAssignTable().empty());
			Assert::IsTrue(patStore.getWhileTable().empty());
			Assert::IsTrue(patStore.getIfTable().empty());
		}
	};
}
