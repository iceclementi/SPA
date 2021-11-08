#include "stdafx.h"
#include "TestBasicStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestBasicStorage)
	{
	public:
		BasicStorage<VAR> basicStore;

		TEST_METHOD_INITIALIZE(basicStore_Init)
		{
			basicStore.addValue("a");
			basicStore.addValue("b");
			basicStore.addValue("c");
		}

		TEST_METHOD_CLEANUP(basicStore_Cleanup)
		{
			basicStore.clear();
		}

		TEST_METHOD(addValue_Pass)
		{
			basicStore.addValue("d");
			INDEX index = basicStore.getIndexOf("d");
			Assert::IsTrue(basicStore.contains(index));
		}

		TEST_METHOD(addValueDup_Fail)
		{
			basicStore.addValue("a");
			INDEX index = basicStore.getIndexOf("a");
			Assert::IsTrue(basicStore.getIndexedValues().count(index) == 1);
		}

		TEST_METHOD(getTable_Pass)
		{
			Assert::IsTrue(basicStore.getIndexedValues() == tableStub);
		}

		TEST_METHOD(contains_Pass)
		{
			INDEX index = basicStore.getIndexOf("c");
			Assert::IsTrue(basicStore.contains(index));
		}

		TEST_METHOD(contains_Fail)
		{
			INDEX index = basicStore.getIndexOf("e");
			Assert::IsFalse(basicStore.contains(index));
		}

		TEST_METHOD(getSize_Pass)
		{
			Assert::IsTrue(basicStore.getTableSize() == tableStub.size());
		}

		TEST_METHOD(clear_Pass)
		{
			basicStore.clear();
			Assert::IsTrue(basicStore.isEmpty());
			Assert::IsTrue(basicStore.getIndexedValues().empty());
		}
	};
}
