#include "stdafx.h"
#include "Test_MiniTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(Test_MiniTable)
	{
	public:
		MiniSingleTable singleTable;
		MiniDoubleTable doubleTable;

		TEST_METHOD_INITIALIZE(MiniTable_Init)
		{
			singleTable = MiniSingleTable(201, singleTableStub);
			doubleTable = MiniDoubleTable(202, 203, doubleTableStub);
		}

#pragma region Mini Single Table

		TEST_METHOD(MiniSingleTable_getSynonym)
		{
			Assert::AreEqual(singleTable.getSynonym(), 201);
		}

		TEST_METHOD(MiniSingleTable_getSynonymValuesOf)
		{
			Assert::IsTrue(singleTable.getSynonymValuesOf(201) == singleTableStub);
			Assert::IsTrue(singleTable.getSynonymValuesOf(202) == emptyValuesStub);
		}

		TEST_METHOD(MiniSingleTable_intersectWith_Pass)
		{
			VALUES erasedValues;
			VALUES intersectValues = { 101, 103, 105, 108, 109 };
			VALUES expectedTable = { 101, 103, 105 };
			VALUES expectedErasedValues = { 102, 104, 106 };
			Assert::IsTrue(singleTable.intersectWith(201, intersectValues, erasedValues));
			Assert::IsTrue(singleTable.getSynonymValuesOf(201) == expectedTable);
			Assert::IsTrue(erasedValues == expectedErasedValues);
		}

		TEST_METHOD(MiniSingleTable_intersectWith_Fail)
		{
			VALUES erasedValues;
			VALUES intersectValues = { 1, 2, 3, 108, 109 };
			VALUES expectedTable = { };
			VALUES expectedErasedValues = { 101, 102, 103, 104, 105, 106 };
			Assert::IsFalse(singleTable.intersectWith(201, intersectValues, erasedValues));
			Assert::IsTrue(singleTable.getSynonymValuesOf(201) == expectedTable);
			Assert::IsTrue(erasedValues == expectedErasedValues);
		}

#pragma endregion

#pragma region Mini Double Table

		TEST_METHOD(MiniDoubleTable_getSynonyms)
		{
			Assert::IsTrue(doubleTable.getSynonyms() == PAIR<SYNONYM_INDEX>(202, 203));
		}

		TEST_METHOD(MiniDoubleTable_getOtherSynonym)
		{
			Assert::AreEqual(doubleTable.getOtherSynonym(202), 203);
			Assert::AreEqual(doubleTable.getOtherSynonym(203), 202);
		}

		TEST_METHOD(MiniDoubleTable_getSynonymValuesOf)
		{
			VALUES expectedSynonymValuesB = { 1, 2, 3, 4, 5 };
			VALUES expectedSynonymValuesC = { 1, 2, 3, 5 };
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202) == expectedSynonymValuesB);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(203) == expectedSynonymValuesC);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(204) == emptyValuesStub);

			VALUE_PAIRS expectedSynonymValuesCB = {
				{ 1, 1 }, { 2, 1 }, { 1, 2 }, { 1, 3 }, { 3, 3 },
				{ 1, 4 }, { 5, 4 }, { 2, 5 }, { 3, 5 }, { 5, 5 }
			};
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 203) == doubleTableStub);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(203, 202) == expectedSynonymValuesCB);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(203, 204) == emptyValuePairsStub);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(204, 205) == emptyValuePairsStub);

			VALUES expectedSynonymValuesBC1 = { 1, 2, 3, 4 };
			VALUES expectedSynonymValuesCB5 = { 2, 3, 5 };
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 203, 1) == expectedSynonymValuesBC1);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(203, 202, 5) == expectedSynonymValuesCB5);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 203, 6) == emptyValuesStub);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 204, 2) == emptyValuesStub);
		}

		TEST_METHOD(MiniDoubleTable_intersectWith_Pass)
		{
			VALUES intersectValues = { 1, 3, 5, 7, 9 };
			VALUE_PAIRS expectedTableIntersectB = { { 1, 1 }, { 1, 2 }, { 3, 1 }, { 3, 3 }, { 5, 2 }, { 5, 3 }, { 5, 5 } };
			Assert::IsTrue(doubleTable.intersectWith(202, intersectValues));
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 203) == expectedTableIntersectB);
		}

		TEST_METHOD(MiniDoubleTable_intersectWith_Fail)
		{
			VALUES intersectValues = { 4, 6, 8 };
			Assert::IsFalse(doubleTable.intersectWith(203, intersectValues));
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 203) == emptyValuePairsStub);
		}

		TEST_METHOD(MiniDoubleTable_removeSynonym)
		{
			VALUES valuesToErase = { 2, 4, 5 };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 1, 2 }, { 3, 1 }, { 3, 3 } };
			VALUES expectedSynonymValuesB = { 1, 3 };
			VALUES expectedSynonymValuesC = { 1, 2, 3 };
			doubleTable.removeSynonymValues(202, valuesToErase);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202, 203) == expectedSynonymValuesBC);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(202) == expectedSynonymValuesB);
			Assert::IsTrue(doubleTable.getSynonymValuesOf(203) == expectedSynonymValuesC);
		}

#pragma endregion

#pragma region Table Utilities

		TEST_METHOD(TableUtils_makeValuePairs)
		{
			VALUES firstValues = { 101, 102, 103 };
			VALUES secondValues = { 1, 2 };
			VALUE_PAIRS expectedValuePairs = { 
				{ 101, 1 }, { 102, 1 }, { 103, 1 }, 
				{ 101, 2 }, { 102, 2 }, { 103, 2 }
			};
			Assert::IsTrue(TableUtils::makeValuePairs(firstValues, secondValues) == expectedValuePairs);
		}

		TEST_METHOD(TableUtils_splitPairedValues)
		{
			VALUE_PAIRS valuePairs = {
				{ 101, 1 }, { 102, 1 }, { 103, 1 },
				{ 101, 2 }, { 102, 2 }, { 103, 2 }
			};
			VALUES expectedFirstValues = { 101, 102, 103 };
			VALUES expectedSecondValues = { 1, 2 };
			Assert::IsTrue(TableUtils::splitPairedValues(valuePairs) == PAIR<VALUES>(expectedFirstValues, expectedSecondValues));
		}

		TEST_METHOD(TableUtils_orderSynonyms)
		{
			PAIR<SYNONYM_INDEX> expectedOrder = { 101, 102 };
			Assert::IsTrue(TableUtils::orderSynonyms(101, 102) == expectedOrder);
			Assert::IsTrue(TableUtils::orderSynonyms(102, 101) == expectedOrder);
		}

		TEST_METHOD(TableUtils_getValueOfPairAt)
		{
			PAIR<VALUE> valuePair = { 3, 5 };
			Assert::IsTrue(TableUtils::getValueOfPairAt(valuePair, 0) == 3);
			Assert::IsTrue(TableUtils::getValueOfPairAt(valuePair, 1) == 5);
		}

#pragma endregion

	};
};
