#include <iostream>
#include "TNode.h"
#include "stdafx.h"
#include "CppUnitTest.h"
#include "MockAst.h"

using namespace std;
using namespace MockAst;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestTNode)
	{

	public:
		TEST_METHOD(isEquals)
		{
			VAR_NAMES vars = { "a", "b", "c" };
			VARS cons = { "1","2","3" };
			TNode test = createMockAssignNode(1, "1 1 + ", "test", vars, cons);
		}
	};
}