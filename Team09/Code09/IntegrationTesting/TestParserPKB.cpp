#include "stdafx.h"
#include "TestParserPKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestParserPKB
{
	TEST_CLASS(TestParserPKB)
	{
	public:
		PKB pkb;

		VARS getStringDEValuesOf(DESIGN_ENTITY deType) {
			INDICES& indexedDEValues = pkb.getIndexedDEValuesOf(deType);
			VARS stringDEValues;
			for (auto& index : indexedDEValues) {
				stringDEValues.insert(pkb.getStringValueOfDE(index));
			}
			return stringDEValues;
		}

		RELATIONSHIP_TABLE<VAR, VAR> convertTotStringRelationshipTable(RelationshipStorage<INDEX, INDEX>& relStore) {
			RELATIONSHIP_TABLE<VAR, VAR> stringRelTable;
			for (auto& row : relStore.getForwardTable()) {
				VAR firstStringValue = pkb.getStringValueOfDE(row.first);
				for (auto& secondIndex : row.second) {
					stringRelTable[firstStringValue].insert(pkb.getStringValueOfDE(secondIndex));
				}
			}
			return stringRelTable;
		}

		RELATIONSHIP_TABLE<VAR, VAR> getStringRelationshipTableOf(RELATIONSHIP_TYPE rType) {
			return convertTotStringRelationshipTable(pkb.getRelationshipStoreOf(rType));
		}

		RELATIONSHIP_TABLE<STMT_NUM, PATTERN> getStringAssignPatternTable() {
			RelationshipStorage<INDEX, PATTERN>& assignPatternStore = pkb.getAssignPatternStore();
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> stringAssignPatternTable;
			for (auto& row : assignPatternStore.getForwardTable()) {
				VAR firstStringValue = pkb.getStringValueOfDE(row.first);
				for (auto& pattern : row.second) {
					stringAssignPatternTable[firstStringValue].insert(pattern);
				}
			}
			return stringAssignPatternTable;
		}


		TEST_METHOD_CLEANUP(TestParserPKB_CleanUp) {
			pkb.clear();
		}

		TEST_METHOD(singleLineTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/singleLineTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var2", "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var2", "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "1", { "var1" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "1", { " var2  var3  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(BasicTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/basictest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3" } },
				{ "2", { "3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "2", "3" } },
				{ "2", { "3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "3", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "2", { "var2" } },
				{ "3", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "3", { "var3" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "3", { " 1  var4  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/whileTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2" , "3", "4" } },
				{ "2", { "1", "2" , "3", "4" } },
				{ "3", { "1", "2" , "3", "4" } },
				{ "4", { "1", "2" , "3", "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "2", { "3" } },
				{ "3", { "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "2", { "3", "4" } },
				{ "3", { "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var3" } },
				{ "3", { "var3" } },
				{ "4", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "4", { "var3" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "4", { " 1  var4  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/WhileTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2" , "3", "4", "5", "6", "7" } },
				{ "2", { "1", "2" , "3", "4", "5", "6", "7" } },
				{ "3", { "1", "2" , "3", "4", "5", "6", "7" } },
				{ "4", { "1", "2" , "3", "4", "5", "6", "7" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);
			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "5" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "5", "6", "7" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "1" } },
				{ "5", { "6" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var4", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var4" } },
				{ "2", { "var1" } },
				{ "4", { "var4" } },
				{ "5", { "var2" } },
				{ "7", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3", "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "6", { "var3" } },
				{ "7", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "4", { "var3" } },
				{ "7", { "var4" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "4", { " 1  var4  / " } },
				{ "7", { " 1  var5  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/WhileMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3", "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "3", { "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "4", { "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "5", { "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "6", { "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "7", { "4" ,"5", "6", "7", "8" ,"9", "10" } },
				{ "8", { "9", "10" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "10", "2", "3", "4", "8", "9" } },
				{ "2", { "10", "3", "4", "8", "9" } },
				{ "3", { "10", "4", "8", "9" } },
				{ "4", { "10", "8", "9" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "4", { "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "4", { "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "4" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "10", { "var12" } },
				{ "3", { "var4" } },
				{ "4", { "var1", "var5", "var8" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var11" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var6", "var7" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var11" } },
				{ "3", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " 1  var12  + " } },
				{ "3", { " 1  var4  + " } },
				{ "7", { " 1  var8  % " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "4", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/WhileBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3", "4" ,"5", "6", "7" } },
				{ "2", { "3", "4" ,"5", "6", "7" } },
				{ "3", { "4" ,"5", "6", "7" } },
				{ "4", { "4" ,"5", "6", "7" } },
				{ "5", { "4" ,"5", "6", "7" } },
				{ "6", { "4" ,"5", "6", "7" } },
				{ "7", { "4" ,"5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "2", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "2", "3", "4" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "4", { "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "4", { "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "3", { "var4" } },
				{ "4", { "var4", "var5", "var8", "var9" } },
				{ "5", { "var5" } },
				{ "7", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var6", "var7" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "3", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "3", { " 1  var4  / " } },
				{ "7", { " var8  var9  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3", "4" ,"5", "6", "7" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var4", "var5", "var8", "var9" } },
				{ "2", { "var1" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3", "var6", "var7" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "4", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "4", { " var4  2  + " } },
				{ "7", { " var8  var9  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3", "4" ,"5", "6", "7", "8", "9", "10" } },
				{ "2", { "3", "4", "8", "9", "10" } },
				{ "3", { "4", "8", "9", "10" } },
				{ "4", { "8", "9", "10" } },
				{ "5", { "6", "7", "8", "9", "10" } },
				{ "6", { "7", "8", "9", "10" } },
				{ "7", { "8", "9", "10" } },
				{ "8", { "9", "10" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "10", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "8" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "10", "8", "9" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var2", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4", "var5", "var8" } },
				{ "10", { "var11", "var12" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3", "var6", "var7" } },
				{ "10", { "var11" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var11" } },
				{ "4", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " var11  var12  % " } },
				{ "4", { " 1  var4  / " } },
				{ "7", { " var8  var8  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13" } },
				{ "4", { "5", "6", "7", "8", "9", "10", "11", "12", "13" } },
				{ "5", { "6", "7", "11", "12", "13" } },
				{ "6", { "7", "11", "12", "13" } },
				{ "7", { "11", "12", "13" } },
				{ "8", { "9", "10", "11", "12", "13" } },
				{ "9", { "10", "11", "12", "13" } },
				{ "10", { "11", "12", "13" } },
				{ "11", { "12", "13" } },
				{ "12", { "13" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{ 
				"var1", "var10", "var11", "var12", "var13", "var14", "var15", "var16", 
				"var17", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" 
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "11", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "10", "13", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "11" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "11", "12", "13", "2", "3", "4" } },
				{ "11", { "12", "13" } },
				{ "12", { "13" } },
				{ "2", { "11", "12", "13", "3", "4" } },
				{ "3", { "11", "12", "13", "4" } },
				{ "4", { "11", "12", "13" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "11" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var13", "var16", "var17", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "10", { "var12" } },
				{ "11", { "var13" } },
				{ "13", { "var16", "var17" } },
				{ "3", { "var4" } },
				{ "4", { "var12", "var4", "var5", "var8", "var9" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var14", "var15", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var11" } },
				{ "12", { "var14" } },
				{ "13", { "var15" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var10", "var11", "var6", "var7" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var11" } },
				{ "13", { "var15" } },
				{ "3", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " 1  var12  / " } },
				{ "13", { " var16  var17  * " } },
				{ "3", { " 1  var4  + " } },
				{ "7", { " 1  var8  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2" , "3", "4" ,"5", "6", "7", "8", "9", "10" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10" } },
				{ "4", { "5", "6", "7", "8", "9", "10" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "9", "10" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);
			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "2", "3", "4" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "10", { "var12" } },
				{ "3", { "var4" } },
				{ "4", { "var12", "var4", "var5", "var8", "var9" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var11" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var10", "var11", "var6", "var7" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var11" } },
				{ "3", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " 1  var12  / " } },
				{ "3", { " 1  var4  % " } },
				{ "7", { " 1  var8  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileIfElseTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/whileIfElseTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "5", { "6", "7", "8", "9", "10", "11" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "11" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "5" } },
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "5" } },
				{ "10", { "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "1" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var4" } },
				{ "11", { "var12" } },
				{ "2", { "var1" } },
				{ "4", { "var4" } },
				{ "5", { "var12", "var4", "var5", "var8", "var9" } },
				{ "6", { "var5" } },
				{ "8", { "var8" } },
				{ "9", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3" } },
				{ "10", { "var10" } },
				{ "11", { "var11" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var10", "var11", "var6", "var7" } },
				{ "7", { "var6" } },
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var11" } },
				{ "4", { "var3" } },
				{ "8", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var12  % " } },
				{ "4", { " 1  var4  / " } },
				{ "8", { " 1  var8  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "5", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileIfElseTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/whileIfElseTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "7", "8", "12", "13", "14" } },
				{ "7", { "8", "12", "13", "14" } },
				{ "8", { "12", "13", "14" } },
				{ "9", { "10", "11", "12", "13", "14" } },
				{ "10", { "11", "12", "13", "14" } },
				{ "11", { "12", "13", "14" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{ 
				"var1", "var10", "var11", "var12", "var13", "var14", "var15", 
				"var16", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" 
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "11", "123" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "13", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "12", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "5" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "12" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "12", "13", "14", "5" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "12", "13", "14" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "1" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var13", "var16", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var4" } },
				{ "11", { "var12" } },
				{ "12", { "var13" } },
				{ "14", { "var16" } },
				{ "2", { "var1" } },
				{ "4", { "var4" } },
				{ "5", { "var12", "var4", "var5", "var8", "var9" } },
				{ "6", { "var5" } },
				{ "8", { "var8" } },
				{ "9", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var14", "var15", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3" } },
				{ "10", { "var10" } },
				{ "11", { "var11" } },
				{ "13", { "var14" } },
				{ "14", { "var15" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var10", "var11", "var6", "var7" } },
				{ "7", { "var6" } },
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var11" } },
				{ "14", { "var15" } },
				{ "4", { "var3" } },
				{ "8", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var12  % " } },
				{ "14", { " 123  var16  + " } },
				{ "4", { " 1  var4  / " } },
				{ "8", { " 1  var8  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "5", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileIfElseMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/whileIfElseMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "4", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "5", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "6", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "7", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "8", { "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "9", { "10", "11", "15", "16", "17" } },
				{ "10", { "11", "15", "16", "17" } },
				{ "11", { "15", "16", "17" } },
				{ "12", { "13", "14", "15", "16", "17" } },
				{ "13", { "14", "15", "16", "17" } },
				{ "14", { "15", "16", "17" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15",
				"var16", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "11", "123" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "13", "16", "2", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "12", "15", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "17", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "15" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "15", "16", "17", "2", "3", "4", "8" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } },
				{ "2", { "15", "16", "17", "3", "4", "8" } },
				{ "3", { "15", "16", "17", "4", "8" } },
				{ "4", { "15", "16", "17", "8" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "15", "16", "17" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "4", { "5", "6", "7" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "4", { "5", "6", "7" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "15" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "14", { "15" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "4" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var13", "var16", "var4", "var6", "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "11", { "var8" } },
				{ "12", { "var9" } },
				{ "14", { "var12" } },
				{ "15", { "var13" } },
				{ "17", { "var16" } },
				{ "3", { "var4" } },
				{ "4", { "var4", "var6" } },
				{ "5", { "var4" } },
				{ "7", { "var6" } },
				{ "8", { "var12", "var6", "var7", "var8", "var9" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var14", "var15", "var2", "var3", "var5", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var7" } },
				{ "11", { "var8" } },
				{ "13", { "var10" } },
				{ "14", { "var11" } },
				{ "16", { "var14" } },
				{ "17", { "var15" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var5" } },
				{ "6", { "var5" } },
				{ "7", { "var5" } },
				{ "8", { "var10", "var11", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "14", { "var11" } },
				{ "17", { "var15" } },
				{ "3", { "var3" } },
				{ "7", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var8  * " } },
				{ "14", { " 1  var12  % " } },
				{ "17", { " 123  var16  + " } },
				{ "3", { " 1  var4  / " } },
				{ "7", { " 1  var6  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "8", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(whileIfElseBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/whileIfElseBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "4", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "5", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "7", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "8", { "9", "10", "11", "12", "13", "14" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "11" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "13", "2", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "12", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "2", "3", "4", "8" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4", "8" } },
				{ "3", { "4", "8" } },
				{ "4", { "8" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "4", { "5", "6", "7" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "4", { "5", "6", "7" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "4" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var4", "var6", "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "11", { "var8" } },
				{ "12", { "var9" } },
				{ "14", { "var12" } },
				{ "3", { "var4" } },
				{ "4", { "var4", "var6" } },
				{ "5", { "var4" } },
				{ "7", { "var6" } },
				{ "8", { "var12", "var6", "var7", "var8", "var9" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var5", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var7" } },
				{ "11", { "var8" } },
				{ "13", { "var10" } },
				{ "14", { "var11" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var5" } },
				{ "6", { "var5" } },
				{ "7", { "var5" } },
				{ "8", { "var10", "var11", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "14", { "var11" } },
				{ "3", { "var3" } },
				{ "7", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var8  * " } },
				{ "14", { " 1  var12  + " } },
				{ "3", { " 1  var4  - " } },
				{ "7", { " 1  var6  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "8", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseWhileTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseWhileTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "3", "4", "8", "9", "10", "11" } },
				{ "3", { "4", "8", "9", "10", "11" } },
				{ "4", { "8", "9", "10", "11" } },
				{ "5", { "6", "7", "8", "9", "10", "11" } },
				{ "6", { "7", "8", "9", "10", "11" } },
				{ "7", { "8", "9", "10", "11" } },
				{ "8", { "8", "9", "10", "11" } },
				{ "9", { "8", "9", "10", "11" } },
				{ "10", { "8", "9", "10", "11" } },
				{ "11", { "8", "9", "10", "11" } },

			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "8" } },
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "8" } },
				{ "10", { "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "8" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var2", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4", "var5", "var8" } },
				{ "11", { "var11", "var12" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var11", "var12", "var8", "var9" } },
				{ "9", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3", "var6", "var7" } },
				{ "10", { "var10" } },
				{ "11", { "var11" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } },
				{ "8", { "var10", "var11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var11" } },
				{ "4", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var11  var12  % " } },
				{ "4", { " 1  var4  / " } },
				{ "7", { " var8  var8  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "8", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseWhileTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseWhileTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4", "8", "9", "10", "11", "12", "13", "14" } },
				{ "3", { "4", "8", "9", "10", "11", "12", "13", "14" } },
				{ "4", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "7", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "8", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "9", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "10", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "11", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15",
				"var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "13", "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "12", "2", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "8" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "12" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "12", "13", "14", "8" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "12", "13", "14" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "8" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var15", "var2", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4", "var5", "var8" } },
				{ "11", { "var11", "var12" } },
				{ "12", { "var12" } },
				{ "14", { "var15" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var11", "var12", "var8", "var9" } },
				{ "9", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var13", "var14", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3", "var6", "var7" } },
				{ "10", { "var10" } },
				{ "11", { "var11" } },
				{ "13", { "var13" } },
				{ "14", { "var14" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "6", { "var6" } },
				{ "7", { "var7" } },
				{ "8", { "var10", "var11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var11" } },
				{ "14", { "var14" } },
				{ "4", { "var3" } },
				{ "7", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var11  var12  % " } },
				{ "14", { " var15  2  + " } },
				{ "4", { " 1  var4  / " } },
				{ "7", { " var8  var8  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "8", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseWhileMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseWhileMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "4", { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "5", { "6", "7", "11", "12", "13", "14", "15", "16", "17" } },
				{ "6", { "7", "11", "12", "13", "14", "15", "16", "17" } },
				{ "7", { "11", "12", "13", "14", "15", "16", "17" } },
				{ "8", { "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "9", { "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "10", { "11", "12", "13", "14", "15", "16", "17" } },
				{ "11", { "11", "12", "13", "14", "15", "16", "17" } },
				{ "12", { "11", "12", "13", "14", "15", "16", "17" } },
				{ "13", { "11", "12", "13", "14", "15", "16", "17" } },
				{ "14", { "11", "12", "13", "14", "15", "16", "17" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var10", "var11", "var12", "var13", "var14", "var15",
				"var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "13", "16", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "12", "15", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "10", "14", "17", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "11" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "11", { "15" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "11" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "11", "15", "16", "17", "2", "3", "4" } },
				{ "11", { "15", "16", "17" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } },
				{ "2", { "11", "15", "16", "17", "3", "4" } },
				{ "3", { "11", "15", "16", "17", "4" } },
				{ "4", { "11", "15", "16", "17" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "11", { "12", "13", "14" } },
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "11", { "12", "13", "14" } },
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "12", "15" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "14", { "11" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "11" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var11", "var12", "var15", "var2", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var2" } },
				{ "10", { "var8" } },
				{ "11", { "var11", "var12", "var8", "var9" } },
				{ "12", { "var9" } },
				{ "14", { "var11", "var12" } },
				{ "15", { "var12" } },
				{ "17", { "var15" } },
				{ "3", { "var4" } },
				{ "4", { "var2", "var4", "var5", "var8" } },
				{ "5", { "var2" } },
				{ "7", { "var4" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var13", "var14", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var7" } },
				{ "11", { "var10", "var11" } },
				{ "13", { "var10" } },
				{ "14", { "var11" } },
				{ "16", { "var13" } },
				{ "17", { "var14" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var2", "var3", "var6", "var7" } },
				{ "6", { "var2" } },
				{ "7", { "var3" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var7" } },
				{ "14", { "var11" } },
				{ "17", { "var14" } },
				{ "3", { "var3" } },
				{ "7", { "var3" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " var8  var8  * " } },
				{ "14", { " var11  var12  % " } },
				{ "17", { " var15  2  + " } },
				{ "3", { " 1  var4  / " } },
				{ "7", { " 1  var4  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "11", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(ifElseWhileBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/ifElseWhileBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "4", { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "5", { "6", "7", "11", "12", "13", "14" } },
				{ "6", { "7", "11", "12", "13", "14" } },
				{ "7", { "11", "12", "13", "14" } },
				{ "8", { "9", "10", "11", "12", "13", "14" } },
				{ "9", { "10", "11", "12", "13", "14" } },
				{ "10", { "11", "12", "13", "14" } },
				{ "11", { "11", "12", "13", "14" } },
				{ "12", { "11", "12", "13", "14" } },
				{ "13", { "11", "12", "13", "14" } },
				{ "14", { "11", "12", "13", "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "10", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "13", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "12", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "10", "14", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "11" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "11" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "11", "2", "3", "4" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "11", "3", "4" } },
				{ "3", { "11", "4" } },
				{ "4", { "11" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "11", { "12", "13", "14" } },
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "11", { "12", "13", "14" } },
				{ "4", { "10", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "14", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5", "8" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "11" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var2", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "10", { "var8" } },
				{ "11", { "var11", "var12", "var8", "var9" } },
				{ "12", { "var9" } },
				{ "14", { "var11", "var12" } },
				{ "3", { "var4" } },
				{ "4", { "var2", "var4", "var5", "var8" } },
				{ "5", { "var2" } },
				{ "7", { "var4" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var7" } },
				{ "11", { "var10", "var11" } },
				{ "13", { "var10" } },
				{ "14", { "var11" } },
				{ "2", { "var2" } },
				{ "3", { "var3" } },
				{ "4", { "var2", "var3", "var6", "var7" } },
				{ "6", { "var2" } },
				{ "7", { "var3" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var7" } },
				{ "14", { "var11" } },
				{ "3", { "var3" } },
				{ "7", { "var3" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " var8  var8  * " } },
				{ "14", { " var11  var12  % " } },
				{ "3", { " 1  var4  / " } },
				{ "7", { " 1  var4  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "11", { "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "4", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileWhileTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileWhileTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5" } },
				{ "2", { "1", "2", "3", "4" ,"5" } },
				{ "3", { "1", "2", "3", "4" ,"5" } },
				{ "4", { "1", "2", "3", "4" ,"5" } },
				{ "5", { "1", "2", "3", "4" ,"5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "3", { "4" } },
				{ "4", { "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "3", { "4", "5" } },
				{ "4", { "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "1", "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4" } },
				{ "2", { "var1", "var2", "var4" } },
				{ "3", { "var2" } },
				{ "5", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var3" } },
				{ "2", { "var3" } },
				{ "4", { "var3" } },
				{ "5", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "5", { "var3" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "5", { " 1  var4  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } },
				{ "2", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileWhileTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileWhileTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "2", { "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "2", { "6", "7", "8" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "6", "7", "8" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3", "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "2" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4", "var5", "var8", "var9" } },
				{ "2", { "var1", "var2", "var4" } },
				{ "3", { "var2" } },
				{ "5", { "var4" } },
				{ "6", { "var5" } },
				{ "8", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var3", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var3", "var6", "var7" } },
				{ "2", { "var3" } },
				{ "4", { "var3" } },
				{ "5", { "var3" } },
				{ "7", { "var6" } },
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "5", { "var3" } },
				{ "8", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "5", { " 1  var4  - " } },
				{ "8", { " var8  var9  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } },
				{ "2", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);


		}

		TEST_METHOD(nestedWhileWhileMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileWhileMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "9", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "10", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "11", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },

			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "10", "11", "3", "4", "5", "9" } },
				{ "3", { "10", "11", "4", "5", "9" } },
				{ "4", { "10", "11", "5", "9" } },
				{ "5", { "10", "11", "9" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "1" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var4", "var6", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var4", "var6", "var8", "var9" } },
				{ "11", { "var8", "var9" } },
				{ "2", { "var1" } },
				{ "4", { "var4" } },
				{ "5", { "var4", "var6" } },
				{ "6", { "var4" } },
				{ "8", { "var6" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3", "var5", "var6", "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3", "var5", "var6", "var7" } },
				{ "10", { "var6" } },
				{ "11", { "var7" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var5" } },
				{ "7", { "var5" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var7" } },
				{ "4", { "var3" } },
				{ "8", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var8  var9  / " } },
				{ "4", { " 1  var4  - " } },
				{ "8", { " 1  var6  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } },
				{ "5", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileWhileBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileWhileBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "2", { "3", "4", "5" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "1", "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var4", "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var4", "var6" } },
				{ "2", { "var1" } },
				{ "4", { "var4" } },
				{ "5", { "var4", "var6" } },
				{ "6", { "var4" } },
				{ "8", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var3", "var5" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var5" } },
				{ "7", { "var5" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "4", { "var3" } },
				{ "8", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "4", { " 1  var4  - " } },
				{ "8", { " 1  var6  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } },
				{ "5", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileIfElseTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileIfElseTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "2", { "3", "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "1" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var4", "var5", "var7", "var9" } },
				{ "2", { "var2", "var4", "var5", "var7", "var9" } },
				{ "4", { "var4" } },
				{ "5", { "var4", "var5" } },
				{ "7", { "var7" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var3", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var3", "var5", "var6", "var8" } },
				{ "2", { "var3", "var5", "var6", "var8" } },
				{ "3", { "var3" } },
				{ "5", { "var5" } },
				{ "6", { "var6" } },
				{ "8", { "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "5", { "var5" } },
				{ "8", { "var8" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "5", { " var4  var5  * " } },
				{ "8", { " var9  5  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "2", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileIfElseTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileIfElseTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "9", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "10", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "11", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "9" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "10", "11", "9" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "2", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "1" } },
				{ "2", { "3", "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var13", "var2", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var11", "var13", "var2", "var4", "var5", "var7", "var9" } },
				{ "10", { "var11" } },
				{ "11", { "var13" } },
				{ "2", { "var2", "var4", "var5", "var7", "var9" } },
				{ "4", { "var4" } },
				{ "5", { "var4", "var5" } },
				{ "7", { "var7" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var12", "var3", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var10", "var12", "var3", "var5", "var6", "var8" } },
				{ "11", { "var12" } },
				{ "2", { "var3", "var5", "var6", "var8" } },
				{ "3", { "var3" } },
				{ "5", { "var5" } },
				{ "6", { "var6" } },
				{ "8", { "var8" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var12" } },
				{ "5", { "var5" } },
				{ "8", { "var8" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 3  var13  % " } },
				{ "5", { " var4  var5  * " } },
				{ "8", { " var9  5  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "2", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileIfElseMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileIfElseMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "9", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "10", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "11", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "12", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "13", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "14", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },


			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "12" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "12", "13", "14", "3", "4", "5" } },
				{ "3", { "12", "13", "14", "4", "5" } },
				{ "4", { "12", "13", "14", "5" } },
				{ "5", { "12", "13", "14" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "12", "13", "14", "2", "3", "4", "5" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "14", { "1" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var11", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "10", { "var7" } },
				{ "11", { "var9" } },
				{ "13", { "var11" } },
				{ "14", { "var13" } },
				{ "3", { "var2" } },
				{ "4", { "var1", "var3" } },
				{ "5", { "var3", "var4", "var5", "var7", "var9" } },
				{ "7", { "var4" } },
				{ "8", { "var4", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var10", "var12", "var2", "var3", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var1", "var10", "var12", "var2", "var3", "var5", "var6", "var8" } },
				{ "11", { "var8" } },
				{ "12", { "var10" } },
				{ "14", { "var12" } },
				{ "2", { "var1" } },
				{ "4", { "var2" } },
				{ "5", { "var3", "var5", "var6", "var8" } },
				{ "6", { "var3" } },
				{ "8", { "var5" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "14", { "var12" } },
				{ "4", { "var2" } },
				{ "8", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var9  5  / " } },
				{ "14", { " 3  var13  % " } },
				{ "4", { " var1  var3  * " } },
				{ "8", { " var4  var5  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "5", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedWhileIfElseBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedWhileIfElseBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "4", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "5", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "6", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "7", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "8", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "9", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "10", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "11", { "1", "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },


			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "3", "4", "5" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "11", { "1" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "1" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "10", { "var7" } },
				{ "11", { "var9" } },
				{ "3", { "var2" } },
				{ "4", { "var1", "var3" } },
				{ "5", { "var3", "var4", "var5", "var7", "var9" } },
				{ "7", { "var4" } },
				{ "8", { "var4", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "11", { "var8" } },
				{ "2", { "var1" } },
				{ "4", { "var2" } },
				{ "5", { "var3", "var5", "var6", "var8" } },
				{ "6", { "var3" } },
				{ "8", { "var5" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "4", { "var2" } },
				{ "8", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var9  5  / " } },
				{ "4", { " var1  var3  * " } },
				{ "8", { " var4  var5  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "5", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfIfElseTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfIfElseTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8" } },
				{ "3", { "4" ,"5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "2", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "9" } },
				{ "10", { "11" } },
				{ "2", { "3", "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var2", "var3", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var11", "var12", "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "10", { "var11" } },
				{ "11", { "var12" } },
				{ "2", { "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "4", { "var3" } },
				{ "5", { "var4", "var5" } },
				{ "7", { "var7" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var12", "var2", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var10", "var12", "var2", "var5", "var6", "var8" } },
				{ "11", { "var12" } },
				{ "2", { "var2", "var5", "var6", "var8" } },
				{ "3", { "var2" } },
				{ "5", { "var5" } },
				{ "6", { "var6" } },
				{ "8", { "var8" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var12" } },
				{ "5", { "var5" } },
				{ "8", { "var8" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", {" var12  5  / " } },
				{ "5", {" var4  var5  * " } },
				{ "8", {" var9  10  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "2", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfIfElseTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfIfElseTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "4" ,"5", "9", "10", "11" } },
				{ "4", { "5", "9", "10", "11" } },
				{ "5", { "9", "10", "11" } },
				{ "6", { "7", "8", "9", "10", "11" } },
				{ "7", { "8", "9", "10", "11" } },
				{ "8", { "9", "10", "11" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },

			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "3", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "9" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "10", "11", "9" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "12", "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3", "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "10", { "var11" } },
				{ "11", { "var12" } },
				{ "13", { "var12" } },
				{ "14", { "var13" } },
				{ "2", { "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "4", { "var3" } },
				{ "5", { "var4", "var5" } },
				{ "7", { "var7" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var12", "var2", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var10", "var11", "var12", "var2", "var5", "var6", "var8" } },
				{ "11", { "var12" } },
				{ "12", { "var11" } },
				{ "14", { "var12" } },
				{ "2", { "var2", "var5", "var6", "var8" } },
				{ "3", { "var2" } },
				{ "5", { "var5" } },
				{ "6", { "var6" } },
				{ "8", { "var8" } },
				{ "9", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var12" } },
				{ "14", { "var12" } },
				{ "5", { "var5" } },
				{ "8", { "var8" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var12  5  / " } },
				{ "14", { " var13  5  / " } },
				{ "5", { " var4  var5  * " } },
				{ "8", { " var9  10  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "2", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfIfElseMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfIfElseMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "4", { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "7", "8", "12", "13", "14" } },
				{ "7", { "8", "12", "13", "14" } },
				{ "8", { "12", "13", "14" } },
				{ "9", { "10", "11", "12", "13", "14" } },
				{ "10", { "11", "12", "13", "14" } },
				{ "11", { "12", "13", "14" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } },

			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "15", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "16", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "17", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "12" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } },
				{ "2", { "12", "13", "14", "3", "4", "5" } },
				{ "3", { "12", "13", "14", "4", "5" } },
				{ "4", { "12", "13", "14", "5" } },
				{ "5", { "12", "13", "14" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "12", "13", "14", "15", "16", "17", "2", "3", "4", "5" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "15", "16", "17", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "15", "2" } },
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "10", { "var7" } },
				{ "11", { "var9" } },
				{ "13", { "var11" } },
				{ "14", { "var12" } },
				{ "16", { "var12" } },
				{ "17", { "var13" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var3", "var4", "var5", "var7", "var9" } },
				{ "7", { "var4" } },
				{ "8", { "var4", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var12", "var3", "var4", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var1", "var10", "var11", "var12", "var3", "var4", "var6", "var8" } },
				{ "11", { "var8" } },
				{ "12", { "var10" } },
				{ "14", { "var12" } },
				{ "15", { "var11" } },
				{ "17", { "var12" } },
				{ "2", { "var1" } },
				{ "4", { "var3" } },
				{ "5", { "var3", "var4", "var6", "var8" } },
				{ "6", { "var3" } },
				{ "8", { "var4" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "14", { "var12" } },
				{ "17", { "var12" } },
				{ "4", { "var3" } },
				{ "8", { "var4" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var9  10  + " } },
				{ "14", { " var12  5  / " } },
				{ "17", { " var13  5  / " } },
				{ "4", { " var3  var3  * " } },
				{ "8", { " var4  var5  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "5", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfIfElseBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfIfElseBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "4", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "5", { "6", "7", "8", "9", "10", "11" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },

			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4", "5" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "12", "13", "14", "2", "3", "4", "5" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "12", "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var7", "var9" } },
				{ "10", { "var7" } },
				{ "11", { "var9" } },
				{ "13", { "var12" } },
				{ "14", { "var13" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var3", "var4", "var5", "var7", "var9" } },
				{ "7", { "var4" } },
				{ "8", { "var4", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var11", "var12", "var3", "var4", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var1", "var11", "var12", "var3", "var4", "var6", "var8" } },
				{ "11", { "var8" } },
				{ "12", { "var11" } },
				{ "14", { "var12" } },
				{ "2", { "var1" } },
				{ "4", { "var3" } },
				{ "5", { "var3", "var4", "var6", "var8" } },
				{ "6", { "var3" } },
				{ "8", { "var4" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "14", { "var12" } },
				{ "4", { "var3" } },
				{ "8", { "var4" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var9  10  + " } },
				{ "14", { " var13  5  / " } },
				{ "4", { " var3  var3  * " } },
				{ "8", { " var4  var5  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "5", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseIfTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseIfTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8", "9", "10", "11" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } },

			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{ 
				"var1", "var10", "var11", "var12", "var13", "var14", "var15", 
				"var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" 
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var13", "var15", "var3", "var5", "var6", "var8" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var11", "var13", "var15", "var3", "var5", "var6", "var8" } },
				{ "10", { "var13" } },
				{ "11", { "var15" } },
				{ "3", { "var3" } },
				{ "4", { "var5" } },
				{ "5", { "var10", "var11", "var13", "var15", "var6", "var8" } },
				{ "7", { "var8" } },
				{ "8", { "var10", "var11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var12", "var14", "var2", "var4", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var12", "var14", "var2", "var4", "var7", "var9" } },
				{ "11", { "var14" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var12", "var14", "var7", "var9" } },
				{ "6", { "var7" } },
				{ "8", { "var9" } },
				{ "9", { "var12" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var14" } },
				{ "4", { "var4" } },
				{ "8", { "var9" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var15  10  + " } },
				{ "4", { " var5  5  / " } },
				{ "8", { " var10  var11  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "5", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseIfTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseIfTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "7", "8", "12", "13", "14" } },
				{ "7", { "8", "12", "13", "14" } },
				{ "8", { "12", "13", "14" } },
				{ "9", { "10", "11", "12", "13", "14" } },
				{ "10", { "11", "12", "13", "14" } },
				{ "11", { "12", "13", "14" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15", "var16", "var17", "var19",
				"var2", "var20", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "12" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "12", "13", "14" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "12", "13", "14", "2", "3", "4", "5" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var13", "var15", "var17",
				"var20", "var3", "var5", "var6", "var8" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var11", "var13", "var15", "var17", "var20", "var3", "var5", "var6", "var8" } },
				{ "10", { "var13" } },
				{ "11", { "var15" } },
				{ "13", { "var17" } },
				{ "14", { "var20" } },
				{ "3", { "var3" } },
				{ "4", { "var5" } },
				{ "5", { "var10", "var11", "var13", "var15", "var6", "var8" } },
				{ "7", { "var8" } },
				{ "8", { "var10", "var11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var12", "var14", "var16", "var19", "var2", "var4", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var12", "var14", "var16", "var19", "var2", "var4", "var7", "var9" } },
				{ "11", { "var14" } },
				{ "12", { "var16" } },
				{ "14", { "var19" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var12", "var14", "var7", "var9" } },
				{ "6", { "var7" } },
				{ "8", { "var9" } },
				{ "9", { "var12" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var14" } },
				{ "14", { "var19" } },
				{ "4", { "var4" } },
				{ "8", { "var9" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var15  10  + " } },
				{ "14", { " 10  var20  + " } },
				{ "4", { " var5  5  / " } },
				{ "8", { " var10  var11  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "5", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseIfMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseIfMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "6", { "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "7", { "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "8", { "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
				{ "9", { "10", "11", "15", "16", "17" } },
				{ "10", { "11", "15", "16", "17" } },
				{ "11", { "15", "16", "17" } },
				{ "12", { "13", "14", "15", "16", "17" } },
				{ "13", { "14", "15", "16", "17" } },
				{ "14", { "15", "16", "17" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15", "var16", "var17", "var19",
				"var2", "var20", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "15", "2", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "16", "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "17", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "15" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "15", "16", "17", "6", "7", "8" } },
				{ "6", { "15", "16", "17", "7", "8" } },
				{ "7", { "15", "16", "17", "8" } },
				{ "8", { "15", "16", "17" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "15", "16", "17", "2", "3", "4", "5", "6", "7", "8" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "15", "16", "17", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "15" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "14", { "15" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var13", "var15", "var17", "var20", "var3", "var5", "var6", "var7", "var8" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var11", "var13", "var15", "var17", "var20", "var3", "var5", "var6", "var7", "var8" } },
				{ "10", { "var8" } },
				{ "11", { "var10", "var11" } },
				{ "13", { "var13" } },
				{ "14", { "var15" } },
				{ "16", { "var17" } },
				{ "17", { "var20" } },
				{ "3", { "var3" } },
				{ "4", { "var5" } },
				{ "6", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var10", "var11", "var13", "var15", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var12", "var14", "var16", "var19", "var2", "var4", "var5", "var6", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var12", "var14", "var16", "var19", "var2", "var4", "var5", "var6", "var7", "var9" } },
				{ "11", { "var9" } },
				{ "12", { "var12" } },
				{ "14", { "var14" } },
				{ "15", { "var16" } },
				{ "17", { "var19" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var12", "var14", "var7", "var9" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var9" } },
				{ "14", { "var14" } },
				{ "17", { "var19" } },
				{ "4", { "var4" } },
				{ "7", { "var6" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var10  var11  * " } },
				{ "14", { " var15  10  + " } },
				{ "17", { " 10  var20  + " } },
				{ "4", { " var5  5  / " } },
				{ "7", { " var6  var6  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseIfBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseIfBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "7", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "8", { "9", "10", "11", "12", "13", "14" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15",
				"var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "12", "2", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "10", "13", "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var13", "var15", "var3", "var5", "var6", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var11", "var13", "var15", "var3", "var5", "var6", "var7", "var8" } },
				{ "10", { "var8" } },
				{ "11", { "var10", "var11" } },
				{ "13", { "var13" } },
				{ "14", { "var15" } },
				{ "3", { "var3" } },
				{ "4", { "var5" } },
				{ "6", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var10", "var11", "var13", "var15", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var12", "var14", "var2", "var4", "var5", "var6", "var7", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var12", "var14", "var2", "var4", "var5", "var6", "var7", "var9" } },
				{ "11", { "var9" } },
				{ "12", { "var12" } },
				{ "14", { "var14" } },
				{ "2", { "var2" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var12", "var14", "var7", "var9" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var9" } },
				{ "14", { "var14" } },
				{ "4", { "var4" } },
				{ "7", { "var6" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var10  var11  * " } },
				{ "14", { " var15  10  + " } },
				{ "4", { " var5  5  / " } },
				{ "7", { " var6  var6  * " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } },
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfWhileElseTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfWhileElseTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "2", { "2", "3", "4", "5" } },
				{ "3", { "2", "3", "4", "5" } },
				{ "4", { "2", "3", "4", "5" } },
				{ "5", { "2", "3", "4", "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "6", "7", "8" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "6" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "2" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var2", "var3", "var6", "var7" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var2", "var3", "var6", "var7" } },
				{ "2", { "var2", "var3", "var6" } },
				{ "3", { "var3" } },
				{ "5", { "var6" } },
				{ "6", { "var7" } },
				{ "8", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var4", "var5", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var4", "var5", "var8", "var9" } },
				{ "2", { "var4", "var5" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "5", { "var5" } },
				{ "8", { "var9" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "5", { " 1  var6  / " } },
				{ "8", { " 1  var10  % " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "2", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfWhileElseTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfWhileElseTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "3", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "4", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "5", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "3", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "5", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "6", "7", "8" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "2", "6", "7", "8", "9" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "2", { "3", "4", "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "9" } },
				{ "10", { "11" } },
				{ "2", { "3", "6" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "2" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var2", "var3", "var6", "var7", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var12", "var2", "var3", "var6", "var7", "var9" } },
				{ "11", { "var12" } },
				{ "2", { "var2", "var3", "var6" } },
				{ "3", { "var3" } },
				{ "5", { "var6" } },
				{ "6", { "var7" } },
				{ "8", { "var9" } },
				{ "9", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var4", "var5", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var10", "var11", "var4", "var5", "var8" } },
				{ "10", { "var10" } },
				{ "11", { "var11" } },
				{ "2", { "var4", "var5" } },
				{ "4", { "var4" } },
				{ "5", { "var5" } },
				{ "7", { "var8" } },
				{ "8", { "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var11" } },
				{ "5", { "var5" } },
				{ "8", { "var8" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var12  % " } },
				{ "5", { " 1  var6  + " } },
				{ "8", { " 9  var9  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "2", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfWhileElseMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfWhileElseMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "3", { "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "4", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "5", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "6", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "7", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "8", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "13", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "12", "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "10", "11", "3", "4", "5", "9" } },
				{ "3", { "10", "11", "4", "5", "9" } },
				{ "4", { "10", "11", "5", "9" } },
				{ "5", { "10", "11", "9" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "12", "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{

				{ "proc1", { "var1", "var12", "var2", "var3", "var6", "var7", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var12", "var2", "var3", "var6", "var7", "var9" } },
				{ "11", { "var9" } },
				{ "12", { "var9" } },
				{ "14", { "var12" } },
				{ "2", { "var1" } },
				{ "4", { "var2" } },
				{ "5", { "var2", "var3", "var6" } },
				{ "6", { "var3" } },
				{ "8", { "var6" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var4", "var5", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var10", "var11", "var2", "var4", "var5", "var8" } },
				{ "10", { "var8" } },
				{ "11", { "var8" } },
				{ "13", { "var10" } },
				{ "14", { "var11" } },
				{ "3", { "var2" } },
				{ "4", { "var2" } },
				{ "5", { "var4", "var5" } },
				{ "7", { "var4" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "14", { "var11" } },
				{ "4", { "var2" } },
				{ "8", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 9  var9  - " } },
				{ "14", { " 1  var12  / " } },
				{ "4", { " var2  3  * " } },
				{ "8", { " 1  var6  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "5", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfWhileElseBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfWhileElseBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "3", "4" ,"5", "6", "7", "8" } },
				{ "3", { "4" ,"5", "6", "7", "8" } },
				{ "4", { "5", "6", "7", "8" } },
				{ "5", { "5", "6", "7", "8" } },
				{ "6", { "5", "6", "7", "8" } },
				{ "7", { "5", "6", "7", "8" } },
				{ "8", { "5", "6", "7", "8" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "3", "4", "5" } },
				{ "3", { "4", "5" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "9" } },
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var6", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var3", "var6", "var9" } },
				{ "11", { "var9" } },
				{ "2", { "var1" } },
				{ "4", { "var2" } },
				{ "5", { "var2", "var3", "var6" } },
				{ "6", { "var3" } },
				{ "8", { "var6" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var4", "var5", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var2", "var4", "var5", "var7", "var8" } },
				{ "10", { "var7" } },
				{ "11", { "var8" } },
				{ "3", { "var2" } },
				{ "4", { "var2" } },
				{ "5", { "var4", "var5" } },
				{ "7", { "var4" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "4", { "var2" } },
				{ "8", { "var5" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var9  / " } },
				{ "4", { " var2  3  * " } },
				{ "8", { " 1  var6  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "5", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseWhileTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseWhileTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "5", "6", "7", "8" } },
				{ "6", { "5", "6", "7", "8" } },
				{ "7", { "5", "6", "7", "8" } },
				{ "8", { "5", "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var2", "var5", "var6", "var7" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var2", "var5", "var6", "var7" } },
				{ "2", { "var2" } },
				{ "4", { "var5" } },
				{ "5", { "var10", "var6", "var7" } },
				{ "6", { "var7" } },
				{ "8", { "var10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var3", "var4", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var3", "var4", "var8", "var9" } },
				{ "3", { "var3" } },
				{ "4", { "var4" } },
				{ "5", { "var8", "var9" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "4", { "var4" } },
				{ "8", { "var9" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "4", { " 1  var5  * " } },
				{ "8", { " 1  var10  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "5", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseWhileTopTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseWhileTopTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "6", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "7", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "8", { "5", "6", "7", "8", "9", "10", "11" } },
				{ "9", { "10", "11" } },
				{ "10", { "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15",
				"var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "3", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "10", "11", "9" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "6", "7", "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6", "9" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var14", "var15", "var2", "var5", "var6", "var7" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var11", "var14", "var15", "var2", "var5", "var6", "var7" } },
				{ "11", { "var14", "var15" } },
				{ "2", { "var2" } },
				{ "4", { "var5" } },
				{ "5", { "var10", "var6", "var7" } },
				{ "6", { "var7" } },
				{ "8", { "var10" } },
				{ "9", { "var11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var12", "var13", "var3", "var4", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var12", "var13", "var3", "var4", "var8", "var9" } },
				{ "10", { "var12" } },
				{ "11", { "var13" } },
				{ "3", { "var3" } },
				{ "4", { "var4" } },
				{ "5", { "var8", "var9" } },
				{ "7", { "var8" } },
				{ "8", { "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var13" } },
				{ "4", { "var4" } },
				{ "8", { "var9" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var14  var15  % " } },
				{ "4", { " 1  var5  * " } },
				{ "8", { " 1  var10  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "5", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseWhileMiddleTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseWhileMiddleTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "6", { "7", "8", "9", "10", "11", "12", "13", "14" } },
				{ "7", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "8", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "9", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "10", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "11", { "8", "9", "10", "11", "12", "13", "14" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = 
			{
				"var1", "var10", "var11", "var12", "var13", "var14", "var15",
				"var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9"
			};
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "13", "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "12", "2", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "14", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "12", "13", "14", "6", "7", "8" } },
				{ "6", { "12", "13", "14", "7", "8" } },
				{ "7", { "12", "13", "14", "8" } },
				{ "8", { "12", "13", "14" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "12", "13", "14", "2", "3", "4", "5", "6", "7", "8" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "8" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var11", "var14", "var15", "var2", "var5", "var6", "var7" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var11", "var14", "var15", "var2", "var5", "var6", "var7" } },
				{ "11", { "var10" } },
				{ "12", { "var11" } },
				{ "14", { "var14", "var15" } },
				{ "2", { "var2" } },
				{ "4", { "var5" } },
				{ "5", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var10", "var7" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var12", "var13", "var3", "var4", "var5", "var6", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var12", "var13", "var3", "var4", "var5", "var6", "var8", "var9" } },
				{ "10", { "var8" } },
				{ "11", { "var9" } },
				{ "13", { "var12" } },
				{ "14", { "var13" } },
				{ "3", { "var3" } },
				{ "4", { "var4" } },
				{ "6", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var9" } },
				{ "14", { "var13" } },
				{ "4", { "var4" } },
				{ "7", { "var6" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var10  + " } },
				{ "14", { " var14  var15  % " } },
				{ "4", { " 1  var5  * " } },
				{ "7", { " var6  3  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(nestedIfElseWhileBottomTest)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/nestedIfElseWhileBottomTest.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3", "4" ,"5", "6", "7", "8", "9", "10", "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8", "9", "10", "11" } },
				{ "6", { "7", "8", "9", "10", "11" } },
				{ "7", { "8", "9", "10", "11" } },
				{ "8", { "8", "9", "10", "11" } },
				{ "9", { "8", "9", "10", "11" } },
				{ "10", { "8", "9", "10", "11" } },
				{ "11", { "8", "9", "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "3", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "2", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "4", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11" } },
				{ "2", { "3", "4" } },
				{ "3", { "4" } },
				{ "5", { "6", "7", "8" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "10", "11", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "10", "11", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "5" } },
				{ "10", { "11" } },
				{ "11", { "8" } },
				{ "2", { "3" } },
				{ "3", { "4" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var10", "var2", "var5", "var6", "var7" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var10", "var2", "var5", "var6", "var7" } },
				{ "11", { "var10" } },
				{ "2", { "var2" } },
				{ "4", { "var5" } },
				{ "5", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var10", "var7" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var3", "var4", "var5", "var6", "var8", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var3", "var4", "var5", "var6", "var8", "var9" } },
				{ "10", { "var8" } },
				{ "11", { "var9" } },
				{ "3", { "var3" } },
				{ "4", { "var4" } },
				{ "6", { "var5" } },
				{ "7", { "var6" } },
				{ "8", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var9" } },
				{ "4", { "var4" } },
				{ "7", { "var6" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var10  + " } },
				{ "4", { " 1  var5  * " } },
				{ "7", { " var6  3  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "8", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "1", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(complicated1Test)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/complicated1Test.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "2", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "3", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "4", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "6", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "7", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "9", { "1", "10", "2", "3", "4", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var11", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "0", "1", "10", "2", "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "6", "8", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1", "2", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "10" } },
				{ "2", { "9" } },
				{ "4", { "5" } },
				{ "5", { "6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "10" } },
				{ "2", { "9" } },
				{ "4", { "5", "6" } },
				{ "5", { "6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "9" } },
				{ "2", { "3" } },
				{ "3", { "4", "5", "6", "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } },
				{ "3", { "4", "5", "6", "7", "8" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "10", "2" } },
				{ "2", { "3", "9" } },
				{ "3", { "4", "7" } },
				{ "4", { "5" } },
				{ "5", { "6" } },
				{ "6", { "2" } },
				{ "7", { "2", "8" } },
				{ "8", { "7" } },
				{ "9", { "1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var5", "var6", "var7", "var8" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var3", "var5", "var6", "var7", "var8" } },
				{ "2", { "var1", "var2", "var3", "var5", "var6", "var7", "var8" } },
				{ "3", { "var1", "var2", "var3", "var5", "var6", "var7", "var8" } },
				{ "5", { "var5" } },
				{ "6", { "var6", "var7" } },
				{ "7", { "var1", "var2", "var3", "var8" } },
				{ "8", { "var1", "var2", "var3" } },
				{ "9", { "var1", "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var11", "var2", "var4", "var6", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var11", "var4", "var6", "var9" } },
				{ "10", { "var2" } },
				{ "2", { "var4", "var6", "var9" } },
				{ "3", { "var4", "var6", "var9" } },
				{ "4", { "var4" } },
				{ "6", { "var6" } },
				{ "7", { "var9" } },
				{ "8", { "var9" } },
				{ "9", { "var11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "6", { "var6" } },
				{ "8", { "var9" } },
				{ "9", { "var11" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "6", { " var6  var7  +  2  - " } },
				{ "8", { " var1  var2  /  var3  *  10  % " } },
				{ "9", { " var1  var2  +  2  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } },
				{ "2", { "var2" } },
				{ "7", { "var1", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "3", { "var3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(complicated2Test)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/complicated2Test.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "10", { "11", "12", "13", "14", "15", "16", "24" } },
				{ "11", { "12", "13", "14", "15", "24" } },
				{ "12", { "13", "14", "24" } },
				{ "13", { "24" } },
				{ "14", { "24" } },
				{ "15", { "24" } },
				{ "16", { "24" } },
				{ "17", { "18", "19", "20", "21", "22", "23", "24" } },
				{ "18", { "19", "20", "21", "22", "24" } },
				{ "19", { "20", "21", "24" } },
				{ "2", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "20", { "24" } },
				{ "21", { "24" } },
				{ "22", { "24" } },
				{ "23", { "24" } },
				{ "3", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "4", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "6", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "7", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "3", "4", "5", "6", "7", "8", "9" } },
				{ "9", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "x", "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "0", "1", "2", "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "15", "20", "24", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "14", "21", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "13", "16", "2", "22", "23", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "1", "3", "4", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "10", "11", "12", "17", "18", "19", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "9" } },
				{ "2", { "3" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "9", { "24" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "24", "9" } },
				{ "2", { "3" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
				{ "9", { "24" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "1", { "2", "3" } },
				{ "10", { "11", "16" } },
				{ "11", { "12", "15" } },
				{ "12", { "13", "14" } },
				{ "17", { "18", "23" } },
				{ "18", { "19", "22" } },
				{ "19", { "20", "21" } },
				{ "3", { "4" } },
				{ "4", { "5" } },
				{ "5", { "6", "7", "8" } },
				{ "9", { "10", "17" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "1", { "2", "3", "4", "5", "6", "7", "8" } },
				{ "10", { "11", "12", "13", "14", "15", "16" } },
				{ "11", { "12", "13", "14", "15" } },
				{ "12", { "13", "14" } },
				{ "17", { "18", "19", "20", "21", "22", "23" } },
				{ "18", { "19", "20", "21", "22" } },
				{ "19", { "20", "21" } },
				{ "3", { "4", "5", "6", "7", "8" } },
				{ "4", { "5", "6", "7", "8" } },
				{ "5", { "6", "7", "8" } },
				{ "9", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2", "9" } },
				{ "10", { "11", "16" } },
				{ "11", { "12", "15" } },
				{ "12", { "13", "14" } },
				{ "13", { "24" } },
				{ "14", { "24" } },
				{ "15", { "24" } },
				{ "16", { "24" } },
				{ "17", { "18", "23" } },
				{ "18", { "19", "22" } },
				{ "19", { "20", "21" } },
				{ "2", { "3" } },
				{ "20", { "24" } },
				{ "21", { "24" } },
				{ "22", { "24" } },
				{ "23", { "24" } },
				{ "3", { "1", "4" } },
				{ "4", { "3", "5" } },
				{ "5", { "4", "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } },
				{ "9", { "10", "17" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var4", "var5", "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var2", "var3", "var4", "var5" } },
				{ "10", { "var2", "var3", "var4", "var5", "var7", "var8", "var9" } },
				{ "11", { "var2", "var3", "var4", "var5" } },
				{ "12", { "var2", "var4", "var5" } },
				{ "14", { "var5" } },
				{ "16", { "var7", "var8", "var9" } },
				{ "17", { "var1", "var2", "var3", "var4" } },
				{ "18", { "var2", "var3", "var4" } },
				{ "19", { "var2", "var3", "var4" } },
				{ "2", { "var1", "var2", "var3" } },
				{ "21", { "var2" } },
				{ "22", { "var2", "var3" } },
				{ "3", { "var2", "var3", "var4", "var5" } },
				{ "4", { "var2", "var3", "var4", "var5" } },
				{ "5", { "var2", "var3", "var5" } },
				{ "7", { "var5" } },
				{ "8", { "var2", "var3" } },
				{ "9", { "var1", "var2", "var3", "var4", "var5", "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var2", "var4", "var5", "var6", "x" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var1", "var2", "var4" } },
				{ "10", { "var5", "var6" } },
				{ "11", { "var5", "var6" } },
				{ "12", { "var5" } },
				{ "13", { "var5" } },
				{ "15", { "var6" } },
				{ "16", { "var6" } },
				{ "17", { "var1", "var5", "var6" } },
				{ "18", { "var1", "var5" } },
				{ "19", { "var1" } },
				{ "2", { "var2" } },
				{ "20", { "var1" } },
				{ "22", { "var5" } },
				{ "23", { "var6" } },
				{ "24", { "x" } },
				{ "3", { "var1", "var4" } },
				{ "4", { "var1", "var4" } },
				{ "5", { "var1", "var4" } },
				{ "6", { "var1" } },
				{ "8", { "var4" } },
				{ "9", { "var1", "var5", "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "13", { "var5" } },
				{ "16", { "var6" } },
				{ "2", { "var2" } },
				{ "22", { "var5" } },
				{ "23", { "var6" } },
				{ "8", { "var4" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "13", { " 3  3  * " } },
				{ "16", { " var7  var8  *  var9  % " } },
				{ "2", { " var1  var2  +  var3  - " } },
				{ "22", { " var3  var2  0  /  - " } },
				{ "23", { " 2  2  % " } },
				{ "8", { " var2  var3  %  2  / " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "1", { "var1" } },
				{ "3", { "var2", "var3" } },
				{ "4", { "var3", "var4" } },
				{ "5", { "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "10", { "var2" } },
				{ "11", { "var3" } },
				{ "12", { "var2", "var4" } },
				{ "17", { "var1" } },
				{ "18", { "var2" } },
				{ "19", { "var3", "var4" } },
				{ "9", { "var1", "var2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(complicated3Test)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/complicated3Test.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "10", { "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "11", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "12", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "13", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "14", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "15", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "16", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "17", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "18", { "12", "13", "14", "15", "16", "17", "18", "27" } },
				{ "19", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "2", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "20", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "21", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "22", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "23", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "24", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "25", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "26", { "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "3", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "4", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "5", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "6", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "7", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } },
				{ "9", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "2", "20", "21", "22", "23", "24", "25", "26", "27", "3", "4", "5", "6", "7", "8", "9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "x", "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "15", "23", "27", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "1", "16", "24", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "11", "17", "18", "19", "25", "26", "6", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "12", "13", "14", "2", "20", "21" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "10", "22", "3", "4", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "10", { "27" } },
				{ "11", { "12" } },
				{ "13", { "18" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "19", { "20" } },
				{ "2", { "10" } },
				{ "21", { "26" } },
				{ "24", { "25" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "10", "2", "27" } },
				{ "10", { "27" } },
				{ "11", { "12" } },
				{ "13", { "18" } },
				{ "15", { "16", "17" } },
				{ "16", { "17" } },
				{ "19", { "20" } },
				{ "2", { "10", "27" } },
				{ "21", { "26" } },
				{ "24", { "25" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "10", { "11", "12", "19", "20" } },
				{ "12", { "13", "18" } },
				{ "13", { "14" } },
				{ "14", { "15", "16", "17" } },
				{ "2", { "3" } },
				{ "20", { "21", "26" } },
				{ "21", { "22" } },
				{ "22", { "23", "24", "25" } },
				{ "3", { "4", "9" } },
				{ "4", { "5", "8" } },
				{ "5", { "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "10", { "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26" } },
				{ "12", { "13", "14", "15", "16", "17", "18" } },
				{ "13", { "14", "15", "16", "17" } },
				{ "14", { "15", "16", "17" } },
				{ "2", { "3", "4", "5", "6", "7", "8", "9" } },
				{ "20", { "21", "22", "23", "24", "25", "26" } },
				{ "21", { "22", "23", "24", "25" } },
				{ "22", { "23", "24", "25" } },
				{ "3", { "4", "5", "6", "7", "8", "9" } },
				{ "4", { "5", "6", "7", "8" } },
				{ "5", { "6", "7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable = { };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11", "19" } },
				{ "11", { "12" } },
				{ "12", { "13", "27" } },
				{ "13", { "14", "18" } },
				{ "14", { "13", "15" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "17", { "14" } },
				{ "18", { "12" } },
				{ "19", { "20" } },
				{ "2", { "10", "3" } },
				{ "20", { "21", "27" } },
				{ "21", { "22", "26" } },
				{ "22", { "23", "24" } },
				{ "23", { "21" } },
				{ "24", { "25" } },
				{ "25", { "21" } },
				{ "26", { "20" } },
				{ "3", { "4", "9" } },
				{ "4", { "5", "8" } },
				{ "5", { "6", "7" } },
				{ "6", { "2" } },
				{ "7", { "2" } },
				{ "8", { "2" } },
				{ "9", { "2" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var4", "var5", "var7", "var8", "var9" } } };
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "10", { "var1", "var2", "var3", "var4", "var5", "var7" } },
				{ "11", { "var1", "var2", "var3" } },
				{ "12", { "var2", "var3", "var4", "var5" } },
				{ "13", { "var2", "var3", "var4", "var5" } },
				{ "14", { "var2", "var3", "var5" } },
				{ "16", { "var5" } },
				{ "17", { "var2", "var3" } },
				{ "18", { "var4" } },
				{ "19", { "var1", "var2", "var3" } },
				{ "2", { "var1", "var2", "var3", "var4", "var5", "var7", "var8", "var9" } },
				{ "20", { "var2", "var3", "var4", "var5", "var7" } },
				{ "21", { "var2", "var3", "var4", "var5" } },
				{ "22", { "var2", "var3", "var5" } },
				{ "24", { "var5" } },
				{ "25", { "var2", "var3" } },
				{ "26", { "var7" } },
				{ "3", { "var2", "var3", "var4", "var5", "var7", "var8", "var9" } },
				{ "4", { "var2", "var3", "var4", "var5" } },
				{ "5", { "var2", "var4", "var5" } },
				{ "7", { "var5" } },
				{ "9", { "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var2", "var4", "var5", "var6", "x" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var1", "var2", "var4", "var6" } },
				{ "11", { "var2" } },
				{ "12", { "var1", "var4" } },
				{ "13", { "var1", "var4" } },
				{ "14", { "var1", "var4" } },
				{ "15", { "var1" } },
				{ "17", { "var4" } },
				{ "18", { "var4" } },
				{ "19", { "var2" } },
				{ "2", { "var5", "var6" } },
				{ "20", { "var1", "var4", "var6" } },
				{ "21", { "var1", "var4" } },
				{ "22", { "var1", "var4" } },
				{ "23", { "var1" } },
				{ "25", { "var4" } },
				{ "26", { "var6" } },
				{ "27", { "x" } },
				{ "3", { "var5", "var6" } },
				{ "4", { "var5", "var6" } },
				{ "5", { "var5" } },
				{ "6", { "var5" } },
				{ "8", { "var6" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var2" } },
				{ "17", { "var4" } },
				{ "18", { "var4" } },
				{ "19", { "var2" } },
				{ "25", { "var4" } },
				{ "26", { "var6" } },
				{ "6", { "var5" } },
				{ "9", { "var6" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " var1  var2  +  var3  - " } },
				{ "17", { " var2  var3  %  2  / " } },
				{ "18", { " var4 " } },
				{ "19", { " var1  var2  +  var3  - " } },
				{ "25", { " var2  var3  %  2  / " } },
				{ "26", { " var7 " } },
				{ "6", { " 3  3  * " } },
				{ "9", { " var7  var8  *  var9  % " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "12", { "var2", "var3" } },
				{ "13", { "var3", "var4" } },
				{ "14", { "var2" } },
				{ "2", { "var1" } },
				{ "20", { "var2", "var3" } },
				{ "21", { "var3", "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "10", { "var1", "var2" } },
				{ "22", { "var2" } },
				{ "3", { "var2" } },
				{ "4", { "var3" } },
				{ "5", { "var2", "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(multiProc1Test)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/multiProc1Test.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "10", { "11", "12", "13", "14", "15", "16", "17", "18", "19" } },
				{ "11", { "12", "13", "14", "15", "16", "17", "18", "19" } },
				{ "12", { "13", "14", "15", "16", "17", "18", "19" } },
				{ "13", { "14", "15", "16", "17", "18", "19" } },
				{ "14", { "15", "16" } },
				{ "15", { "16" } },
				{ "17", { "18", "19" } },
				{ "18", { "19" } },
				{ "2", { "3", "4", "5", "6", "7", "8" } },
				{ "20", { "21", "22" } },
				{ "21", { "22" } },
				{ "23", { "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "24", { "25", "26", "27", "28", "29", "30" } },
				{ "25", { "26", "27", "28", "29", "30" } },
				{ "26", { "27", "28", "29", "30" } },
				{ "27", { "27", "28", "29", "30" } },
				{ "28", { "27", "28", "29", "30" } },
				{ "29", { "27", "28", "29", "30" } },
				{ "3", { "4", "5", "6", "7", "8" } },
				{ "30", { "27", "28", "29", "30" } },
				{ "31", { "32", "33" } },
				{ "32", { "33" } },
				{ "4", { "5", "6", "7", "8" } },
				{ "5", { "5", "6", "7", "8" } },
				{ "6", { "5", "6", "7", "8" } },
				{ "7", { "5", "6", "7", "8" } },
				{ "8", { "5", "6", "7", "8" } },
				{ "9", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1", "proc2", "proc3", "proc4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "10", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable = { "10", "14", "17", "20", "25", "29", "3", "32" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable = { "11", "15", "2", "21", "24", "28", "31", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable = { "12", "16", "19", "22", "26", "30", "33", "4", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable = { "1", "18", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable = { "27", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable = { "13", "23", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "14", { "15" } },
				{ "15", { "16" } },
				{ "17", { "18" } },
				{ "18", { "19" } },
				{ "2", { "3" } },
				{ "20", { "21" } },
				{ "21", { "22" } },
				{ "24", { "25" } },
				{ "25", { "26" } },
				{ "26", { "27" } },
				{ "28", { "29" } },
				{ "29", { "30" } },
				{ "3", { "4" } },
				{ "31", { "32" } },
				{ "32", { "33" } },
				{ "4", { "5" } },
				{ "6", { "7" } },
				{ "7", { "8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "10", { "11", "12", "13" } },
				{ "11", { "12", "13" } },
				{ "12", { "13" } },
				{ "14", { "15", "16" } },
				{ "15", { "16" } },
				{ "17", { "18", "19" } },
				{ "18", { "19" } },
				{ "2", { "3", "4", "5" } },
				{ "20", { "21", "22" } },
				{ "21", { "22" } },
				{ "24", { "25", "26", "27" } },
				{ "25", { "26", "27" } },
				{ "26", { "27" } },
				{ "28", { "29", "30" } },
				{ "29", { "30" } },
				{ "3", { "4", "5" } },
				{ "31", { "32", "33" } },
				{ "32", { "33" } },
				{ "4", { "5" } },
				{ "6", { "7", "8" } },
				{ "7", { "8" } },
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "13", { "14", "15", "16", "17", "18", "19" } },
				{ "23", { "24", "25", "26", "27", "31", "32", "33" } },
				{ "27", { "28", "29", "30" } },
				{ "5", { "6", "7", "8" } },
				{ "9", { "10", "11", "12", "13", "20", "21", "22" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "13", { "14", "15", "16", "17", "18", "19" } },
				{ "23", { "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "27", { "28", "29", "30" } },
				{ "5", { "6", "7", "8" } },
				{ "9", { "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);
			
			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expecetedCallTable =
			{
				{ "proc1", { "proc4" } },
				{ "proc2", { "proc3" } },
				{ "proc3", { "proc4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS) == expecetedCallTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable =
			{
				{ "proc1", { "proc4" } },
				{ "proc2", { "proc3", "proc4" } },
				{ "proc3", { "proc4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "10", { "11" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "13", { "14", "17" } },
				{ "14", { "15" } },
				{ "15", { "16" } },
				{ "17", { "18" } },
				{ "18", { "19" } },
				{ "2", { "3" } },
				{ "20", { "21" } },
				{ "21", { "22" } },
				{ "23", { "24", "31" } },
				{ "24", { "25" } },
				{ "25", { "26" } },
				{ "26", { "27" } },
				{ "27", { "28" } },
				{ "28", { "29" } },
				{ "29", { "30" } },
				{ "3", { "4" } },
				{ "30", { "27" } },
				{ "31", { "32" } },
				{ "32", { "33" } },
				{ "4", { "5" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "8" } },
				{ "8", { "5" } },
				{ "9", { "10", "20" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var12", "var2", "var3", "var6", "var9" } },
				{ "proc2", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var8", "var9" } },
				{ "proc3", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var9" } },
				{ "proc4", { "var1", "var12", "var2", "var3", "var6", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1", "var12", "var2", "var3", "var6", "var9" } },
				{ "11", { "var2" } },
				{ "12", { "var3" } },
				{ "13", { "var1", "var12", "var2", "var3", "var4", "var5", "var6", "var9" } },
				{ "15", { "var4" } },
				{ "16", { "var4", "var5" } },
				{ "18", { "var1", "var12", "var2", "var3", "var6", "var9" } },
				{ "19", { "var9" } },
				{ "2", { "var1" } },
				{ "21", { "var12" } },
				{ "22", { "var13" } },
				{ "23", { "var1", "var12", "var2", "var3", "var6", "var9" } },
				{ "24", { "var1" } },
				{ "26", { "var2" } },
				{ "27", { "var2", "var3", "var6" } },
				{ "28", { "var3" } },
				{ "30", { "var6" } },
				{ "31", { "var9" } },
				{ "33", { "var12" } },
				{ "4", { "var4" } },
				{ "5", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var8", "var9" } },
				{ "6", { "var5" } },
				{ "7", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var9" } },
				{ "8", { "var8" } },
				{ "9", { "var1", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var10", "var11", "var2", "var4", "var5" } },
				{ "proc2", { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8" } },
				{ "proc3", { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var8" } },
				{ "proc4", { "var10", "var11", "var2", "var4", "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "1", { "var10", "var11", "var2", "var4", "var5" } },
				{ "10", { "var1" } },
				{ "12", { "var3" } },
				{ "13", { "var10", "var11", "var2", "var3", "var4", "var5", "var6", "var8" } },
				{ "14", { "var3" } },
				{ "16", { "var4" } },
				{ "17", { "var6" } },
				{ "18", { "var10", "var11", "var2", "var4", "var5" } },
				{ "19", { "var8" } },
				{ "20", { "var11" } },
				{ "22", { "var12" } },
				{ "23", { "var10", "var11", "var2", "var4", "var5" } },
				{ "25", { "var2" } },
				{ "26", { "var2" } },
				{ "27", { "var4", "var5" } },
				{ "29", { "var4" } },
				{ "3", { "var2" } },
				{ "30", { "var5" } },
				{ "32", { "var10" } },
				{ "33", { "var11" } },
				{ "4", { "var3" } },
				{ "5", { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var8" } },
				{ "7", { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var8" } },
				{ "8", { "var7" } },
				{ "9", { "var1", "var10", "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "12", { "var3" } },
				{ "16", { "var4" } },
				{ "19", { "var8" } },
				{ "22", { "var12" } },
				{ "26", { "var2" } },
				{ "30", { "var5" } },
				{ "33", { "var11" } },
				{ "4", { "var3" } },
				{ "8", { "var7" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "12", { " var3  var3  * " } },
				{ "16", { " var4  var5  * " } },
				{ "19", { " var9  10  + " } },
				{ "22", { " var13  5  / " } },
				{ "26", { " var2  3  * " } },
				{ "30", { " 1  var6  + " } },
				{ "33", { " 1  var12  / " } },
				{ "4", { " 1  var4  + " } },
				{ "8", { " 1  var8  % " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "27", { "var2" } },
				{ "5", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "13", { "var3" } },
				{ "23", { "var1" } },
				{ "9", { "var1" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(multiProc2Test)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/multiProc2Test.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "2", "3" } },
				{ "11", { "12", "13" } },
				{ "12", { "13" } },
				{ "14", { "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "15", { "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "16", { "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "17", { "18", "19", "20", "21", "22", "23", "24", "25", "26", "27" } },
				{ "18", { "19", "20", "24", "25", "26", "27" } },
				{ "19", { "20", "24", "25", "26", "27" } },
				{ "2", { "3" } },
				{ "20", { "24", "25", "26", "27" } },
				{ "21", { "22", "23", "24", "25", "26", "27" } },
				{ "22", { "23", "24", "25", "26", "27" } },
				{ "23", { "24", "25", "26", "27" } },
				{ "24", { "24", "25", "26", "27" } },
				{ "25", { "24", "25", "26", "27" } },
				{ "26", { "24", "25", "26", "27" } },
				{ "27", { "24", "25", "26", "27" } },
				{ "28", { "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41" } },
				{ "29", { "30", "31" } },
				{ "30", { "31" } },
				{ "32", { "33", "34", "35", "36", "37", "38", "39", "40", "41" } },
				{ "33", { "34", "35", "36", "37", "38", "39", "40", "41" } },
				{ "34", { "35", "36", "37", "38", "39", "40", "41" } },
				{ "35", { "36", "37", "38", "39", "40", "41" } },
				{ "36", { "37", "38" } },
				{ "37", { "38" } },
				{ "39", { "40", "41" } },
				{ "4", { "10", "11", "12", "13", "5", "6", "7", "8", "9" } },
				{ "40", { "41" } },
				{ "5", { "10", "11", "12", "13", "6", "7", "8", "9" } },
				{ "6", { "10", "11", "12", "13", "7", "8", "9" } },
				{ "7", { "10", "11", "12", "13", "8", "9" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1", "proc2", "proc3", "proc4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var11", "var12", "var13", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "2", "3", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable =
			{ "15", "19", "2", "22", "29", "32", "39", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable =
			{ "1", "11", "14", "18", "21", "25", "33", "37", "4", "40", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable =
			{ "10", "13", "16", "20", "23", "27", "3", "31", "34", "38", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable =
			{ "12", "26", "30", "36", "41" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable =
			{ "24" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable =
			{ "17", "28", "35", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "14", { "15" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "17", { "24" } },
				{ "18", { "19" } },
				{ "19", { "20" } },
				{ "2", { "3" } },
				{ "21", { "22" } },
				{ "22", { "23" } },
				{ "25", { "26" } },
				{ "26", { "27" } },
				{ "29", { "30" } },
				{ "30", { "31" } },
				{ "32", { "33" } },
				{ "33", { "34" } },
				{ "34", { "35" } },
				{ "36", { "37" } },
				{ "37", { "38" } },
				{ "39", { "40" } },
				{ "4", { "5" } },
				{ "40", { "41" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "2", "3" } },
				{ "11", { "12", "13" } },
				{ "12", { "13" } },
				{ "14", { "15", "16", "17", "24" } },
				{ "15", { "16", "17", "24" } },
				{ "16", { "17", "24" } },
				{ "17", { "24" } },
				{ "18", { "19", "20" } },
				{ "19", { "20" } },
				{ "2", { "3" } },
				{ "21", { "22", "23" } },
				{ "22", { "23" } },
				{ "25", { "26", "27" } },
				{ "26", { "27" } },
				{ "29", { "30", "31" } },
				{ "30", { "31" } },
				{ "32", { "33", "34", "35" } },
				{ "33", { "34", "35" } },
				{ "34", { "35" } },
				{ "36", { "37", "38" } },
				{ "37", { "38" } },
				{ "39", { "40", "41" } },
				{ "4", { "5", "6", "7" } },
				{ "40", { "41" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "8", { "10", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "17", { "18", "19", "20", "21", "22", "23" } },
				{ "24", { "25", "26", "27" } },
				{ "28", { "29", "30", "31", "32", "33", "34", "35" } },
				{ "35", { "36", "37", "38", "39", "40", "41" } },
				{ "7", { "10", "11", "12", "13", "8", "9" } },
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "17", { "18", "19", "20", "21", "22", "23" } },
				{ "24", { "25", "26", "27" } },
				{ "28", { "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41" } },
				{ "35", { "36", "37", "38", "39", "40", "41" } },
				{ "7", { "10", "11", "12", "13", "8", "9" } },
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expecetedCallTable =
			{
				{ "proc2", { "proc1" } },
				{ "proc3", { "proc2" } },
				{ "proc4", { "proc1", "proc2", "proc3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS) == expecetedCallTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable =
			{
				{ "proc2", { "proc1" } },
				{ "proc3", { "proc1", "proc2" } },
				{ "proc4", { "proc1", "proc2", "proc3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "11", { "12" } },
				{ "12", { "13" } },
				{ "14", { "15" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "17", { "18", "21" } },
				{ "18", { "19" } },
				{ "19", { "20" } },
				{ "2", { "3" } },
				{ "20", { "24" } },
				{ "21", { "22" } },
				{ "22", { "23" } },
				{ "23", { "24" } },
				{ "24", { "25" } },
				{ "25", { "26" } },
				{ "26", { "27" } },
				{ "27", { "24" } },
				{ "28", { "29", "32" } },
				{ "29", { "30" } },
				{ "30", { "31" } },
				{ "32", { "33" } },
				{ "33", { "34" } },
				{ "34", { "35" } },
				{ "35", { "36", "39" } },
				{ "36", { "37" } },
				{ "37", { "38" } },
				{ "39", { "40" } },
				{ "4", { "5" } },
				{ "40", { "41" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "11", "8" } },
				{ "8", { "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var4" } },
				{ "proc2", { "var1", "var12", "var4", "var5", "var8", "var9" } },
				{ "proc3", { "var1", "var11", "var12", "var2", "var4", "var5", "var8", "var9" } },
				{ "proc4", { "var1", "var10", "var11", "var12", "var13", "var2", "var4", "var5", "var6", "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "10", { "var8" } },
				{ "11", { "var9" } },
				{ "12", { "var1", "var4" } },
				{ "13", { "var12" } },
				{ "14", { "var2" } },
				{ "16", { "var4" } },
				{ "17", { "var2", "var4", "var5", "var8" } },
				{ "18", { "var2" } },
				{ "20", { "var4" } },
				{ "21", { "var5" } },
				{ "23", { "var8" } },
				{ "24", { "var1", "var11", "var12", "var4", "var5", "var8", "var9" } },
				{ "25", { "var9" } },
				{ "26", { "var1", "var12", "var4", "var5", "var8", "var9" } },
				{ "27", { "var11", "var12" } },
				{ "28", { "var1", "var10", "var11", "var12", "var13", "var2", "var4", "var5", "var6", "var7", "var8", "var9" } },
				{ "3", { "var4" } },
				{ "30", { "var1", "var4" } },
				{ "31", { "var5" } },
				{ "33", { "var5" } },
				{ "34", { "var6" } },
				{ "35", { "var1", "var10", "var11", "var12", "var13", "var2", "var4", "var5", "var7", "var8", "var9" } },
				{ "36", { "var1", "var12", "var4", "var5", "var8", "var9" } },
				{ "37", { "var8" } },
				{ "38", { "var10", "var11" } },
				{ "4", { "var1" } },
				{ "40", { "var13" } },
				{ "41", { "var1", "var11", "var12", "var2", "var4", "var5", "var8", "var9" } },
				{ "6", { "var4" } },
				{ "7", { "var1", "var12", "var4", "var5", "var8", "var9" } },
				{ "8", { "var5" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var2", "var3" } },
				{ "proc2", { "var11", "var2", "var3", "var6", "var7" } },
				{ "proc3", { "var11", "var2", "var3", "var6", "var7" } },
				{ "proc4", { "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var7" } },
				{ "12", { "var2", "var3" } },
				{ "13", { "var11" } },
				{ "15", { "var2" } },
				{ "16", { "var3" } },
				{ "17", { "var2", "var3", "var6", "var7" } },
				{ "19", { "var2" } },
				{ "2", { "var2" } },
				{ "20", { "var3" } },
				{ "22", { "var6" } },
				{ "23", { "var7" } },
				{ "24", { "var11", "var2", "var3", "var6", "var7" } },
				{ "26", { "var11", "var2", "var3", "var6", "var7" } },
				{ "27", { "var11" } },
				{ "28", { "var11", "var12", "var2", "var3", "var4", "var5", "var6", "var7", "var9" } },
				{ "29", { "var2" } },
				{ "3", { "var3" } },
				{ "30", { "var2", "var3" } },
				{ "31", { "var4" } },
				{ "32", { "var5" } },
				{ "34", { "var6" } },
				{ "35", { "var11", "var12", "var2", "var3", "var6", "var7", "var9" } },
				{ "36", { "var11", "var2", "var3", "var6", "var7" } },
				{ "38", { "var9" } },
				{ "39", { "var12" } },
				{ "41", { "var11", "var2", "var3", "var6", "var7" } },
				{ "5", { "var2" } },
				{ "6", { "var3" } },
				{ "7", { "var11", "var2", "var3", "var6", "var7" } },
				{ "9", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "10", { "var7" } },
				{ "13", { "var11" } },
				{ "16", { "var3" } },
				{ "20", { "var3" } },
				{ "23", { "var7" } },
				{ "27", { "var11" } },
				{ "3", { "var3" } },
				{ "31", { "var4" } },
				{ "34", { "var6" } },
				{ "38", { "var9" } },
				{ "6", { "var3" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "10", { " 1  var8  - " } },
				{ "13", { " 1  var12  / " } },
				{ "16", { " 1  var4  / " } },
				{ "20", { " 1  var4  / " } },
				{ "23", { " var8  var8  * " } },
				{ "27", { " var11  var12  % " } },
				{ "3", { " 1  var4  + " } },
				{ "31", { " var5  5  / " } },
				{ "34", { " var6  var6  * " } },
				{ "38", { " var10  var11  * " } },
				{ "6", { " 1  var4  + " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "24", { "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "17", { "var4" } },
				{ "28", { "var1" } },
				{ "35", { "var7" } },
				{ "7", { "var4" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(multiProc3Test)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/multiProc3Test.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateNextStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedNextStarTable =
			{
				{ "1", { "10", "11", "12", "13", "14", "2", "3", "4", "5", "6", "7", "8", "9" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "15", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "16", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "17", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "18", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "19", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "2", { "10", "11", "12", "13", "14", "3", "4", "5", "6", "7", "8", "9" } },
				{ "20", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "21", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "22", { "15", "16", "17", "18", "19", "20", "21", "22" } },
				{ "23", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "24", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "25", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "26", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "27", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "28", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "29", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "3", { "10", "11", "12", "13", "14", "4", "5", "6", "7", "8", "9" } },
				{ "30", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "31", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "32", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "33", { "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "34", { "35", "36", "37", "38", "39", "40", "41", "42", "43", "44" } },
				{ "35", { "36", "37" } },
				{ "36", { "37" } },
				{ "38", { "39", "40", "41", "42", "43", "44" } },
				{ "39", { "40", "41", "42", "43", "44" } },
				{ "4", { "10", "11", "12", "13", "14", "4", "5", "6", "7", "8", "9" } },
				{ "40", { "41", "42", "43", "44" } },
				{ "41", { "41", "42", "43", "44" } },
				{ "42", { "41", "42", "43", "44" } },
				{ "43", { "41", "42", "43", "44" } },
				{ "44", { "41", "42", "43", "44" } },
				{ "5", { "10", "11", "12", "13", "14", "4", "5", "6", "7", "8", "9" } },
				{ "6", { "10", "11", "12", "13", "14", "4", "5", "6", "7", "8", "9" } },
				{ "7", { "10", "11", "12", "13", "14", "4", "5", "6", "7", "8", "9" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT_STAR) == expectedNextStarTable);

			PROC_NAMES expectedProc = { "proc1", "proc2", "proc3", "proc4" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PROCEDURE) == expectedProc);

			VAR_NAMES expectedVar = { "var1", "var10", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::VARIABLE) == expectedVar);

			CONSTS expectedConst = { "1", "11", "2", "3", "5" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CONSTANT) == expectedConst);

			STMT_NUMS expectedReadTable =
			{ "10", "13", "17", "2", "21", "24", "28", "31", "36", "6" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::READ) == expectedReadTable);

			STMT_NUMS expectedPrintTable =
			{ "1", "12", "16", "20", "25", "29", "32", "35", "38", "42", "5", "9" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::PRINT) == expectedPrintTable);

			STMT_NUMS expectedAssignTable =
			{ "11", "18", "22", "26", "3", "33", "37", "40" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::ASSIGN) == expectedAssignTable);

			STMT_NUMS expectedCallTable =
			{ "14", "30", "39", "43", "44", "7" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::CALL) == expectedCallTable);

			STMT_NUMS expectedWhileTable =
			{ "15", "19", "23", "4", "41" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::WHILE) == expectedWhileTable);

			STMT_NUMS expectedIfTable =
			{ "27", "34", "8" };
			Assert::IsTrue(getStringDEValuesOf(DESIGN_ENTITY::IF) == expectedIfTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "16", { "17" } },
				{ "17", { "18" } },
				{ "18", { "19" } },
				{ "2", { "3" } },
				{ "20", { "21" } },
				{ "21", { "22" } },
				{ "24", { "25" } },
				{ "25", { "26" } },
				{ "26", { "27" } },
				{ "28", { "29" } },
				{ "29", { "30" } },
				{ "3", { "4" } },
				{ "31", { "32" } },
				{ "32", { "33" } },
				{ "35", { "36" } },
				{ "36", { "37" } },
				{ "38", { "39" } },
				{ "39", { "40" } },
				{ "4", { "8" } },
				{ "40", { "41" } },
				{ "42", { "43" } },
				{ "43", { "44" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS) == expectedFTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedFStarTable =
			{
				{ "1", { "2", "3", "4", "8" } },
				{ "10", { "11" } },
				{ "12", { "13", "14" } },
				{ "13", { "14" } },
				{ "16", { "17", "18", "19" } },
				{ "17", { "18", "19" } },
				{ "18", { "19" } },
				{ "2", { "3", "4", "8" } },
				{ "20", { "21", "22" } },
				{ "21", { "22" } },
				{ "24", { "25", "26", "27" } },
				{ "25", { "26", "27" } },
				{ "26", { "27" } },
				{ "28", { "29", "30" } },
				{ "29", { "30" } },
				{ "3", { "4", "8" } },
				{ "31", { "32", "33" } },
				{ "32", { "33" } },
				{ "35", { "36", "37" } },
				{ "36", { "37" } },
				{ "38", { "39", "40", "41" } },
				{ "39", { "40", "41" } },
				{ "4", { "8" } },
				{ "40", { "41" } },
				{ "42", { "43", "44" } },
				{ "43", { "44" } },
				{ "5", { "6", "7" } },
				{ "6", { "7" } },
				{ "9", { "10", "11" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::FOLLOWS_STAR) == expectedFStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentTable =
			{
				{ "15", { "16", "17", "18", "19" } },
				{ "19", { "20", "21", "22" } },
				{ "23", { "24", "25", "26", "27" } },
				{ "27", { "28", "29", "30", "31", "32", "33" } },
				{ "34", { "35", "36", "37", "38", "39", "40", "41" } },
				{ "4", { "5", "6", "7" } },
				{ "41", { "42", "43", "44" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } },
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT) == expectedParentTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expectedParentStarTable =
			{
				{ "15", { "16", "17", "18", "19", "20", "21", "22" } },
				{ "19", { "20", "21", "22" } },
				{ "23", { "24", "25", "26", "27", "28", "29", "30", "31", "32", "33" } },
				{ "27", { "28", "29", "30", "31", "32", "33" } },
				{ "34", { "35", "36", "37", "38", "39", "40", "41", "42", "43", "44" } },
				{ "4", { "5", "6", "7" } },
				{ "41", { "42", "43", "44" } },
				{ "8", { "10", "11", "12", "13", "14", "9" } },
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::PARENT_STAR) == expectedParentStarTable);

			RELATIONSHIP_TABLE<STMT_NUM, STMT_NUM> expecetedCallTable =
			{
				{ "proc1", { "proc2", "proc3" } },
				{ "proc3", { "proc2" } },
				{ "proc4", { "proc1", "proc2", "proc3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS) == expecetedCallTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedCallStarTable =
			{
				{ "proc1", { "proc2", "proc3" } },
				{ "proc3", { "proc2" } },
				{ "proc4", { "proc1", "proc2", "proc3" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::CALLS_STAR) == expectedCallStarTable);

			RELATIONSHIP_TABLE<PROC_NAME, PROC_NAME> expectedNextTable =
			{
				{ "1", { "2" } },
				{ "10", { "11" } },
				{ "12", { "13" } },
				{ "13", { "14" } },
				{ "15", { "16" } },
				{ "16", { "17" } },
				{ "17", { "18" } },
				{ "18", { "19" } },
				{ "19", { "15", "20" } },
				{ "2", { "3" } },
				{ "20", { "21" } },
				{ "21", { "22" } },
				{ "22", { "19" } },
				{ "23", { "24" } },
				{ "24", { "25" } },
				{ "25", { "26" } },
				{ "26", { "27" } },
				{ "27", { "28", "31" } },
				{ "28", { "29" } },
				{ "29", { "30" } },
				{ "3", { "4" } },
				{ "30", { "23" } },
				{ "31", { "32" } },
				{ "32", { "33" } },
				{ "33", { "23" } },
				{ "34", { "35", "38" } },
				{ "35", { "36" } },
				{ "36", { "37" } },
				{ "38", { "39" } },
				{ "39", { "40" } },
				{ "4", { "5", "8" } },
				{ "40", { "41" } },
				{ "41", { "42" } },
				{ "42", { "43" } },
				{ "43", { "44" } },
				{ "44", { "41" } },
				{ "5", { "6" } },
				{ "6", { "7" } },
				{ "7", { "4" } },
				{ "8", { "12", "9" } },
				{ "9", { "10" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::NEXT) == expectedNextTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcUsesTable =
			{
				{ "proc1", { "var1", "var2", "var3", "var4", "var6", "var7", "var8", "var9" } },
				{ "proc2", { "var1", "var4", "var6" } },
				{ "proc3", { "var1", "var2", "var3", "var4", "var6", "var7", "var9" } },
				{ "proc4", { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_PROC) == expectedProcUsesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtUsesTable =
			{
				{ "1", { "var1" } },
				{ "11", { "var8" } },
				{ "12", { "var9" } },
				{ "14", { "var1", "var4", "var6" } },
				{ "15", { "var1", "var4", "var6" } },
				{ "16", { "var1" } },
				{ "18", { "var4" } },
				{ "19", { "var4", "var6" } },
				{ "20", { "var4" } },
				{ "22", { "var6" } },
				{ "23", { "var1", "var2", "var3", "var4", "var6", "var7", "var9" } },
				{ "25", { "var2" } },
				{ "26", { "var1", "var3" } },
				{ "27", { "var1", "var3", "var4", "var6", "var7", "var9" } },
				{ "29", { "var4" } },
				{ "3", { "var4" } },
				{ "30", { "var1", "var4", "var6" } },
				{ "32", { "var7" } },
				{ "33", { "var9" } },
				{ "34", { "var1", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9" } },
				{ "35", { "var2" } },
				{ "37", { "var5" } },
				{ "38", { "var5" } },
				{ "39", { "var1", "var2", "var3", "var4", "var6", "var7", "var8", "var9" } },
				{ "4", { "var1", "var2", "var3", "var4", "var6", "var7", "var9" } },
				{ "40", { "var6" } },
				{ "41", { "var1", "var2", "var3", "var4", "var6", "var7", "var9" } },
				{ "42", { "var7" } },
				{ "43", { "var1", "var4", "var6" } },
				{ "44", { "var1", "var2", "var3", "var4", "var6", "var7", "var9" } },
				{ "5", { "var4" } },
				{ "7", { "var1", "var2", "var3", "var4", "var6", "var7", "var9" } },
				{ "8", { "var1", "var4", "var6", "var7", "var8", "var9" } },
				{ "9", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::USES_STMT) == expectedStmtUsesTable);

			RELATIONSHIP_TABLE<PROC_NAME, VAR_NAME> expectedProcModifiesTable =
			{
				{ "proc1", { "var1", "var10", "var2", "var3", "var5", "var6", "var7", "var8" } },
				{ "proc2", { "var2", "var3", "var5" } },
				{ "proc3", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "proc4", { "var1", "var10", "var2", "var3", "var4", "var5", "var6", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_PROC) == expectedProcModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedStmtModifiesTable =
			{
				{ "10", { "var7" } },
				{ "11", { "var8" } },
				{ "13", { "var10" } },
				{ "14", { "var2", "var3", "var5" } },
				{ "15", { "var2", "var3", "var5" } },
				{ "17", { "var2" } },
				{ "18", { "var3" } },
				{ "19", { "var5" } },
				{ "2", { "var2" } },
				{ "21", { "var5" } },
				{ "22", { "var5" } },
				{ "23", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "24", { "var1" } },
				{ "26", { "var2" } },
				{ "27", { "var2", "var3", "var5", "var6", "var8" } },
				{ "28", { "var3" } },
				{ "3", { "var3" } },
				{ "30", { "var2", "var3", "var5" } },
				{ "31", { "var6" } },
				{ "33", { "var8" } },
				{ "34", { "var1", "var10", "var2", "var3", "var4", "var5", "var6", "var7", "var8" } },
				{ "36", { "var3" } },
				{ "37", { "var4" } },
				{ "39", { "var1", "var10", "var2", "var3", "var5", "var6", "var7", "var8" } },
				{ "4", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "40", { "var6" } },
				{ "41", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "43", { "var2", "var3", "var5" } },
				{ "44", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "6", { "var5" } },
				{ "7", { "var1", "var2", "var3", "var5", "var6", "var8" } },
				{ "8", { "var10", "var2", "var3", "var5", "var7", "var8" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::MODIFIES_STMT) == expectedStmtModifiesTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedAssVarTable =
			{
				{ "11", { "var8" } },
				{ "18", { "var3" } },
				{ "22", { "var5" } },
				{ "26", { "var2" } },
				{ "3", { "var3" } },
				{ "33", { "var8" } },
				{ "37", { "var4" } },
				{ "40", { "var6" } }
			};
			RELATIONSHIP_TABLE<STMT_NUM, PATTERN> expectedAssPatTable =
			{
				{ "11", { " 1  var8  * " } },
				{ "18", { " 1  var4  - " } },
				{ "22", { " 1  var6  - " } },
				{ "26", { " var1  var3  * " } },
				{ "3", { " 1  var4  / " } },
				{ "33", { " var9  5  / " } },
				{ "37", { " 1  var5  * " } },
				{ "40", { " var6  3  - " } }
			};
			Assert::IsTrue(convertTotStringRelationshipTable(pkb.getAssignVarStore()) == expectedAssVarTable);
			Assert::IsTrue(getStringAssignPatternTable() == expectedAssPatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedWhilePatTable =
			{
				{ "15", { "var1" } },
				{ "19", { "var4" } },
				{ "23", { "var1" } },
				{ "4", { "var4" } },
				{ "41", { "var7" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::WHILE_PATTERN) == expectedWhilePatTable);

			RELATIONSHIP_TABLE<STMT_NUM, VAR_NAME> expectedIfPatTable =
			{
				{ "27", { "var3" } },
				{ "34", { "var1" } },
				{ "8", { "var6" } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::IF_PATTERN) == expectedIfPatTable);
		}

		TEST_METHOD(affects_no_container)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/affects_no_container.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateAffectsCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedAffectsTable =
			{
				{"1", {"2", } },
				{"5", {"6", } },
				{"6", {"7", } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::AFFECTS) == expectedAffectsTable);

			
		}

		TEST_METHOD(affects_while)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/affects_while.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateAffectsCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedAffectsTable =
			{
				{"2", {"4", "10", "12", } },
				{"8", {"9", "7", } },
				{"6", {"5", } },
				{"7", {"6", } },
				{"1", {"11", } },
				{"9", {"8", } },
				{"4", {"11", } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::AFFECTS) == expectedAffectsTable);
		}

		TEST_METHOD(affects_if)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/affects_if.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateAffectsCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedAffectsTable =
			{
				{"1", {"15", "2", "13", } },
				{"2", {"7", "16", } },
				{"3", {"12", } },
				{"8", {"14", } },
				{"10", {"14", } },
				{"9", {"15", } },
				{"13", {"16", } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::AFFECTS) == expectedAffectsTable);
		}

		TEST_METHOD(affects_complicated)
		{
			string filename = "../IntegrationTesting/ParserPKBTests/affects_complicated.txt";
			SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
			TNode astRoot = parser.parseProgram();
			PROC_MATRIX procMatrix = parser.getMatrix();
			DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
			pkb.populateAffectsCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedAffectsTable =
			{
				{"13", {"51", "43", "45", "30", "48", } },
				{"75", {"91", "76", } },
				{"66", {"73", } },
				{"77", {"91", "76", "79", "100", } },
				{"7", {"13", "40", "55", "62", } },
				{"73", {"99", "78", } },
				{"22", {"39", } },
				{"97", {"100", } },
				{"76", {"79", } },
				{"8", {"6", "21", "33", "34", } },
				{"88", {"85", "88", } },
				{"56", {"53", "58", } },
				{"121", {"121", "123", } },
				{"84", {"84", } },
				{"80", {"86", } },
				{"6", {"8", "53", "54", "61", } },
				{"87", {"85", "86", "99", "90", } },
				{"127", {"128", } },
				{"85", {"88", } },
				{"9", {"16", } },
				{"89", {"84", "94", } },
				{"2", {"40", "8", "33", "36", "55", } },
				{"86", {"91", } },
				{"108", {"108", "120", } },
				{"68", {"97", } },
				{"95", {"97", } },
				{"39", {"12", "55", "65", } },
				{"107", {"117", "106", "116", } },
				{"32", {"43", "23", "24", } },
				{"103", {"116", "110", } },
				{"35", {"38", "6", "8", "33", } },
				{"104", {"112", "127", } },
				{"48", {"55", "61", } },
				{"115", {"116", "120", "129", } },
				{"116", {"115", "123", } },
				{"53", {"53", } },
				{"117", {"116", "117", } },
				{"118", {"121", } },
				{"43", {"38", } },
				{"110", {"122", } },
				{"122", {"115", "123", } },
				{"54", {"62", "63", } },
				{"120", {"122", "129", } },
				{"36", {"7", "13", "32", "65", "33", "54", } },
				{"106", {"125", } },
				{"44", {"6", "18", "56", } },
				{"112", {"128", "129", } },
				{"15", {"18", "6", "56", } },
				{"16", {"54", "14", "22", "31", "64", } },
				{"23", {"54", "14", "22", "31", "64", } },
				{"27", {"32", "35", "23", } },
				{"33", {"32", "33", } },
				{"31", {"36", "38", "40", } },
				{"18", {"38", } },
				{"34", {"39", } },
				{"38", {"38", "6", "8", "33", } },
				{"12", {"45", "9", "48", } },
				{"30", {"15", "7", "18", "9", "61", "13", "27", "62", "64", } },
				{"14", {"9", "63", "31", "35", "53", } },
				{"21", {"15", "35", "27", "32", } },
				{"45", {"40", "32", "39", "53", } },
				{"24", {"34", "35", "64", "56", } },
				{"55", {"60", "51", } },
				{"60", {"60", "58", "56", "64", "51", } },
				{"51", {"62", } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::AFFECTS) == expectedAffectsTable);

			pkb.populateAffectsStarCache();

			RELATIONSHIP_TABLE<PROG_LINE, PROG_LINE> expectedAffectsStarTable =
			{
				{"13", {"51", "43", "61", "45", "30", "55", "48", "60", "62", "58", "64", "56", "53", "15", "7", "18", "9", "13", "27", "32", "35", "23", "22", "14", "54", "31", "36", "38", "40", "6", "8", "33", "24", "34", "21", "39", "12", "65", "16", "63", } },
				{"75", {"91", "76", "79", } },
				{"66", {"73", "99", "78", } },
				{"77", {"91", "76", "79", "100", } },
				{"108", {"129", "108", "120", "122", "115", "116", "123", } },
				{"68", {"97", "100", } },
				{"51", {"62", } },
				{"116", {"115", "116", "123", "120", "129", "122", } },
				{"73", {"99", "78", } },
				{"7", {"13", "64", "40", "55", "62", "60", "51", "58", "56", "53", "43", "45", "30", "48", "61", "15", "7", "18", "9", "27", "32", "35", "23", "22", "14", "54", "31", "36", "38", "6", "8", "33", "24", "34", "21", "39", "12", "65", "16", "63", } },
				{"76", {"79", } },
				{"97", {"100", } },
				{"22", {"55", "39", "51", "12", "58", "65", "60", "62", "64", "56", "53", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "38", "40", "6", "8", "33", "32", "43", "23", "24", "34", "35", "21", "63", "15", "27", "18", "7", "13", "30", } },
				{"36", {"63", "7", "13", "32", "65", "33", "62", "54", "43", "23", "24", "34", "35", "56", "64", "53", "58", "38", "6", "8", "21", "39", "12", "55", "60", "51", "45", "9", "48", "61", "16", "22", "14", "31", "36", "40", "15", "27", "18", "30", } },
				{"106", {"125", } },
				{"43", {"38", "6", "24", "32", "8", "33", "43", "23", "34", "35", "56", "64", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "7", "13", "63", "30", "15", "18", "27", } },
				{"110", {"122", "115", "116", "123", "120", "129", } },
				{"80", {"86", "91", } },
				{"56", {"53", "58", } },
				{"84", {"84", } },
				{"121", {"121", "123", } },
				{"85", {"88", "85", } },
				{"127", {"128", } },
				{"86", {"91", } },
				{"2", {"40", "8", "33", "36", "55", "60", "51", "62", "58", "64", "56", "53", "7", "13", "32", "65", "54", "63", "43", "23", "24", "34", "35", "38", "6", "21", "39", "12", "45", "9", "48", "61", "16", "22", "14", "31", "15", "27", "18", "30", } },
				{"87", {"91", "85", "86", "99", "90", "88", } },
				{"6", {"8", "53", "62", "54", "61", "63", "6", "21", "33", "34", "39", "12", "55", "65", "60", "51", "58", "64", "56", "45", "9", "48", "16", "22", "14", "31", "36", "38", "40", "32", "43", "23", "24", "35", "7", "13", "30", "15", "18", "27", } },
				{"88", {"85", "88", } },
				{"8", {"6", "21", "65", "33", "12", "34", "55", "39", "60", "51", "62", "58", "64", "56", "53", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "38", "40", "8", "32", "43", "23", "24", "35", "63", "7", "13", "30", "15", "18", "27", } },
				{"89", {"84", "94", } },
				{"9", {"64", "16", "54", "14", "22", "31", "36", "38", "40", "6", "8", "33", "32", "43", "23", "24", "34", "35", "56", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "63", "15", "27", "18", "7", "13", "30", } },
				{"14", {"9", "63", "31", "6", "35", "53", "38", "8", "33", "32", "43", "23", "24", "34", "56", "64", "58", "39", "12", "55", "65", "60", "51", "62", "45", "48", "61", "16", "22", "14", "54", "36", "40", "21", "15", "27", "18", "7", "13", "30", } },
				{"16", {"54", "14", "22", "31", "40", "64", "36", "38", "6", "8", "33", "32", "43", "23", "24", "34", "35", "56", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "63", "15", "27", "18", "7", "13", "30", } },
				{"21", {"23", "15", "35", "27", "24", "32", "34", "43", "56", "64", "53", "58", "38", "6", "8", "33", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "63", "7", "13", "30", "18", } },
				{"95", {"97", "100", } },
				{"30", {"7", "15", "9", "18", "13", "61", "27", "62", "64", "32", "35", "23", "22", "14", "54", "31", "36", "38", "40", "6", "8", "33", "43", "24", "34", "56", "53", "58", "21", "39", "12", "55", "65", "60", "51", "45", "48", "16", "63", "30", } },
				{"32", {"34", "43", "23", "64", "56", "24", "35", "53", "58", "38", "6", "8", "33", "32", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "63", "7", "13", "30", "15", "18", "27", } },
				{"103", {"123", "116", "110", "115", "120", "129", "122", } },
				{"35", {"38", "6", "24", "32", "8", "33", "43", "23", "34", "35", "56", "64", "53", "58", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "21", "63", "15", "27", "18", "7", "13", "30", } },
				{"104", {"112", "127", "128", "129", } },
				{"39", {"51", "12", "55", "58", "65", "60", "62", "64", "56", "53", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "38", "40", "6", "8", "33", "32", "43", "23", "24", "34", "35", "21", "39", "63", "15", "27", "18", "7", "13", "30", } },
				{"107", {"117", "106", "123", "116", "115", "120", "129", "122", "125", } },
				{"44", {"6", "33", "58", "18", "8", "56", "53", "38", "32", "43", "23", "24", "34", "35", "64", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "7", "13", "63", "30", "15", "27", } },
				{"112", {"128", "129", } },
				{"48", {"55", "61", "60", "51", "62", "58", "56", "64", "53", } },
				{"115", {"123", "116", "120", "129", "122", "115", } },
				{"53", {"53", } },
				{"117", {"123", "116", "117", "115", "120", "129", "122", } },
				{"118", {"121", "123", } },
				{"54", {"62", "63", } },
				{"120", {"122", "129", "115", "116", "123", "120", } },
				{"122", {"115", "116", "123", "120", "129", "122", } },
				{"12", {"61", "45", "9", "55", "48", "60", "51", "62", "58", "64", "56", "53", "16", "22", "14", "54", "31", "36", "38", "40", "6", "8", "33", "32", "43", "23", "24", "34", "35", "21", "39", "12", "65", "63", "15", "27", "18", "7", "13", "30", } },
				{"15", {"58", "18", "6", "8", "56", "53", "54", "61", "62", "63", "21", "33", "34", "39", "12", "55", "65", "60", "51", "64", "45", "9", "48", "16", "22", "14", "31", "36", "38", "40", "32", "43", "23", "24", "35", "7", "13", "30", "15", "27", } },
				{"18", {"38", "6", "24", "32", "8", "33", "43", "23", "34", "35", "56", "64", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "7", "13", "63", "30", "15", "18", "27", } },
				{"23", {"54", "14", "22", "31", "40", "64", "36", "38", "6", "8", "33", "32", "43", "23", "24", "34", "35", "56", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "63", "15", "27", "18", "7", "13", "30", } },
				{"24", {"34", "6", "35", "64", "56", "53", "58", "38", "8", "33", "32", "43", "23", "24", "22", "14", "54", "31", "36", "40", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "63", "15", "27", "18", "7", "13", "30", } },
				{"27", {"64", "32", "35", "31", "23", "54", "14", "22", "36", "38", "40", "6", "8", "33", "43", "24", "34", "56", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "63", "15", "27", "18", "7", "13", "30", } },
				{"31", {"36", "38", "32", "8", "40", "6", "33", "43", "23", "24", "34", "35", "56", "64", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "63", "15", "27", "18", "7", "13", "30", } },
				{"33", {"56", "24", "32", "33", "34", "43", "23", "35", "64", "53", "58", "38", "6", "8", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "63", "7", "13", "30", "15", "18", "27", } },
				{"34", {"55", "39", "51", "12", "58", "65", "60", "62", "64", "56", "53", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "38", "40", "6", "8", "33", "32", "43", "23", "24", "34", "35", "21", "63", "15", "27", "18", "7", "13", "30", } },
				{"38", {"38", "6", "24", "32", "8", "33", "43", "23", "34", "35", "56", "64", "53", "58", "21", "39", "12", "55", "65", "60", "51", "62", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "40", "7", "13", "63", "30", "15", "18", "27", } },
				{"45", {"40", "32", "55", "39", "53", "12", "65", "60", "51", "62", "58", "64", "56", "45", "9", "48", "61", "16", "22", "14", "54", "31", "36", "38", "6", "8", "33", "43", "23", "24", "34", "35", "21", "63", "15", "27", "18", "7", "13", "30", } },
				{"55", {"60", "51", "62", "58", "56", "64", "53", } },
				{"60", {"60", "58", "56", "64", "51", "62", "53", } }
			};
			Assert::IsTrue(getStringRelationshipTableOf(RELATIONSHIP_TYPE::AFFECTS_STAR) == expectedAffectsStarTable);

			
		}
	};
}
