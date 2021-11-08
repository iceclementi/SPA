#include "stdafx.h"
#include "TestQueryEvaluator_Pattern.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator_Pattern_Assign)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;

		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{
			for (auto& value : procTableStub_Pat) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_Pat) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_Pat) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_Pat) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_Pat) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_Pat) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_Pat) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_Pat) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : assignPatternStub_Pat) {
				pkb.addPattern(valueGroup.first, valueGroup.second.first, valueGroup.second.second);
			}
			for (auto& valueGroup : whilePatternStub_Pat) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::WHILE_PATTERN, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : ifPatternStub_Pat) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::IF_PATTERN, valueGroup.first, value);
				}
			}
			
			synTable.setSynonymStorage(TEMP_STORAGE());
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

#pragma region Evaluate If Pattern

		TEST_METHOD(evaluateAssignPattern_IDENT_Pass)
		{
			//Select pd pattern a (ident, "c")
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 3, argument, { PATTERN_MATCH_TYPE::EXACT_MATCH, "(e) + (1 / 1)" });
			pc.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_IDENT_Fail)
		{
			//Select pd pattern a (ident, "b") 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 2, argument, { PATTERN_MATCH_TYPE::EXACT_MATCH, "(e + 1) / (1)" });
			pc.addClauseSynonym(2);

			Assert::IsFalse(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_IDENT_PartialMatch_Pass)
		{
			//Select pd pattern a ("e", _"a"_) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::PARTIAL_MATCH, "c * 1" });
			pc.addClauseSynonym(1);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_IDENT_Unrestricted_Pass)
		{
			//Select pd pattern a1 ("e" ,_) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::UNRESTRICTED, "" });
			pc.addClauseSynonym(1);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_Synonym_ExactMatch_Pass)
		{
			//Select pd pattern a (v, "c") 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 4, argument, { PATTERN_MATCH_TYPE::EXACT_MATCH, "(e+(1))" });
			pc.addClauseSynonym(4);
			pc.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_Synonym_PartialMatch_Pass)
		{
			//Select pd pattern a (v, _"b + a"_) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::PARTIAL_MATCH, "1 / (1)" });
			pc.addClauseSynonym(1);
			pc.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_Synonym_Unrestricted_Pass)
		{
			//Select pd pattern a (v, _) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 2, argument, { PATTERN_MATCH_TYPE::UNRESTRICTED, "" });
			pc.addClauseSynonym(2);
			pc.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_UNDERSCORE_ExactMatch_Pass)
		{
			//Select pd pattern a (_, "c") 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::EXACT_MATCH, "0" });
			pc.addClauseSynonym(1);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_UNDERSCORE_PartialMatch_Pass)
		{
			//Select pd pattern a1 (_, _"c"_) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 2, argument, { PATTERN_MATCH_TYPE::PARTIAL_MATCH, "b - 100" });
			pc.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateAssignPattern_UNDERSCORE_Unrestricted_Pass)
		{
			//Select pd pattern a (_, _) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 3, argument, { PATTERN_MATCH_TYPE::UNRESTRICTED, "" });
			pc.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

#pragma endregion

#pragma region Evaluate While Pattern
		TEST_METHOD(evaluateWhilePattern_IDENT_Pass)
		{
			//Select w pattern w (ident, _)
			QueryArgument argument1(NO_ERROR, ARGUMENT_TYPE::IDENT, "a", QueryElement());
			PatternClause pc1(DESIGN_ENTITY::WHILE, 1, argument1, QueryPattern());
			pc1.addClauseSynonym(1);

			Assert::IsTrue(PatternClauseEvaluator(pc1).evaluateClause());

			QueryArgument argument2(NO_ERROR, ARGUMENT_TYPE::IDENT, "c", QueryElement());
			PatternClause pc2(DESIGN_ENTITY::WHILE, 2, argument2, QueryPattern());
			pc2.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc2).evaluateClause());

			QueryArgument argument3(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc3(DESIGN_ENTITY::WHILE, 3, argument3, QueryPattern());
			pc3.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc3).evaluateClause());

			QueryArgument argument4(NO_ERROR, ARGUMENT_TYPE::IDENT, "g", QueryElement());
			PatternClause pc4(DESIGN_ENTITY::WHILE, 4, argument4, QueryPattern());
			pc4.addClauseSynonym(4);

			Assert::IsTrue(PatternClauseEvaluator(pc4).evaluateClause());

			QueryArgument argument5(NO_ERROR, ARGUMENT_TYPE::IDENT, "i", QueryElement());
			PatternClause pc5(DESIGN_ENTITY::WHILE, 5, argument5, QueryPattern());
			pc5.addClauseSynonym(5);

			Assert::IsTrue(PatternClauseEvaluator(pc5).evaluateClause());
		}

		TEST_METHOD(evaluateWhilePattern_IDENT_Fail)
		{
			//Select s pattern w (ident, _) 
			QueryArgument argument1(NO_ERROR, ARGUMENT_TYPE::IDENT, "q", QueryElement());
			PatternClause pc1(DESIGN_ENTITY::WHILE, 1, argument1, QueryPattern());
			pc1.addClauseSynonym(1);

			Assert::IsFalse(PatternClauseEvaluator(pc1).evaluateClause());

			QueryArgument argument2(NO_ERROR, ARGUMENT_TYPE::IDENT, "r", QueryElement());
			PatternClause pc2(DESIGN_ENTITY::WHILE, 2, argument2, QueryPattern());
			pc2.addClauseSynonym(2);

			Assert::IsFalse(PatternClauseEvaluator(pc2).evaluateClause());

			QueryArgument argument3(NO_ERROR, ARGUMENT_TYPE::IDENT, "s", QueryElement());
			PatternClause pc3(DESIGN_ENTITY::WHILE, 3, argument3, QueryPattern());
			pc3.addClauseSynonym(3);

			Assert::IsFalse(PatternClauseEvaluator(pc3).evaluateClause());

			QueryArgument argument4(NO_ERROR, ARGUMENT_TYPE::IDENT, "w", QueryElement());
			PatternClause pc4(DESIGN_ENTITY::WHILE, 4, argument4, QueryPattern());
			pc4.addClauseSynonym(4);

			Assert::IsFalse(PatternClauseEvaluator(pc4).evaluateClause());

			QueryArgument argument5(NO_ERROR, ARGUMENT_TYPE::IDENT, "x", QueryElement());
			PatternClause pc5(DESIGN_ENTITY::WHILE, 5, argument5, QueryPattern());
			pc5.addClauseSynonym(5);

			Assert::IsFalse(PatternClauseEvaluator(pc5).evaluateClause());
		}

		TEST_METHOD(evaluateWhilePattern_Synonym_Pass)
		{
			//Select v pattern w (v, _) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			PatternClause pc(DESIGN_ENTITY::WHILE, 3, argument, QueryPattern());
			pc.addClauseSynonym(3);
			pc.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateWhilePattern_UNDERSCORE_Pass)
		{
			//Select v pattern w (_, _) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			PatternClause pc(DESIGN_ENTITY::WHILE, 3, argument, QueryPattern());
			pc.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

#pragma endregion

#pragma region Evaluate If Pattern

		TEST_METHOD(evaluateIfPattern_IDENT_Pass)
		{
			//Select ifs pattern ifs (ident, _, _)
			QueryArgument argument1(NO_ERROR, ARGUMENT_TYPE::IDENT, "a", QueryElement());
			PatternClause pc1(DESIGN_ENTITY::IF, 1, argument1, QueryPattern());
			pc1.addClauseSynonym(1);

			Assert::IsTrue(PatternClauseEvaluator(pc1).evaluateClause());

			QueryArgument argument2(NO_ERROR, ARGUMENT_TYPE::IDENT, "c", QueryElement());
			PatternClause pc2(DESIGN_ENTITY::IF, 2, argument2, QueryPattern());
			pc2.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc2).evaluateClause());

			QueryArgument argument3(NO_ERROR, ARGUMENT_TYPE::IDENT, "e", QueryElement());
			PatternClause pc3(DESIGN_ENTITY::IF, 3, argument3, QueryPattern());
			pc3.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc3).evaluateClause());

			QueryArgument argument4(NO_ERROR, ARGUMENT_TYPE::IDENT, "x", QueryElement());
			PatternClause pc4(DESIGN_ENTITY::IF, 4, argument4, QueryPattern());
			pc4.addClauseSynonym(4);

			Assert::IsTrue(PatternClauseEvaluator(pc4).evaluateClause());

			QueryArgument argument5(NO_ERROR, ARGUMENT_TYPE::IDENT, "y", QueryElement());
			PatternClause pc5(DESIGN_ENTITY::IF, 5, argument5, QueryPattern());
			pc5.addClauseSynonym(5);

			Assert::IsTrue(PatternClauseEvaluator(pc5).evaluateClause());
		}

		TEST_METHOD(evaluateIfPattern_IDENT_Fail)
		{
			//Select s pattern ifs (ident, _, _) 
			QueryArgument argument1(NO_ERROR, ARGUMENT_TYPE::IDENT, "g", QueryElement());
			PatternClause pc1(DESIGN_ENTITY::IF, 1, argument1, QueryPattern());
			pc1.addClauseSynonym(1);

			Assert::IsFalse(PatternClauseEvaluator(pc1).evaluateClause());

			QueryArgument argument2(NO_ERROR, ARGUMENT_TYPE::IDENT, "i", QueryElement());
			PatternClause pc2(DESIGN_ENTITY::IF, 2, argument2, QueryPattern());
			pc2.addClauseSynonym(2);

			Assert::IsFalse(PatternClauseEvaluator(pc2).evaluateClause());

			QueryArgument argument3(NO_ERROR, ARGUMENT_TYPE::IDENT, "n", QueryElement());
			PatternClause pc3(DESIGN_ENTITY::IF, 3, argument3, QueryPattern());
			pc3.addClauseSynonym(3);

			Assert::IsFalse(PatternClauseEvaluator(pc3).evaluateClause());

			QueryArgument argument4(NO_ERROR, ARGUMENT_TYPE::IDENT, "k", QueryElement());
			PatternClause pc4(DESIGN_ENTITY::IF, 4, argument4, QueryPattern());
			pc4.addClauseSynonym(4);

			Assert::IsFalse(PatternClauseEvaluator(pc4).evaluateClause());

			QueryArgument argument5(NO_ERROR, ARGUMENT_TYPE::IDENT, "loops", QueryElement());
			PatternClause pc5(DESIGN_ENTITY::IF, 5, argument5, QueryPattern());
			pc5.addClauseSynonym(5);

			Assert::IsFalse(PatternClauseEvaluator(pc5).evaluateClause());
		}

		TEST_METHOD(evaluateIfPattern_Synonym_Pass)
		{
			//Select v pattern ifs (v, _, _) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			PatternClause pc(DESIGN_ENTITY::IF, 3, argument, QueryPattern());
			pc.addClauseSynonym(3);
			pc.addClauseSynonym(2);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

		TEST_METHOD(evaluateIfPattern_UNDERSCORE_Pass)
		{
			//Select v pattern ifs (_, _, _) 
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement());
			PatternClause pc(DESIGN_ENTITY::IF, 3, argument, QueryPattern());
			pc.addClauseSynonym(3);

			Assert::IsTrue(PatternClauseEvaluator(pc).evaluateClause());
		}

#pragma endregion

	};
}
