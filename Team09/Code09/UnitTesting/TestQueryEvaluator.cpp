#include "stdafx.h"
#include "TestQueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator)
	{
	public:
		PKB pkb;
		SynonymStorageManager synTable;
		
		TEST_METHOD_INITIALIZE(QueryEvaluator_Init)
		{	
			//FOR FILE "onlyBasic.txt";
			for (auto& value : procTableStub_QE) {
				pkb.addDEValue(DESIGN_ENTITY::PROCEDURE, value);
			}
			for (auto& value : varTableStub_QE) {
				pkb.addDEValue(DESIGN_ENTITY::VARIABLE, value);
			}
			for (auto& value : constTableStub_QE) {
				pkb.addDEValue(DESIGN_ENTITY::CONSTANT, value);
			}

			for (auto& value : readStmtNumsStub_QE) {
				pkb.addStmt(DESIGN_ENTITY::READ, value);
			}
			for (auto& value : printStmtNumsStub_QE) {
				pkb.addStmt(DESIGN_ENTITY::PRINT, value);
			}
			for (auto& value : assignStmtNumsStub_QE) {
				pkb.addStmt(DESIGN_ENTITY::ASSIGN, value);
			}
			for (auto& value : whileStmtNumsStub_QE) {
				pkb.addStmt(DESIGN_ENTITY::WHILE, value);
			}
			for (auto& value : ifStmtNumsStub_QE) {
				pkb.addStmt(DESIGN_ENTITY::IF, value);
			}

			for (auto& valueGroup : fTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::FOLLOWS, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : fStarTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::FOLLOWS_STAR, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : pTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::PARENT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : pStarTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::PARENT_STAR, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : usesProcTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::USES_PROC, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : usesStmtTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::USES_STMT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : modifiesProcTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::MODIFIES_PROC, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : modifiesStmtTableStub_QE) {
				for (auto& value : valueGroup.second) {
					pkb.addRelationship(RELATIONSHIP_TYPE::MODIFIES_STMT, valueGroup.first, value);
				}
			}
			for (auto& valueGroup : assignPatternStub_QE) {
				pkb.addPattern(valueGroup.first, valueGroup.second.first, valueGroup.second.second);
			}
			for (auto& valueGroup : whilePatternStub_QE) {
				pkb.addCondVar(RELATIONSHIP_TYPE::FOLLOWS, valueGroup.first, valueGroup.second);
			}
			for (auto& valueGroup : ifPatternStub_QE) {
				pkb.addCondVar(RELATIONSHIP_TYPE::FOLLOWS, valueGroup.first, valueGroup.second);
			}
		}

		TEST_METHOD_CLEANUP(QueryEvaluator_CleanUp) {
			synTable.clear();
		}

		#pragma region evaluateSuchThatClause

		TEST_METHOD(evaluateSuchThatClause_Follows_Pass)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("2");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { TRUE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_Follows_Fail)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("3");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { FALSE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_FollowsStar_Pass)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("1");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("3");
			stc.relType = SUCH_THAT_REL_TYPE::FOLLOWS_T;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { TRUE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_Parent_Pass)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("6");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("7");
			stc.relType = SUCH_THAT_REL_TYPE::PARENT;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { TRUE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_ParentStar_Pass)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("6");
			stc.secondArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.secondArg.setArgStr("9");
			stc.relType = SUCH_THAT_REL_TYPE::PARENT_T;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { TRUE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_Uses_Pass)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("4");
			stc.secondArg.setArgType(ARGUMENT_TYPE::IDENT);
			stc.secondArg.setArgStr("var");
			stc.relType = SUCH_THAT_REL_TYPE::USES;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { TRUE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_Modifies_Pass)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("2");
			stc.secondArg.setArgType(ARGUMENT_TYPE::IDENT);
			stc.secondArg.setArgStr("var");
			stc.relType = SUCH_THAT_REL_TYPE::MODIFIES;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { TRUE };

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluateSuchThatClause_Invalid_Fail)
		{
			SuchThatClause stc;
			stc.firstArg.setArgType(ARGUMENT_TYPE::INTEGER);
			stc.firstArg.setArgStr("2");
			stc.secondArg.setArgType(ARGUMENT_TYPE::IDENT);
			stc.secondArg.setArgStr("var");
			stc.relType = SUCH_THAT_REL_TYPE::INVALID;

			QueryObject queryObject;
			queryObject.addSuchThatClause(stc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { FALSE };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}
		
		#pragma endregion

		#pragma region evaluatePatternClauses

		TEST_METHOD(evaluatePatternClauses_Assign_Pass)
		{
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "var", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::EXACT_MATCH, "1+1" });
			pc.addClauseSynonym(1);

			QueryObject queryObject;
			queryObject.getSelectResult().addToSelectElementList(QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			queryObject.getSelectResult().addSelectSynonym(1);
			queryObject.addPatternClause(pc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "5", "9" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		TEST_METHOD(evaluatePatternClauses_Assign_Fail)
		{
			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::IDENT, "var", QueryElement());
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::EXACT_MATCH, "1111+1" });
			pc.addClauseSynonym(1);

			QueryObject queryObject;
			queryObject.getSelectResult().addToSelectElementList(QueryElement(NO_ERROR, 1, DESIGN_ENTITY::ASSIGN, ATTRIBUTE_TYPE::NONE));
			queryObject.getSelectResult().addSelectSynonym(1);
			queryObject.addPatternClause(pc, 1);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = {};
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}

		#pragma endregion

		#pragma region evaluateQuery

		TEST_METHOD(evaluateQuery_Pass)
		{
			QueryArgument firstArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement());
			QueryArgument secondArg(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement());
			SuchThatClause stc(SUCH_THAT_REL_TYPE::FOLLOWS, firstArg, secondArg);

			QueryArgument argument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			PatternClause pc(DESIGN_ENTITY::ASSIGN, 1, argument, { PATTERN_MATCH_TYPE::UNRESTRICTED, "" });
			pc.addClauseSynonym(1);
			pc.addClauseSynonym(2);

			QueryObject queryObject;
			queryObject.getSelectResult().addToSelectElementList(QueryElement(NO_ERROR, 2, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE));
			queryObject.getSelectResult().addSelectSynonym(2);
			queryObject.addSuchThatClause(stc, 1);
			queryObject.addPatternClause(pc, 2);
			queryObject = QueryOptimiser(queryObject).optimiseQuery();
			QueryEvaluator queryEvaluator(queryObject);

			ANSWER evaluatedAnswer = queryEvaluator.evaluateQuery();
			ANSWER expectedAnswer = { "var" };
			evaluatedAnswer.sort();
			expectedAnswer.sort();

			Assert::IsTrue(evaluatedAnswer == expectedAnswer);
		}
	
		#pragma endregion	
	};
}
