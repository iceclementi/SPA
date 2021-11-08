#include "stdafx.h"
#include "Test_SynonymStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(Test_SynonymStorage)
	{
	public:
		SelectSynonymStorage selectStorage;
		TempSynonymStorage tempStorage;
		ResultSynonymStorage resultStorage;

		TEST_METHOD_INITIALIZE(SelectSynonymStorage_Init)
		{
			selectStorage = SelectSynonymStorage(synonymCounter, selectSynonyms);
			selectStorage.addSingleTable(MiniSingleTable(101, singleTableStub_a), 101);
			selectStorage.addSingleTable(MiniSingleTable(102, singleTableStub_b), 102);
			selectStorage.addSingleTable(MiniSingleTable(103, singleTableStub_c), 103);
			selectStorage.addSingleTable(MiniSingleTable(104, singleTableStub_d), 104);
			selectStorage.addSingleTable(MiniSingleTable(105, singleTableStub_e), 105);
			selectStorage.addSingleTable(MiniSingleTable(106, singleTableStub_f), 106);
			selectStorage.addSingleTable(MiniSingleTable(107, singleTableStub_g), 107);
			selectStorage.addDoubleTable(MiniDoubleTable(101, 102, doubleTableStub_ab), 101, 102);
			selectStorage.addDoubleTable(MiniDoubleTable(101, 103, doubleTableStub_ac), 101, 103);
			selectStorage.addDoubleTable(MiniDoubleTable(102, 103, doubleTableStub_bc), 102, 103);
			selectStorage.addDoubleTable(MiniDoubleTable(104, 105, doubleTableStub_de), 104, 105);

			tempStorage = TempSynonymStorage(synonymCounter);
			tempStorage.addSingleTable(MiniSingleTable(101, singleTableStub_a), 101);
			tempStorage.addSingleTable(MiniSingleTable(102, singleTableStub_b), 102);
			tempStorage.addSingleTable(MiniSingleTable(103, singleTableStub_c), 103);
			tempStorage.addSingleTable(MiniSingleTable(104, singleTableStub_d), 104);
			tempStorage.addSingleTable(MiniSingleTable(105, singleTableStub_e), 105);
			tempStorage.addSingleTable(MiniSingleTable(106, singleTableStub_f), 106);
			tempStorage.addSingleTable(MiniSingleTable(107, singleTableStub_g), 107);
			tempStorage.addDoubleTable(MiniDoubleTable(101, 102, doubleTableStub_ab), 101, 102);
			tempStorage.addDoubleTable(MiniDoubleTable(101, 103, doubleTableStub_ac), 101, 103);
			tempStorage.addDoubleTable(MiniDoubleTable(102, 103, doubleTableStub_bc), 102, 103);
			tempStorage.addDoubleTable(MiniDoubleTable(104, 105, doubleTableStub_de), 104, 105);

			resultStorage = ResultSynonymStorage(elementListExcess);
		}

		TEST_METHOD_CLEANUP(SelectSynonymStorage_CleanUp) {
			
		}

#pragma region Select Synonym Storage

		TEST_METHOD(SelectSynonymStorage_getSelectSynonyms)
		{
			Assert::IsTrue(selectStorage.getSelectSynonyms() == selectSynonyms);
		}

		TEST_METHOD(SelectSynonymStorage_getSingleTables)
		{
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
		}

		TEST_METHOD(SelectSynonymStorage_getDoubleTables)
		{
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_getSingleTableIndexOf)
		{
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(101), 1);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(102), 2);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(103), 3);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(104), 4);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(105), 5);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(106), 6);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(107), 7);
			Assert::AreEqual(selectStorage.getSingleTableIndexOf(108), 0);
		}

		TEST_METHOD(SelectSynonymStorage_getDoubleTableIndexOf)
		{
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(101) == INDICES({ 8, 9 }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(102) == INDICES({ 8, 10 }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(103) == INDICES({ 9, 10 }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(104) == INDICES({ 11 }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(105) == INDICES({ 11 }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(106) == INDICES({ }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(107) == INDICES({ }));
			Assert::IsTrue(selectStorage.getDoubleTableIndexOf(108) == INDICES({ }));

			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(101, 102)), 8);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(101, 103)), 9);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(102, 103)), 10);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(104, 105)), 11);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(102, 101)), 0);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(103, 104)), 0);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(101, 101)), 0);
			Assert::AreEqual(selectStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(105, 106)), 0);
		}

		TEST_METHOD(SelectSynonymStorage_containsSynonym)
		{
			Assert::IsTrue(selectStorage.containsSynonym(101));
			Assert::IsTrue(selectStorage.containsSynonym(102));
			Assert::IsTrue(selectStorage.containsSynonym(103));
			Assert::IsTrue(selectStorage.containsSynonym(104));
			Assert::IsTrue(selectStorage.containsSynonym(105));
			Assert::IsTrue(selectStorage.containsSynonym(106));
			Assert::IsTrue(selectStorage.containsSynonym(107));
			Assert::IsFalse(selectStorage.containsSynonym(108));
		}

		TEST_METHOD(SelectSynonymStorage_getSynonymValuesOf)
		{
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101) == singleTableStub_a);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108) == emptyValuesStub_ss);

			VALUE_PAIRS expectedSynonymValuesCA = { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 2, 3 } };
			VALUE_PAIRS expectedSynonymValuesEF = { { 124, 101 }, { 124, 102 }, { 125, 101 }, { 125, 102 } };

			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == doubleTableStub_de);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103, 101) == expectedSynonymValuesCA);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105, 106) == expectedSynonymValuesEF);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 109) == emptyValuePairsStub_ss);
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_union_single_normal)
		{
			VALUES synonymValuesH = { 113, 114, 115 };

			Assert::IsTrue(selectStorage.mergeWith(108, synonymValuesH));
			Assert::IsTrue(selectStorage.containsSynonym(108));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108) == synonymValuesH);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_union_single_remove)
		{
			VALUES synonymValuesJ = { 110, 111 };

			Assert::IsTrue(selectStorage.mergeWith(110, synonymValuesJ));
			Assert::IsFalse(selectStorage.containsSynonym(110));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_union_double_normal)
		{
			VALUE_PAIRS synonymValuesHI = { { 113, 116 }, { 114, 117 }, { 115, 118 } };
			VALUES expectedSynonymValuesH = { 113, 114, 115 };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };

			Assert::IsTrue(selectStorage.mergeWith(108, 109, synonymValuesHI));
			Assert::IsTrue(selectStorage.containsSynonym(108));
			Assert::IsTrue(selectStorage.containsSynonym(109));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108) == expectedSynonymValuesH);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108, 109) == synonymValuesHI);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(9));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(5));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_union_double_remove_1)
		{
			VALUE_PAIRS synonymValuesIJ = { { 116, 110 }, { 117, 111 }, { 118, 111 } };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };

			Assert::IsTrue(selectStorage.mergeWith(109, 110, synonymValuesIJ));
			Assert::IsTrue(selectStorage.containsSynonym(109));
			Assert::IsFalse(selectStorage.containsSynonym(110));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(109, 110) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_union_double_remove_2)
		{
			VALUE_PAIRS synonymValuesJK = { { 110, 120 }, { 111, 121 } };

			Assert::IsTrue(selectStorage.mergeWith(110, 111, synonymValuesJK));
			Assert::IsFalse(selectStorage.containsSynonym(110));
			Assert::IsFalse(selectStorage.containsSynonym(111));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(111) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(110, 111) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_single_connected)
		{
			VALUES intersectValuesA = { 2, 3, 4 };
			VALUES expectedSynonymValuesA = { 2, 3 };
			VALUES expectedSynonymValuesB = { 1, 3 };
			VALUES expectedSynonymValuesC = { 1, 2 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 2, 1 }, { 3, 1 }, { 3, 3 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 2, 1 }, { 3, 1 }, { 3, 2 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 3, 1 }, { 3, 2 } };

			Assert::IsTrue(selectStorage.mergeWith(101, intersectValuesA));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_single_notConnected)
		{
			VALUES intersectValuesF = { 101, 103, 105 };
			VALUES expectedSynonymValuesF = { 101 };

			Assert::IsTrue(selectStorage.mergeWith(106, intersectValuesF));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_single_selectSynonym)
		{
			VALUES intersectValuesB = { 1, 5 };
			VALUES expectedSynonymValuesA = { 1, 2, 3 };
			VALUES expectedSynonymValuesB = { 1 };
			VALUES expectedSynonymValuesC = { 1 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 1, 1 }, { 2, 1 }, { 3, 1 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 1, 1 }, { 2, 1 }, { 3, 1 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 } };

			Assert::IsTrue(selectStorage.mergeWith(102, intersectValuesB));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_single_remove)
		{
			VALUES intersectValuesE = { 124, 125, 126 };
			VALUES expectedValuesD = { 124, 125, 126 };

			Assert::IsTrue(selectStorage.mergeWith(105, intersectValuesE));
			Assert::IsFalse(selectStorage.containsSynonym(105));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_single_fail)
		{
			VALUES intersectValuesC = { 4, 5, 6 };

			Assert::IsFalse(selectStorage.mergeWith(103, intersectValuesC));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103) == emptyValuesStub_ss);
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_double_connected)
		{
			VALUE_PAIRS intersectValuesBA = { { 2, 1 }, { 3, 1 }, { 2, 2 }, { 1, 1} };
			VALUES expectedSynonymValuesA = { 1 };
			VALUES expectedSynonymValuesB = { 1, 2 };
			VALUES expectedSynonymValuesC = { 1 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 1, 1 }, { 1, 2 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 1, 1 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 2, 1 } };

			Assert::IsTrue(selectStorage.mergeWith(102, 101, intersectValuesBA));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_double_notConnected)
		{
			VALUE_PAIRS intersectValuesCD = { { 1, 124 }, { 2, 124 }, { 2, 125 }, { 3, 125} };
			VALUES expectedSynonymValuesA = { 1, 2, 3 };
			VALUES expectedSynonymValuesB = { 1, 2, 3 };
			VALUES expectedSynonymValuesC = { 1, 2 };
			VALUES expectedSynonymValuesD = { 124, 125 };
			VALUES expectedSynonymValuesE = { 124, 125 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 3, 1 }, { 3, 3 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 } };
			VALUE_PAIRS expectedSynonymValuesAD = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesAE = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 } };
			VALUE_PAIRS expectedSynonymValuesBD = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesBE = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesCD = { { 1, 124 }, { 2, 124 }, { 2, 125 } };
			VALUE_PAIRS expectedSynonymValuesCE = { { 1, 124 }, { 2, 124 }, { 2, 125 } };
			VALUE_PAIRS expectedSynonymValuesDE = { { 124, 124 }, { 125, 124 }, { 125, 125 } };

			Assert::IsTrue(selectStorage.mergeWith(103, 104, intersectValuesCD));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == expectedSynonymValuesE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 104) == expectedSynonymValuesAD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(101, 105) == expectedSynonymValuesAE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 104) == expectedSynonymValuesBD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 105) == expectedSynonymValuesBE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103, 104) == expectedSynonymValuesCD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103, 105) == expectedSynonymValuesCE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == expectedSynonymValuesDE);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(10));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_double_connected_remove)
		{
			VALUE_PAIRS intersectValuesDE = { { 124, 124 }, { 125, 125 }, { 126, 126 } };
			VALUES expectedSynonymValuesD = { 124, 125 };

			Assert::IsTrue(selectStorage.mergeWith(104, 105, intersectValuesDE));
			Assert::IsFalse(selectStorage.containsSynonym(105));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_double_notConnected_remove)
		{
			VALUE_PAIRS intersectValuesEF = { { 126, 101 }, { 125, 102 }, { 124, 103 }, { 126, 102 } };
			VALUES expectedSynonymValuesD = { 125, 126 };
			VALUES expectedSynonymValuesF = { 102 };

			Assert::IsTrue(selectStorage.mergeWith(105, 106, intersectValuesEF));
			Assert::IsFalse(selectStorage.containsSynonym(105));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_double_connected_fail)
		{
			VALUE_PAIRS intersectValuesCB = { { 1, 4 }, { 2, 2 }, { 3, 1 } };

			Assert::IsFalse(selectStorage.mergeWith(103, 102, intersectValuesCB));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102) == singleTableStub_b);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(103) == singleTableStub_c);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102, 103) == emptyValuePairsStub_ss);
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersect_double_notConnected_fail)
		{
			VALUE_PAIRS intersectValuesBD = { { 3, 123 }, { 4, 124 } };
			VALUES expectedSynonymValuesB = { 3 };
			VALUES expectedSynonymValuesD = { 124 };

			Assert::IsFalse(selectStorage.mergeWith(102, 104, intersectValuesBD));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_connected_normal)
		{
			VALUE_PAIRS intersectUnionValuesDH = { { 125, 116 }, { 126, 117 }, { 122, 118 }};
			VALUES expectedSynonymValuesD = { 125, 126 };
			VALUES expectedSynonymValuesE = { 124, 125 };
			VALUES expectedSynonymValuesH = { 116, 117 };
			VALUE_PAIRS expectedSynonymValuesDE = { { 125, 124 }, { 125, 125 }, { 126, 125 } };
			VALUE_PAIRS expectedSynonymValuesDH = { { 125, 116 }, { 126, 117 } };
			VALUE_PAIRS expectedSynonymValuesEH = { { 124, 116 }, { 125, 116 }, { 125, 117 } };

			Assert::IsTrue(selectStorage.mergeWith(104, 108, intersectUnionValuesDH));
			Assert::IsTrue(selectStorage.containsSynonym(108));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == expectedSynonymValuesE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108) == expectedSynonymValuesH);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == expectedSynonymValuesDE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 108) == expectedSynonymValuesDH);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105, 108) == expectedSynonymValuesEH);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(6));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_connected_remove_union)
		{
			VALUE_PAIRS intersectUnionValuesDJ = { { 124, 116 }, { 126, 117 }, { 122, 118 } };
			VALUES expectedSynonymValuesD = { 124, 126 };
			VALUES expectedSynonymValuesE = { 124, 125 };
			VALUE_PAIRS expectedSynonymValuesDE = { { 124, 124 }, { 126, 125 } };

			Assert::IsTrue(selectStorage.mergeWith(104, 110, intersectUnionValuesDJ));
			Assert::IsFalse(selectStorage.containsSynonym(110));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == expectedSynonymValuesE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == expectedSynonymValuesDE);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 110) == emptyValuePairsStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105, 110) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_connected_remove_intersect)
		{
			VALUE_PAIRS intersectUnionValuesEI = { { 124, 116 }, { 125, 117 }, { 125, 118 }, { 126, 119 } };
			VALUES expectedSynonymValuesD = { 124, 125, 126 };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };
			VALUE_PAIRS expectedSynonymValuesDI = { { 124, 116 }, { 125, 116 }, { 125, 117 }, { 125, 118 }, { 126, 117 }, { 126, 118 } };

			Assert::IsTrue(selectStorage.mergeWith(105, 109, intersectUnionValuesEI));
			Assert::IsFalse(selectStorage.containsSynonym(105));
			Assert::IsTrue(selectStorage.containsSynonym(109));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 109) == expectedSynonymValuesDI);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105, 109) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_connected_remove_both)
		{
			VALUE_PAIRS intersectUnionValuesEK = { { 125, 117 }, { 125, 118 }, { 126, 119 } };
			VALUES expectedSynonymValuesD = { 125, 126 };

			Assert::IsTrue(selectStorage.mergeWith(105, 111, intersectUnionValuesEK));
			Assert::IsFalse(selectStorage.containsSynonym(105));
			Assert::IsFalse(selectStorage.containsSynonym(111));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(111) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(104, 111) == emptyValuePairsStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105, 111) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_connected_fail)
		{
			VALUE_PAIRS intersectUnionValuesEH = { { 120, 116 }, { 121, 118 }, { 122, 118 } };

			Assert::IsFalse(selectStorage.mergeWith(105, 108, intersectUnionValuesEH));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(105, 108) == emptyValuePairsStub_ss);
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_notConnected_normal)
		{
			VALUE_PAIRS intersectUnionValuesFH = { { 101, 116 }, { 102, 117 }, { 102, 118 }, { 103, 119 } };
			VALUES expectedSynonymValuesF = { 101, 102 };
			VALUES expectedSynonymValuesH = { 116, 117, 118 };
			VALUE_PAIRS expectedSynonymValuesFH = { { 101, 116 }, { 102, 117 }, { 102, 118 } };

			Assert::IsTrue(selectStorage.mergeWith(106, 108, intersectUnionValuesFH));
			Assert::IsTrue(selectStorage.containsSynonym(108));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(108) == expectedSynonymValuesH);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106, 108) == expectedSynonymValuesFH);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(5));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_notConnected_remove_union)
		{
			VALUE_PAIRS intersectUnionValuesFJ = { { 101, 116 }, { 101, 117 }, { 101, 118 } };
			VALUES expectedSynonymValuesF = { 101 };

			Assert::IsTrue(selectStorage.mergeWith(106, 110, intersectUnionValuesFJ));
			Assert::IsFalse(selectStorage.containsSynonym(110));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106, 110) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_notConnected_remove_intersect)
		{
			VALUE_PAIRS intersectUnionValuesGI = { { 102, 116 }, { 103, 116 }, { 103, 117 }, { 104, 118 } };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };

			Assert::IsTrue(selectStorage.mergeWith(107, 109, intersectUnionValuesGI));
			Assert::IsFalse(selectStorage.containsSynonym(107));
			Assert::IsTrue(selectStorage.containsSynonym(109));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(107) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(107, 109) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_notConnected_remove_both)
		{
			VALUE_PAIRS intersectUnionValuesGK = { { 103, 117 }, { 103, 118 }, { 102, 119 } };

			Assert::IsTrue(selectStorage.mergeWith(107, 111, intersectUnionValuesGK));
			Assert::IsFalse(selectStorage.containsSynonym(107));
			Assert::IsFalse(selectStorage.containsSynonym(111));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(107) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(111) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(107, 111) == emptyValuePairsStub_ss);
			Assert::AreEqual(selectStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(selectStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(SelectSynonymStorage_mergeWith_intersectUnion_notConnected_fail)
		{
			VALUE_PAIRS intersectUnionValuesFI = { { 103, 116 }, { 103, 118 }, { 104, 118 } };

			Assert::IsFalse(selectStorage.mergeWith(106, 109, intersectUnionValuesFI));
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(109) == emptyValuesStub_ss);
			Assert::IsTrue(selectStorage.getSynonymValuesOf(106, 109) == emptyValuePairsStub_ss);
		}

#pragma endregion

#pragma region Temporary Synonym Storage

		TEST_METHOD(TempSynonymStorage_getSingleTables)
		{
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
		}

		TEST_METHOD(TempSynonymStorage_getDoubleTables)
		{
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_getSingleTableIndexOf)
		{
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(101), 1);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(102), 2);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(103), 3);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(104), 4);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(105), 5);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(106), 6);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(107), 7);
			Assert::AreEqual(tempStorage.getSingleTableIndexOf(108), 0);
		}

		TEST_METHOD(TempSynonymStorage_getDoubleTableIndexOf)
		{
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(101) == INDICES({ 8, 9 }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(102) == INDICES({ 8, 10 }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(103) == INDICES({ 9, 10 }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(104) == INDICES({ 11 }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(105) == INDICES({ 11 }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(106) == INDICES({ }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(107) == INDICES({ }));
			Assert::IsTrue(tempStorage.getDoubleTableIndexOf(108) == INDICES({ }));

			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(101, 102)), 8);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(101, 103)), 9);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(102, 103)), 10);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(104, 105)), 11);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(102, 101)), 0);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(103, 104)), 0);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(101, 101)), 0);
			Assert::AreEqual(tempStorage.getDoubleTableIndexOf(PAIR<SYNONYM_INDEX>(105, 106)), 0);
		}

		TEST_METHOD(TempSynonymStorage_containsSynonym)
		{
			Assert::IsTrue(tempStorage.containsSynonym(101));
			Assert::IsTrue(tempStorage.containsSynonym(102));
			Assert::IsTrue(tempStorage.containsSynonym(103));
			Assert::IsTrue(tempStorage.containsSynonym(104));
			Assert::IsTrue(tempStorage.containsSynonym(105));
			Assert::IsTrue(tempStorage.containsSynonym(106));
			Assert::IsTrue(tempStorage.containsSynonym(107));
			Assert::IsFalse(tempStorage.containsSynonym(108));
		}

		TEST_METHOD(TempSynonymStorage_getSynonymValuesOf)
		{
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101) == singleTableStub_a);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108) == emptyValuesStub_ss);

			VALUE_PAIRS expectedSynonymValuesCA = { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 2, 3 } };
			VALUE_PAIRS expectedSynonymValuesEF = { { 124, 101 }, { 124, 102 }, { 125, 101 }, { 125, 102 } };

			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == doubleTableStub_de);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103, 101) == expectedSynonymValuesCA);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105, 106) == expectedSynonymValuesEF);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 109) == emptyValuePairsStub_ss);
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_union_single_normal)
		{
			VALUES synonymValuesH = { 113, 114, 115 };

			Assert::IsTrue(tempStorage.mergeWith(108, synonymValuesH));
			Assert::IsTrue(tempStorage.containsSynonym(108));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108) == synonymValuesH);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_union_single_remove)
		{
			VALUES synonymValuesJ = { 110, 111 };

			Assert::IsTrue(tempStorage.mergeWith(110, synonymValuesJ));
			Assert::IsFalse(tempStorage.containsSynonym(110));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_union_double_normal)
		{
			VALUE_PAIRS synonymValuesHI = { { 113, 116 }, { 114, 117 }, { 115, 118 } };
			VALUES expectedSynonymValuesH = { 113, 114, 115 };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };

			Assert::IsTrue(tempStorage.mergeWith(108, 109, synonymValuesHI));
			Assert::IsTrue(tempStorage.containsSynonym(108));
			Assert::IsTrue(tempStorage.containsSynonym(109));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108) == expectedSynonymValuesH);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108, 109) == synonymValuesHI);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(9));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(5));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_union_double_remove_1)
		{
			VALUE_PAIRS synonymValuesIJ = { { 116, 110 }, { 117, 111 }, { 118, 111 } };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };

			Assert::IsTrue(tempStorage.mergeWith(109, 110, synonymValuesIJ));
			Assert::IsTrue(tempStorage.containsSynonym(109));
			Assert::IsFalse(tempStorage.containsSynonym(110));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(109, 110) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_union_double_remove_2)
		{
			VALUE_PAIRS synonymValuesJK = { { 110, 120 }, { 111, 121 } };

			Assert::IsTrue(tempStorage.mergeWith(110, 111, synonymValuesJK));
			Assert::IsFalse(tempStorage.containsSynonym(110));
			Assert::IsFalse(tempStorage.containsSynonym(111));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(111) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(110, 111) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_single_connected)
		{
			VALUES intersectValuesA = { 2, 3, 4 };
			VALUES expectedSynonymValuesA = { 2, 3 };
			VALUES expectedSynonymValuesB = { 1, 3 };
			VALUES expectedSynonymValuesC = { 1, 2 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 2, 1 }, { 3, 1 }, { 3, 3 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 2, 1 }, { 3, 1 }, { 3, 2 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 3, 1 }, { 3, 2 } };

			Assert::IsTrue(tempStorage.mergeWith(101, intersectValuesA));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_single_notConnected)
		{
			VALUES intersectValuesF = { 101, 103, 105 };
			VALUES expectedSynonymValuesF = { 101 };

			Assert::IsTrue(tempStorage.mergeWith(106, intersectValuesF));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_single_remove)
		{
			VALUES intersectValuesE = { 124, 125, 126 };
			VALUES expectedValuesD = { 124, 125, 126 };

			Assert::IsTrue(tempStorage.mergeWith(105, intersectValuesE));
			Assert::IsFalse(tempStorage.containsSynonym(105));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_single_fail)
		{
			VALUES intersectValuesC = { 4, 5, 6 };

			Assert::IsFalse(tempStorage.mergeWith(103, intersectValuesC));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103) == emptyValuesStub_ss);
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_double_connected)
		{
			VALUE_PAIRS intersectValuesCA = { { 2, 1 }, { 3, 1 }, { 2, 2 }, { 1, 1} };
			VALUES expectedSynonymValuesA = { 1 };
			VALUES expectedSynonymValuesB = { 1, 2 };
			VALUES expectedSynonymValuesC = { 1 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 1, 1 }, { 1, 2 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 1, 1 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 2, 1 } };

			Assert::IsTrue(tempStorage.mergeWith(103, 101, intersectValuesCA));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_double_notConnected)
		{
			VALUE_PAIRS intersectValuesCD = { { 1, 124 }, { 2, 124 }, { 2, 125 }, { 3, 125} };
			VALUES expectedSynonymValuesA = { 1, 2, 3 };
			VALUES expectedSynonymValuesB = { 1, 2, 3 };
			VALUES expectedSynonymValuesC = { 1, 2 };
			VALUES expectedSynonymValuesD = { 124, 125 };
			VALUES expectedSynonymValuesE = { 124, 125 };
			VALUE_PAIRS expectedSynonymValuesAB = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 3, 1 }, { 3, 3 } };
			VALUE_PAIRS expectedSynonymValuesAC = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 } };
			VALUE_PAIRS expectedSynonymValuesAD = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesAE = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesBC = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 } };
			VALUE_PAIRS expectedSynonymValuesBD = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesBE = { { 1, 124 }, { 2, 124 }, { 3, 124 }, { 3, 125 } };
			VALUE_PAIRS expectedSynonymValuesCD = { { 1, 124 }, { 2, 124 }, { 2, 125 } };
			VALUE_PAIRS expectedSynonymValuesCE = { { 1, 124 }, { 2, 124 }, { 2, 125 } };
			VALUE_PAIRS expectedSynonymValuesDE = { { 124, 124 }, { 125, 124 }, { 125, 125 } };

			Assert::IsTrue(tempStorage.mergeWith(103, 104, intersectValuesCD));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101) == expectedSynonymValuesA);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103) == expectedSynonymValuesC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == expectedSynonymValuesE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 102) == expectedSynonymValuesAB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 103) == expectedSynonymValuesAC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 104) == expectedSynonymValuesAD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(101, 105) == expectedSynonymValuesAE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102, 103) == expectedSynonymValuesBC);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102, 104) == expectedSynonymValuesBD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102, 105) == expectedSynonymValuesBE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103, 104) == expectedSynonymValuesCD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103, 105) == expectedSynonymValuesCE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == expectedSynonymValuesDE);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(10));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_double_connected_remove)
		{
			VALUE_PAIRS intersectValuesDE = { { 124, 124 }, { 125, 125 }, { 126, 126 } };
			VALUES expectedSynonymValuesD = { 124, 125 };

			Assert::IsTrue(tempStorage.mergeWith(104, 105, intersectValuesDE));
			Assert::IsFalse(tempStorage.containsSynonym(105));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_double_notConnected_remove)
		{
			VALUE_PAIRS intersectValuesEF = { { 126, 101 }, { 125, 102 }, { 124, 103 }, { 126, 102 } };
			VALUES expectedSynonymValuesD = { 125, 126 };
			VALUES expectedSynonymValuesF = { 102 };

			Assert::IsTrue(tempStorage.mergeWith(105, 106, intersectValuesEF));
			Assert::IsFalse(tempStorage.containsSynonym(105));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_double_connected_fail)
		{
			VALUE_PAIRS intersectValuesCB = { { 1, 4 }, { 2, 2 }, { 3, 1 } };

			Assert::IsFalse(tempStorage.mergeWith(103, 102, intersectValuesCB));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102) == singleTableStub_b);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(103) == singleTableStub_c);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102, 103) == emptyValuePairsStub_ss);
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersect_double_notConnected_fail)
		{
			VALUE_PAIRS intersectValuesBD = { { 3, 123 }, { 4, 124 } };
			VALUES expectedSynonymValuesB = { 3 };
			VALUES expectedSynonymValuesD = { 124 };

			Assert::IsFalse(tempStorage.mergeWith(102, 104, intersectValuesBD));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(102) == expectedSynonymValuesB);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_connected_normal)
		{
			VALUE_PAIRS intersectUnionValuesDH = { { 125, 116 }, { 126, 117 }, { 122, 118 } };
			VALUES expectedSynonymValuesD = { 125, 126 };
			VALUES expectedSynonymValuesE = { 124, 125 };
			VALUES expectedSynonymValuesH = { 116, 117 };
			VALUE_PAIRS expectedSynonymValuesDE = { { 125, 124 }, { 125, 125 }, { 126, 125 } };
			VALUE_PAIRS expectedSynonymValuesDH = { { 125, 116 }, { 126, 117 } };
			VALUE_PAIRS expectedSynonymValuesEH = { { 124, 116 }, { 125, 116 }, { 125, 117 } };

			Assert::IsTrue(tempStorage.mergeWith(104, 108, intersectUnionValuesDH));
			Assert::IsTrue(tempStorage.containsSynonym(108));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == expectedSynonymValuesE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108) == expectedSynonymValuesH);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == expectedSynonymValuesDE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 108) == expectedSynonymValuesDH);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105, 108) == expectedSynonymValuesEH);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(6));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_connected_remove_union)
		{
			VALUE_PAIRS intersectUnionValuesDJ = { { 124, 116 }, { 126, 117 }, { 122, 118 } };
			VALUES expectedSynonymValuesD = { 124, 126 };
			VALUES expectedSynonymValuesE = { 124, 125 };
			VALUE_PAIRS expectedSynonymValuesDE = { { 124, 124 }, { 126, 125 } };

			Assert::IsTrue(tempStorage.mergeWith(104, 110, intersectUnionValuesDJ));
			Assert::IsFalse(tempStorage.containsSynonym(110));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == expectedSynonymValuesE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == expectedSynonymValuesDE);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 110) == emptyValuePairsStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105, 110) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_connected_remove_intersect)
		{
			VALUE_PAIRS intersectUnionValuesEI = { { 124, 116 }, { 125, 117 }, { 125, 118 }, { 126, 119 } };
			VALUES expectedSynonymValuesD = { 124, 125, 126 };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };
			VALUE_PAIRS expectedSynonymValuesDI = { { 124, 116 }, { 125, 116 }, { 125, 117 }, { 125, 118 }, { 126, 117 }, { 126, 118 } };

			Assert::IsTrue(tempStorage.mergeWith(105, 109, intersectUnionValuesEI));
			Assert::IsFalse(tempStorage.containsSynonym(105));
			Assert::IsTrue(tempStorage.containsSynonym(109));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 109) == expectedSynonymValuesDI);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105, 109) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_connected_remove_both)
		{
			VALUE_PAIRS intersectUnionValuesEK = { { 125, 117 }, { 125, 118 }, { 126, 119 } };
			VALUES expectedSynonymValuesD = { 125, 126 };

			Assert::IsTrue(tempStorage.mergeWith(105, 111, intersectUnionValuesEK));
			Assert::IsFalse(tempStorage.containsSynonym(105));
			Assert::IsFalse(tempStorage.containsSynonym(111));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104) == expectedSynonymValuesD);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(111) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 105) == emptyValuePairsStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(104, 111) == emptyValuePairsStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105, 111) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(3));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_connected_fail)
		{
			VALUE_PAIRS intersectUnionValuesEH = { { 120, 116 }, { 121, 118 }, { 122, 118 } };

			Assert::IsFalse(tempStorage.mergeWith(105, 108, intersectUnionValuesEH));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(105, 108) == emptyValuePairsStub_ss);
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_notConnected_normal)
		{
			VALUE_PAIRS intersectUnionValuesFH = { { 101, 116 }, { 102, 117 }, { 102, 118 }, { 103, 119 } };
			VALUES expectedSynonymValuesF = { 101, 102 };
			VALUES expectedSynonymValuesH = { 116, 117, 118 };
			VALUE_PAIRS expectedSynonymValuesFH = { { 101, 116 }, { 102, 117 }, { 102, 118 } };

			Assert::IsTrue(tempStorage.mergeWith(106, 108, intersectUnionValuesFH));
			Assert::IsTrue(tempStorage.containsSynonym(108));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(108) == expectedSynonymValuesH);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106, 108) == expectedSynonymValuesFH);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(8));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(5));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_notConnected_remove_union)
		{
			VALUE_PAIRS intersectUnionValuesFJ = { { 101, 116 }, { 101, 117 }, { 101, 118 } };
			VALUES expectedSynonymValuesF = { 101 };

			Assert::IsTrue(tempStorage.mergeWith(106, 110, intersectUnionValuesFJ));
			Assert::IsFalse(tempStorage.containsSynonym(110));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106) == expectedSynonymValuesF);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(110) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106, 110) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_notConnected_remove_intersect)
		{
			VALUE_PAIRS intersectUnionValuesGI = { { 102, 116 }, { 103, 116 }, { 103, 117 }, { 104, 118 } };
			VALUES expectedSynonymValuesI = { 116, 117, 118 };

			Assert::IsTrue(tempStorage.mergeWith(107, 109, intersectUnionValuesGI));
			Assert::IsFalse(tempStorage.containsSynonym(107));
			Assert::IsTrue(tempStorage.containsSynonym(109));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(107) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(109) == expectedSynonymValuesI);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(107, 109) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_notConnected_remove_both)
		{
			VALUE_PAIRS intersectUnionValuesGK = { { 103, 117 }, { 103, 118 }, { 102, 119 } };

			Assert::IsTrue(tempStorage.mergeWith(107, 111, intersectUnionValuesGK));
			Assert::IsFalse(tempStorage.containsSynonym(107));
			Assert::IsFalse(tempStorage.containsSynonym(111));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(107) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(111) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(107, 111) == emptyValuePairsStub_ss);
			Assert::AreEqual(tempStorage.getSingleTables().size(), size_t(6));
			Assert::AreEqual(tempStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(TempSynonymStorage_mergeWith_intersectUnion_notConnected_fail)
		{
			VALUE_PAIRS intersectUnionValuesFI = { { 103, 116 }, { 103, 118 }, { 104, 118 } };

			Assert::IsFalse(tempStorage.mergeWith(106, 109, intersectUnionValuesFI));
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(109) == emptyValuesStub_ss);
			Assert::IsTrue(tempStorage.getSynonymValuesOf(106, 109) == emptyValuePairsStub_ss);
		}

#pragma endregion

#pragma region Result Synonym Storage

		TEST_METHOD(ResultSynonymStorage_mergeTable)
		{
			resultStorage.mergeTable(selectStorage);

			Assert::IsTrue(resultStorage.containsSynonym(101));
			Assert::IsTrue(resultStorage.containsSynonym(102));
			Assert::IsTrue(resultStorage.containsSynonym(103));
			Assert::IsTrue(resultStorage.containsSynonym(104));
			Assert::IsTrue(resultStorage.containsSynonym(105));
			Assert::IsTrue(resultStorage.containsSynonym(106));
			Assert::IsTrue(resultStorage.containsSynonym(107));

			Assert::IsTrue(resultStorage.getSynonymValuesOf(101) == singleTableStub_a);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(102) == singleTableStub_b);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(103) == singleTableStub_c);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(104) == singleTableStub_d);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(105) == singleTableStub_e);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(106) == singleTableStub_f);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(107) == singleTableStub_g);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(101, 102) == doubleTableStub_ab);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(101, 103) == doubleTableStub_ac);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(102, 103) == doubleTableStub_bc);
			Assert::IsTrue(resultStorage.getSynonymValuesOf(104, 105) == doubleTableStub_de);

			Assert::AreEqual(resultStorage.getSingleTables().size(), size_t(7));
			Assert::AreEqual(resultStorage.getDoubleTables().size(), size_t(4));
		}

		TEST_METHOD(ResultSynonymStorage_getMissingSelectSynonyms)
		{
			resultStorage.mergeTable(selectStorage);
			SYNONYM_INDICES expectedMissingSynonyms({ 108, 113 });
			Assert::IsTrue(resultStorage.getMissingSelectSynonyms() == expectedMissingSynonyms);
		}

		TEST_METHOD(ResultSynonymStorage_getIntermediateAnswer_connected)
		{
			resultStorage = ResultSynonymStorage(elementListConnected);
			resultStorage.mergeTable(selectStorage);

			INTERMEDIATE_ANSWER expectedIntermediateAnswer = {
				{ 1, 1, 1 },
				{ 1, 1, 2 },
				{ 2, 1, 1 },
				{ 3, 1, 1 },
				{ 3, 1, 3 },
				{ 3, 2, 3 }
			};
			INTERMEDIATE_ANSWER intermediateAnswer = resultStorage.getIntermediateAnswer();
			sort(expectedIntermediateAnswer.begin(), expectedIntermediateAnswer.end());
			sort(intermediateAnswer.begin(), intermediateAnswer.end());

			Assert::IsTrue(intermediateAnswer == expectedIntermediateAnswer);
		}

		TEST_METHOD(ResultSynonymStorage_getIntermediateAnswer_connected_duplicate)
		{
			resultStorage = ResultSynonymStorage(elementListConnectedDuplicate);
			resultStorage.mergeTable(selectStorage);

			INTERMEDIATE_ANSWER expectedIntermediateAnswer = {
				{ 1, 1, 1, 1, 1 },
				{ 1, 2, 1, 1, 1 },
				{ 2, 1, 2, 1, 1 },
				{ 3, 1, 3, 1, 1 },
				{ 3, 3, 3, 1, 1 },
				{ 3, 3, 3, 2, 2 }
			};
			INTERMEDIATE_ANSWER intermediateAnswer = resultStorage.getIntermediateAnswer();
			sort(expectedIntermediateAnswer.begin(), expectedIntermediateAnswer.end());
			sort(intermediateAnswer.begin(), intermediateAnswer.end());

			Assert::IsTrue(intermediateAnswer == expectedIntermediateAnswer);
		}

		TEST_METHOD(ResultSynonymStorage_getIntermediateAnswer_notConnected)
		{
			resultStorage = ResultSynonymStorage(elementListNotConnected);
			resultStorage.mergeTable(selectStorage);

			INTERMEDIATE_ANSWER expectedIntermediateAnswer = {
				{ 1, 124, 124, 103 }, { 1, 124, 124, 104 },
				{ 1, 125, 124, 103 }, { 1, 125, 124, 104 },
				{ 1, 125, 125, 103 }, { 1, 125, 125, 104 },
				{ 1, 126, 125, 103 }, { 1, 126, 125, 104 },
				{ 2, 124, 124, 103 }, { 2, 124, 124, 104 },
				{ 2, 125, 124, 103 }, { 2, 125, 124, 104 },
				{ 2, 125, 125, 103 }, { 2, 125, 125, 104 },
				{ 2, 126, 125, 103 }, { 2, 126, 125, 104 },
				{ 3, 124, 124, 103 }, { 3, 124, 124, 104 },
				{ 3, 125, 124, 103 }, { 3, 125, 124, 104 },
				{ 3, 125, 125, 103 }, { 3, 125, 125, 104 },
				{ 3, 126, 125, 103 }, { 3, 126, 125, 104 }
			};
			INTERMEDIATE_ANSWER intermediateAnswer = resultStorage.getIntermediateAnswer();
			sort(expectedIntermediateAnswer.begin(), expectedIntermediateAnswer.end());
			sort(intermediateAnswer.begin(), intermediateAnswer.end());

			Assert::IsTrue(intermediateAnswer == expectedIntermediateAnswer);
		}

		TEST_METHOD(ResultSynonymStorage_getIntermediateAnswer_notConnected_duplicate)
		{
			resultStorage = ResultSynonymStorage(elementListNotConnectedDuplicate);
			resultStorage.mergeTable(selectStorage);

			INTERMEDIATE_ANSWER expectedIntermediateAnswer = {
				{ 1, 101, 1, 1, 103, 1 }, { 1, 101, 1, 1, 104, 1 },
				{ 1, 101, 2, 1, 103, 1 }, { 1, 101, 2, 1, 104, 1 },
				{ 1, 102, 1, 1, 103, 1 }, { 1, 102, 1, 1, 104, 1 },
				{ 1, 102, 2, 1, 103, 1 }, { 1, 102, 2, 1, 104, 1 },
				{ 2, 101, 1, 2, 103, 1 }, { 2, 101, 1, 2, 104, 1 },
				{ 2, 102, 1, 2, 103, 1 }, { 2, 102, 1, 2, 104, 1 },
				{ 3, 101, 1, 3, 103, 1 }, { 3, 101, 1, 3, 104, 1 },
				{ 3, 102, 1, 3, 103, 1 }, { 3, 102, 1, 3, 104, 1 },
				{ 3, 101, 3, 3, 103, 1 }, { 3, 101, 3, 3, 103, 2 },
				{ 3, 102, 3, 3, 103, 1 }, { 3, 102, 3, 3, 103, 2 },
				{ 3, 101, 3, 3, 104, 1 }, { 3, 101, 3, 3, 104, 2 },
				{ 3, 102, 3, 3, 104, 1 }, { 3, 102, 3, 3, 104, 2 },
			};
			INTERMEDIATE_ANSWER intermediateAnswer = resultStorage.getIntermediateAnswer();
			sort(expectedIntermediateAnswer.begin(), expectedIntermediateAnswer.end());
			sort(intermediateAnswer.begin(), intermediateAnswer.end());

			Assert::IsTrue(intermediateAnswer == expectedIntermediateAnswer);
		}

#pragma endregion

	};
};
