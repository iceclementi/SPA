#include "stdafx.h"
#include "CppUnitTest.h"
#include "SynonymTable.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(SynonymTableTest)
	{
	public:
	
		SynonymTable synonymTable;

		TEST_METHOD_INITIALIZE(SynonymTable_Init)
		{
			set<SYNONYM> stmts{ "1", "2", "3", "4"};
			set<SYNONYM> a_stmts{ "2", "3" };
			set<SYNONYM> r_stmts{ "4" };

			synonymTable.unionWith("s", stmts);
			synonymTable.unionWith("a", a_stmts);
			synonymTable.unionWith("r", r_stmts);
		}


		TEST_METHOD(containsSynonym_Pass)
		{

			Assert::IsTrue(synonymTable.containsSynonym("s"));
			Assert::IsTrue(synonymTable.containsSynonym("a"));
			Assert::IsTrue(synonymTable.containsSynonym("r"));
		}

		TEST_METHOD(containsSynonym_Fail)
		{
			Assert::IsFalse(synonymTable.containsSynonym("ss"));
			Assert::IsFalse(synonymTable.containsSynonym("d"));
		}


	};



}