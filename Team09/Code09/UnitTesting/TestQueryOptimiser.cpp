#include "stdafx.h"
#include "TestQueryOptimiser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryOptimiser)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;
		
		TEST_METHOD_INITIALIZE(QueryOptimiser_Init)
		{
			
			
		}

#pragma region QueryOptimiser


		TEST_METHOD(optimiserQueryObject_constantQueryGroup_Pass_1)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("3");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS_T;

			SuchThatClause stc2;
			stc2.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc2.firstArg.setArgStr("1");
			stc2.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc2.secondArg.setArgStr("3");
			stc2.relType = SUCH_THAT_REL_TYPE::FOLLOWS;
			
			SuchThatClause stc3;
			stc3.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc3.firstArg.setArgStr("1");
			stc3.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc3.secondArg.setArgStr("3");
			stc3.relType = SUCH_THAT_REL_TYPE::NEXT;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject.addSuchThatClause(stc2, 2);
			queryObject.addSuchThatClause(stc3, 3);

			QueryObject optimisedQueryObject = QueryOptimiser(queryObject).optimiseQuery();
	
			CLAUSE_INDICES clauseGroup = optimisedQueryObject.getConstantQueryGroups()[0].getClauses();
			CLAUSE_INDICES optimisedClauseGroup = { 2, 3, 1};

			Assert::IsTrue(clauseGroup== optimisedClauseGroup);
		}

		TEST_METHOD(optimiserQueryObject_constantQueryGroup_Pass_2)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("3");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS_T;

			SuchThatClause stc2;
			stc2.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc2.firstArg.setArgStr("1");
			stc2.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc2.secondArg.setArgStr("3");
			stc2.relType = SUCH_THAT_REL_TYPE::FOLLOWS;

			SuchThatClause stc3;
			stc3.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc3.firstArg.setArgStr("1");
			stc3.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc3.secondArg.setArgStr("3");
			stc3.relType = SUCH_THAT_REL_TYPE::NEXT;

			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			WithClause wc(arg1, arg2);

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject.addSuchThatClause(stc2, 2);
			queryObject.addSuchThatClause(stc3, 3);
			queryObject.addWithClause(wc, 4);

			QueryObject optimisedQueryObject = QueryOptimiser(queryObject).optimiseQuery();

			CLAUSE_INDICES clauseGroup = optimisedQueryObject.getConstantQueryGroups()[0].getClauses();
			CLAUSE_INDICES optimisedClauseGroup = { 4, 2, 3, 1 };

			Assert::IsTrue(clauseGroup == optimisedClauseGroup);
		}

		TEST_METHOD(optimiserQueryObject_selectQueryGroup_Pass_1)
		{
			QuerySelectResult qsr;
			QueryElement elem = QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::INTEGER);
			qsr.addToSelectElementList(elem);
			qsr.addSelectSynonym(elem.getSynIndex());
			// 1 syn index = s1


			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc.firstArg.setArgStr("s1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc.secondArg.setArgStr("s2");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS_T;
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			SuchThatClause stc2;
			stc2.firstArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc2.firstArg.setArgStr("s1");
			stc2.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc2.secondArg.setArgStr("3");
			stc2.relType = SUCH_THAT_REL_TYPE::FOLLOWS;
			stc2.addClauseSynonym(1);

			SuchThatClause stc3;
			stc3.firstArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc3.firstArg.setArgStr("s1");
			stc3.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc3.secondArg.setArgStr("3");
			stc3.relType = SUCH_THAT_REL_TYPE::NEXT;
			stc3.addClauseSynonym(1);

			/*QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 2, argument, { PATTERN_MATCH_TYPE::UNRESTRICTED, "" });
			pc.addClauseSynonym(2);*/

			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);
		

			QueryObject queryObject;
			queryObject.setSelectResult(qsr);
			queryObject.addSuchThatClause(stc, 1);
			queryObject.addSuchThatClause(stc2, 2);
			queryObject.addSuchThatClause(stc3, 3);
			queryObject.addWithClause(wc, 4);
			

			QueryObject optimisedQueryObject = QueryOptimiser(queryObject).optimiseQuery();

			CLAUSE_INDICES clauseGroup = optimisedQueryObject.getSelectQueryGroups()[0].getClauses();
			CLAUSE_INDICES optimisedClauseGroup = { 4, 2, 3, 1 };

			Assert::IsTrue(clauseGroup == optimisedClauseGroup);
		}

		TEST_METHOD(optimiserQueryObject_selectQueryGroup_Pass_2)
		{
			QuerySelectResult qsr;
			QueryElement elem = QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::INTEGER);
			qsr.addToSelectElementList(elem);
			qsr.addSelectSynonym(elem.getSynIndex());
			// s1 = 1
			// s2 = 2
			// a = 3

			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc.firstArg.setArgStr("s1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc.secondArg.setArgStr("s2");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS_T;
			stc.addClauseSynonym(1);
			stc.addClauseSynonym(2);

			SuchThatClause stc2;
			stc2.firstArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc2.firstArg.setArgStr("s1");
			stc2.secondArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc2.secondArg.setArgStr("a");
			stc2.relType = SUCH_THAT_REL_TYPE::FOLLOWS;
			stc2.addClauseSynonym(1);
			stc2.addClauseSynonym(3);

			SuchThatClause stc3;
			stc3.firstArg.setArgType(ARGUMENT_TYPE::SYNONYM);
			stc3.firstArg.setArgStr("s1");
			stc3.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc3.secondArg.setArgStr("3");
			stc3.relType = SUCH_THAT_REL_TYPE::NEXT;
			stc3.addClauseSynonym(1);

			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 2, argument, { PATTERN_MATCH_TYPE::UNRESTRICTED, "" });
			pc.addClauseSynonym(3);

			QueryArgument arg1(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "s1",
				QueryElement(NO_ERROR, 1, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::INTEGER));
			QueryArgument arg2(NO_ERROR, ARGUMENT_TYPE::INTEGER, "10", QueryElement());
			WithClause wc(arg1, arg2);
			wc.addClauseSynonym(1);


			QueryObject queryObject;
			queryObject.setSelectResult(qsr);
			queryObject.addSuchThatClause(stc, 1);
			queryObject.addSuchThatClause(stc2, 2);
			queryObject.addSuchThatClause(stc3, 3);
			queryObject.addPatternClause(pc, 4);
			queryObject.addWithClause(wc, 5);


			QueryObject optimisedQueryObject = QueryOptimiser(queryObject).optimiseQuery();

			CLAUSE_INDICES clauseGroup = optimisedQueryObject.getSelectQueryGroups()[0].getClauses();
			CLAUSE_INDICES optimisedClauseGroup = { 5, 3, 2, 1, 4 };

			Assert::IsTrue(clauseGroup == optimisedClauseGroup);
		}


#pragma endregion
	
	};
}
