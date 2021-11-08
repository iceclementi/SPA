#include "stdafx.h"
#include "TestRelationshipStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestRelationshipStorage)
	{
	public:

		RelationshipStorage<VALUE, VALUE> relStore;

		TEST_METHOD_INITIALIZE(RelationshipStorage_Init)
		{
			relStore.addRelationship(1, 2);
			relStore.addRelationship(1, 3);
			relStore.addRelationship(2, 3);
			relStore.addRelationship(2, 5);
			relStore.addRelationship(3, 4);
			relStore.addRelationship(4, 5);
		}

		TEST_METHOD_CLEANUP(RelationshipStorage_Cleanup)
		{
			relStore.clear();
		}

		TEST_METHOD(getRelationshipTables_Pass)
		{
			Assert::IsTrue(relStore.getForwardTable() == forwardTableStub);
			Assert::IsTrue(relStore.getBackwardTable() == backwardTableStub);
		}

		TEST_METHOD(addRelationship_Pass)
		{
			relStore.addRelationship(5, 6);
			Assert::IsTrue(relStore.hasRelationship(5, 6));
		}

		TEST_METHOD(addRelationshipDup_Fail)
		{
			int initialSize = relStore.getTableSize();
			relStore.addRelationship(1, 2);
			Assert::IsTrue(relStore.getTableSize() == initialSize);
		}

		TEST_METHOD(removeRelationship_Pass)
		{
			relStore.removeRelationship(4, 5);
			Assert::IsFalse(relStore.hasRelationship(4, 5));
		}

		TEST_METHOD(getFirstValues_Pass) 
		{
			Assert::IsTrue(relStore.getFirstValues() == firstValuesStub);
		}

		TEST_METHOD(getSecondValues_Pass)
		{
			Assert::IsTrue(relStore.getSecondValues() == secondValuesStub);
		}

		TEST_METHOD(getValuePairs_Pass)
		{
			Assert::IsTrue(relStore.getValuePairs() == indexPairsStub);
		}

		TEST_METHOD(getFirstValuesOf_Pass)
		{
			Assert::IsTrue(relStore.getFirstValuesOf(2) == backwardTableStub[2]);
			Assert::IsTrue(relStore.getFirstValuesOf(3) == backwardTableStub[3]);
			Assert::IsTrue(relStore.getFirstValuesOf(4) == backwardTableStub[4]);
			Assert::IsTrue(relStore.getFirstValuesOf(5) == backwardTableStub[5]);
		}

		TEST_METHOD(getSecondValuesOf_Pass)
		{
			Assert::IsTrue(relStore.getSecondValuesOf(1) == forwardTableStub[1]);
			Assert::IsTrue(relStore.getSecondValuesOf(2) == forwardTableStub[2]);
			Assert::IsTrue(relStore.getSecondValuesOf(3) == forwardTableStub[3]);
			Assert::IsTrue(relStore.getSecondValuesOf(4) == forwardTableStub[4]);
		}

		TEST_METHOD(hasRelationship_Pass)
		{
			Assert::IsTrue(relStore.hasRelationship(1, 2));
			Assert::IsTrue(relStore.hasRelationship(1, 3));
			Assert::IsTrue(relStore.hasRelationship(2, 3));
			Assert::IsTrue(relStore.hasRelationship(2, 5));
			Assert::IsTrue(relStore.hasRelationship(4, 5));
			Assert::IsTrue(relStore.hasRelationship(3, 4));

			Assert::IsFalse(relStore.hasRelationship(2, 1));
			Assert::IsFalse(relStore.hasRelationship(5, 10));
			Assert::IsFalse(relStore.hasRelationship(4, 0));
			Assert::IsFalse(relStore.hasRelationship(0, 2));
			Assert::IsFalse(relStore.hasRelationship(-1, -2));
		}
		
		TEST_METHOD(hasRelationshipWithSecondValue_Pass)
		{
			Assert::IsFalse(relStore.hasRelationshipWithSecondValue(1));
			Assert::IsTrue(relStore.hasRelationshipWithSecondValue(2));
			Assert::IsTrue(relStore.hasRelationshipWithSecondValue(3));
			Assert::IsTrue(relStore.hasRelationshipWithSecondValue(4));
			Assert::IsTrue(relStore.hasRelationshipWithSecondValue(5));
		}

		TEST_METHOD(hasRelationshipWithFirstValue_Pass)
		{
			Assert::IsTrue(relStore.hasRelationshipWithFirstValue(1));
			Assert::IsTrue(relStore.hasRelationshipWithFirstValue(2));
			Assert::IsTrue(relStore.hasRelationshipWithFirstValue(3));
			Assert::IsTrue(relStore.hasRelationshipWithFirstValue(4));
			Assert::IsFalse(relStore.hasRelationshipWithFirstValue(5));
		}

		TEST_METHOD(getTableSize_Pass)
		{
			Assert::IsTrue(relStore.getTableSize() == indexPairsStub.size());
		}

		TEST_METHOD(isEmpty_Pass)
		{
			Assert::IsFalse(relStore.isEmpty());
			relStore.clear();
			Assert::IsTrue(relStore.isEmpty());
		}

		TEST_METHOD(clear_Pass)
		{
			relStore.clear();
			Assert::IsTrue(relStore.getForwardTable().empty());
			Assert::IsTrue(relStore.getBackwardTable().empty());
			Assert::IsTrue(relStore.getFirstValues().empty());
			Assert::IsTrue(relStore.getSecondValues().empty());
			Assert::IsTrue(relStore.isEmpty());
		}
	};
}
