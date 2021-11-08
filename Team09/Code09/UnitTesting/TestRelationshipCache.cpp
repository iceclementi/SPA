#include "stdafx.h"
#include "TestRelationshipCache.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestRelationshipCache)
	{
	public:

		RelationshipCache cache = RelationshipCache(RELATIONSHIP_TYPE::NEXT_STAR);

		TEST_METHOD_INITIALIZE(cache_Init)
		{
			for (auto& valueGroup : nStarTableStub) {
				for (auto& value : valueGroup.second) {
					cache.addRelationship(valueGroup.first, value);
				}
			}
		}

		TEST_METHOD_CLEANUP(cache_Cleanup)
		{
			cache.clear();
		}

		TEST_METHOD(setPopulated_Pass)
		{
			cache.setPopulated(true);
			Assert::IsTrue(cache.isPopulated());
			cache.setPopulated(false);
			Assert::IsFalse(cache.isPopulated());
		}

		TEST_METHOD(getForwardTable_Pass)
		{
			Assert::IsTrue(cache.getForwardTable() == nStarTableStub);
		}

		TEST_METHOD(addRelationship_Pass)
		{
			cache.addRelationship(5, 6);
			Assert::IsTrue(cache.hasRelationship(5, 6));
		}

		TEST_METHOD(hasRelationship_Pass)
		{
			Assert::IsTrue(cache.hasRelationship(1, 2));
			Assert::IsTrue(cache.hasRelationship(2, 3));
			Assert::IsTrue(cache.hasRelationship(3, 4));
			Assert::IsTrue(cache.hasRelationship(4, 5));

			Assert::IsFalse(cache.hasRelationship(1, 0));
			Assert::IsFalse(cache.hasRelationship(2, 0));
			Assert::IsFalse(cache.hasRelationship(3, 0));
			Assert::IsFalse(cache.hasRelationship(4, 0));
			Assert::IsFalse(cache.hasRelationship(2, 1));
			Assert::IsFalse(cache.hasRelationship(5, 9));
			Assert::IsFalse(cache.hasRelationship(-1, 2));
			Assert::IsFalse(cache.hasRelationship(0, 0));
		}

		TEST_METHOD(hasRelationshipWithSecondValue_Pass)
		{
			Assert::IsTrue(cache.hasRelationshipWithSecondValue(2));
			Assert::IsTrue(cache.hasRelationshipWithSecondValue(3));
			Assert::IsTrue(cache.hasRelationshipWithSecondValue(4));
			Assert::IsTrue(cache.hasRelationshipWithSecondValue(5));
		}

		TEST_METHOD(hasRelationshipWithFirstValue_Pass)
		{
			Assert::IsTrue(cache.hasRelationshipWithFirstValue(1));
			Assert::IsTrue(cache.hasRelationshipWithFirstValue(2));
			Assert::IsTrue(cache.hasRelationshipWithFirstValue(3));
			Assert::IsTrue(cache.hasRelationshipWithFirstValue(4));
		}

		TEST_METHOD(clear_Pass)
		{
			cache.clear();
			Assert::IsTrue(cache.isEmpty());
		}
	};
}
