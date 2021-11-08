#include "stdafx.h"
#include "TestIndexTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestIndexTable)
	{
	public:
		IndexTable<VAR> indexTable;

		TEST_METHOD_INITIALIZE(basicStore_Init)
		{
			indexTable.addMapping("a");
			indexTable.addMapping("b");
			indexTable.addMapping("c");
			indexTable.addMapping("d");
			indexTable.addMapping("e");
		}

		TEST_METHOD_CLEANUP(basicStore_Cleanup)
		{
			indexTable.clear();
		}

		TEST_METHOD(addMapping_Pass)
		{
			INDEX index = indexTable.addMapping("f");
			Assert::IsTrue(indexTable.getValueWithIndex(index) == "f");
			Assert::IsTrue(indexTable.getIndexOf("f") == 6);
		}

		TEST_METHOD(getValueWithIndex_Pass)
		{
			Assert::IsTrue(indexTable.getValueWithIndex(1) == "a");
			Assert::IsTrue(indexTable.getValueWithIndex(2) == "b");
			Assert::IsTrue(indexTable.getValueWithIndex(3) == "c");
			Assert::IsTrue(indexTable.getValueWithIndex(4) == "d");
			Assert::IsTrue(indexTable.getValueWithIndex(5) == "e");
		}

		TEST_METHOD(getIndexOf_Pass)
		{
			Assert::IsTrue(indexTable.getIndexOf("a") == 1);
			Assert::IsTrue(indexTable.getIndexOf("b") == 2);
			Assert::IsTrue(indexTable.getIndexOf("c") == 3);
			Assert::IsTrue(indexTable.getIndexOf("d") == 4);
			Assert::IsTrue(indexTable.getIndexOf("e") == 5);
		}

		TEST_METHOD(createIndexOf_Pass)
		{
			Assert::IsTrue(indexTable.createIndexOf("a") == 1);
			Assert::IsTrue(indexTable.createIndexOf("f") == 6);
		}

		TEST_METHOD(getTableSize_Pass)
		{
			Assert::IsTrue(indexTable.getTableSize() == indexToValueStub.size());
		}

		TEST_METHOD(isEmpty_Pass)
		{
			Assert::IsFalse(indexTable.isEmpty());
			indexTable.clear();
			Assert::IsTrue(indexTable.isEmpty());
		}

		TEST_METHOD(clear_Pass)
		{
			indexTable.clear();
			Assert::IsTrue(indexTable.isEmpty());
		}
	};
}