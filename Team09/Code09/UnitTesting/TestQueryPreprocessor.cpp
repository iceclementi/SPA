#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreprocessor.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryPreprocessor)
	{
	public:
		TEST_METHOD(parseQuery_validDeclarations_success) {
			QueryObject expectedQueryObject;
			expectedQueryObject.errorMessage = NO_ERROR;

			QuerySelectResult expectedQsr;
			expectedQueryObject.errorMessage = NO_ERROR;

			// Single declaration with one synonym
			string query1 = "stmt s; Select s";
			QueryObject expectedQueryObject1 = expectedQueryObject;
			QuerySelectResult expectedQsr1;
			expectedQsr1.addSelectSynonym(1);
			expectedQsr1.addToSelectElementList({ NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject1.setSelectResult(expectedQsr1);
			Assert::IsTrue(expectedQueryObject1 == QueryPreprocessor(query1).parseQuery());

			// Single declaration with multiple synonyms
			string query2 = "stmt s1, s2; Select s2";
			QueryObject expectedQueryObject2 = expectedQueryObject;
			QuerySelectResult expectedQsr2;
			expectedQsr2.addSelectSynonym(2);
			expectedQsr2.addToSelectElementList({ NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject2.setSelectResult(expectedQsr2);
			Assert::IsTrue(expectedQueryObject2 == QueryPreprocessor(query2).parseQuery());

			// Multiple declarations with multiple synonyms
			string query3 = "stmt s1, s2; prog_line n; procedure p; Select n";
			QueryObject expectedQueryObject3 = expectedQueryObject;
			QuerySelectResult expectedQsr3;
			expectedQsr3.addSelectSynonym(3);
			expectedQsr3.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject3.setSelectResult(expectedQsr3);
			Assert::IsTrue(expectedQueryObject3 == QueryPreprocessor(query3).parseQuery());

			// Keywords used as synonym
			string query4 = "stmt Select, s2; procedure p; Select Select";
			QueryObject expectedQueryObject4;
			QuerySelectResult expectedQsr4;
			expectedQsr4.addSelectSynonym(1);
			expectedQsr4.addToSelectElementList({ NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject4.setSelectResult(expectedQsr4);
			Assert::IsTrue(expectedQueryObject4 == QueryPreprocessor(query4).parseQuery());

			string query5 = "stmt Select, s2; procedure procedure; Select procedure";
			QueryObject expectedQueryObject5 = expectedQueryObject;
			QuerySelectResult expectedQsr5;
			expectedQsr5.addSelectSynonym(3);
			expectedQsr5.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject5.setSelectResult(expectedQsr5);
			Assert::IsTrue(expectedQueryObject5 == QueryPreprocessor(query5).parseQuery());

			string query6 = "stmt Select, s2; call BOOLEAN; procedure procedure; Select BOOLEAN";
			QueryObject expectedQueryObject6 = expectedQueryObject;
			QuerySelectResult expectedQsr6;
			expectedQsr6.addSelectSynonym(3);
			expectedQsr6.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::CALL, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject6.setSelectResult(expectedQsr6);
			Assert::IsTrue(expectedQueryObject6 == QueryPreprocessor(query6).parseQuery());

			// Repeated design entities across declarations
			string query7 = "stmt s1, s2; read r; stmt s3; Select s3";
			QueryObject expectedQueryObject7 = expectedQueryObject;
			QuerySelectResult expectedQsr7;
			expectedQsr7.addSelectSynonym(4);
			expectedQsr7.addToSelectElementList({ NO_ERROR, 4, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject7.setSelectResult(expectedQsr7);
			Assert::IsTrue(expectedQueryObject7 == QueryPreprocessor(query7).parseQuery());
		}

		TEST_METHOD(parseQuery_invalidDeclarations_failure) {
			QueryObject expectedQueryObject;

			// Syntax error for queries 1 - 10
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			// Missing design entity
			string query1 = "stmt s1, s2; p; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query1).parseQuery());

			// Missing synonym
			string query2 = "stmt s1, s2; procedure ; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query2).parseQuery());

			// Missing , for declaration with multiple synonyms
			string query3 = "stmt s1 s2; procedure p; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query3).parseQuery());

			// Missing ; for multiple declarations
			string query4 = "stmt s1, s2 procedure p; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query4).parseQuery());

			// Missing Select keyword
			string query5 = "stmt s1, s2; procedure p; s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query5).parseQuery());

			// Missing select-synonym
			string query6 = "stmt s1, s2; procedure p; Select";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query6).parseQuery());

			// Invalid design entity
			string query7 = "statement s1, s2; procedure p; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query7).parseQuery());

			// Invalid synonym
			string query8 = "stmt s1, 2s; procedure p; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query8).parseQuery());

			// Invalid Select keyword
			string query9 = "stmt s1, s2; procedure p; select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query9).parseQuery());

			// Invalid select-synonym
			string query10 = "stmt s1, s2; procedure p; Select 1s";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query10).parseQuery());

			// Semantic error for queries 11 - 13
			expectedQueryObject.errorMessage = SEMANTIC_ERROR;

			// Repeated synonyms in declaration
			string query11 = "stmt s1, s2; stmt s1; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query11).parseQuery());

			// Repeated synonyms in declaration
			string query12 = "stmt s1, s2; procedure s1; Select s1";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query12).parseQuery());

			// select-synonym not in declaration
			string query13 = "stmt s1, s2; procedure p; Select s3";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query13).parseQuery());

			// Syntax error due to invalid prog_line design entity for queries 14 - 
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			string query14 = "stmt s1, s2; prog_ line p; Select p";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query14).parseQuery());

			string query15 = "stmt s1, s2; prog _line p; Select p";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query15).parseQuery());

			string query16 = "stmt s1, s2; prog_\fline p; Select p";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query16).parseQuery());
		}

		TEST_METHOD(parseQuery_validResultClause_success) {
			QueryObject expectedQueryObject;
			expectedQueryObject.errorMessage = NO_ERROR;

			// Select BOOLEAN
			string query1 = "stmt s1, s2; procedure p; Select BOOLEAN";
			QueryObject expectedQueryObject1;
			QuerySelectResult expectedQsr1;
			expectedQsr1.setSelectBoolean(true);
			expectedQueryObject1.setSelectResult(expectedQsr1);
			Assert::IsTrue(expectedQueryObject1 == QueryPreprocessor(query1).parseQuery());

			// Select synonym
			string query2 = "stmt s1, s2; procedure p; Select p";
			QueryObject expectedQueryObject2;
			QuerySelectResult expectedQsr2;
			expectedQsr2.addSelectSynonym(3);
			expectedQsr2.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE});
			expectedQueryObject2.setSelectResult(expectedQsr2);
			Assert::IsTrue(expectedQueryObject2 == QueryPreprocessor(query2).parseQuery());

			// Select attribute reference
			string query3 = "stmt s1, s2; procedure p; Select p.procName";
			QueryObject expectedQueryObject3;
			QuerySelectResult expectedQsr3;
			expectedQsr3.addSelectSynonym(3);
			expectedQsr3.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NAME });
			expectedQueryObject3.setSelectResult(expectedQsr3);
			Assert::IsTrue(expectedQueryObject3 == QueryPreprocessor(query3).parseQuery());

			// Select tuple
			string query4 = "stmt s1, s2; procedure p; prog_line BOOLEAN; Select <s1.stmt#, s2, BOOLEAN, p.procName>";
			QueryObject expectedQueryObject4;
			QuerySelectResult expectedQsr4;
			expectedQsr4.addSelectSynonym(1);
			expectedQsr4.addSelectSynonym(2);
			expectedQsr4.addSelectSynonym(4);
			expectedQsr4.addSelectSynonym(3);
			expectedQsr4.addToSelectElementList({ NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::INTEGER });
			expectedQsr4.addToSelectElementList({ NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::NONE });
			expectedQsr4.addToSelectElementList({ NO_ERROR, 4, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE });
			expectedQsr4.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NAME });
			expectedQueryObject4.setSelectResult(expectedQsr4);
			Assert::IsTrue(expectedQueryObject4 == QueryPreprocessor(query4).parseQuery());
		}

		TEST_METHOD(parseQuery_invalidResultclause_failure) {
			QueryObject expectedQueryObject;

			// Invalid attribute reference
			string query1 = "stmt s1, s2; procedure p; Select .procName";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query1).parseQuery());

			string query2 = "stmt s1, s2; procedure p; Select p.";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query2).parseQuery());

			string query3 = "stmt s1, s2; procedure p; Select p. procName";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query3).parseQuery());

			string query4 = "stmt s1, s2; procedure p; Select p .procName";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query4).parseQuery());

			string query5 = "stmt s1, s2; procedure p; Select p.varName";
			expectedQueryObject.errorMessage = SEMANTIC_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query5).parseQuery());

			// Invalid tuple
			string query6 = "stmt s1, s2; procedure p; prog_line BOOLEAN; Select s1.stmt#, s2, BOOLEAN, p.procName>";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query6).parseQuery());

			string query7 = "stmt s1, s2; procedure p; prog_line BOOLEAN; Select <s1.stmt#, s2, BOOLEAN, p.procName";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query7).parseQuery());

			string query8 = "stmt s1, s2; procedure p; prog_line BOOLEAN; Select <s1.stmt#, s2, BOOLEAN p.procName>";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query8).parseQuery());

			string query9 = "stmt s1, s2; procedure p; prog_line BOOLEAN; Select <s1.stmt#, s2, BOOLEAN, p.procName, >";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query9).parseQuery());
		}

		TEST_METHOD(parseQuery_validSingleQueryClause_success) {
			QueryObject expectedQueryObject;
			expectedQueryObject.errorMessage = NO_ERROR;

			// Such that clause
			string query1 = "stmt s1, s2; procedure p; Select p such that Next(1, 2)";
			QueryObject expectedQueryObject1;
			QuerySelectResult expectedQsr1;
			expectedQsr1.addSelectSynonym(3);
			expectedQsr1.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject1.setSelectResult(expectedQsr1);
			SuchThatClause qo1c1 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject1.addSuchThatClause(qo1c1, 1);
			Assert::IsTrue(expectedQueryObject1 == QueryPreprocessor(query1).parseQuery());

			// Pattern assign clause
			string query2 = "procedure p; assign assign; variable v; Select p pattern assign(v, _ \" ( x + y ) - ( z * 0 ) \" _)";
			QueryObject expectedQueryObject2;
			QuerySelectResult expectedQsr2;
			expectedQsr2.addSelectSynonym(1);
			expectedQsr2.addToSelectElementList({ NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject2.setSelectResult(expectedQsr2);
			PatternClause qo2c1 = PatternClause(
				DESIGN_ENTITY::ASSIGN,
				2,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::PARTIAL_MATCH, "(x+y)-(z*0)" });
			qo2c1.addClauseSynonym(2);
			qo2c1.addClauseSynonym(3);
			expectedQueryObject2.addPatternClause(qo2c1, 1);
			Assert::IsTrue(expectedQueryObject2 == QueryPreprocessor(query2).parseQuery());

			// Pattern if clause
			string query3 = "procedure p; if if; variable v; Select p pattern if(\" abc \" , _ , _)";
			QueryObject expectedQueryObject3;
			QuerySelectResult expectedQsr3;
			expectedQsr3.addSelectSynonym(1);
			expectedQsr3.addToSelectElementList({ NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject3.setSelectResult(expectedQsr3);
			PatternClause qo3c1 = PatternClause(
				DESIGN_ENTITY::IF,
				2,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::IDENT, "abc", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo3c1.addClauseSynonym(2);
			expectedQueryObject3.addPatternClause(qo3c1, 1);
			Assert::IsTrue(expectedQueryObject3 == QueryPreprocessor(query3).parseQuery());

			// Pattern while clause
			string query4 = "procedure p; while while; variable v; Select p pattern while( _ , _ )";
			QueryObject expectedQueryObject4;
			QuerySelectResult expectedQsr4;
			expectedQsr4.addSelectSynonym(1);
			expectedQsr4.addToSelectElementList({ NO_ERROR, 1, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject4.setSelectResult(expectedQsr4);
			PatternClause qo4c1 = PatternClause(
				DESIGN_ENTITY::WHILE,
				2,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo4c1.addClauseSynonym(2);
			expectedQueryObject4.addPatternClause(qo4c1, 1);
			Assert::IsTrue(expectedQueryObject4 == QueryPreprocessor(query4).parseQuery());

			// With clause
			string query5= "stmt s1, s2; procedure p; Select p with s1.stmt# = s2.stmt#";
			QueryObject expectedQueryObject5;
			QuerySelectResult expectedQsr5;
			expectedQsr5.addSelectSynonym(3);
			expectedQsr5.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject5.setSelectResult(expectedQsr5);
			WithClause qo5c1 = WithClause(
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "s1.stmt#", QueryElement(NO_ERROR, 1, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::INTEGER)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "s2.stmt#", QueryElement(NO_ERROR, 2, DESIGN_ENTITY::STATEMENT, ATTRIBUTE_TYPE::INTEGER)));
			qo5c1.addClauseSynonym(1);
			qo5c1.addClauseSynonym(2);
			expectedQueryObject5.addWithClause(qo5c1, 1);
			Assert::IsTrue(expectedQueryObject5 == QueryPreprocessor(query5).parseQuery());
		}

		TEST_METHOD(parseQuery_invalidSingleQueryClause_failure) {
			QueryObject expectedQueryObject;

			// Missing keyword
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			string query1 = "stmt s1, s2; procedure p; Select p such Next(1, 2)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query1).parseQuery());

			string query2 = "stmt s1, s2; procedure p; Select p that Next(1, 2)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query2).parseQuery());

			string query3 = "stmt s1, s2; procedure p; Select p suchthat Next(1, 2)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query3).parseQuery());

			string query4 = "procedure p; assign assign; variable v; Select p assign(v, _ \" x + y \" _)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query4).parseQuery());

			string query5 = "stmt s1, s2; procedure p; Select p s1.stmt# = s2.stmt#";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query5).parseQuery());

			// Invalid argument syntax
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			string query6 = "stmt s1, s2; procedure p; Select p such that Follows(1s, s2)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query6).parseQuery());

			string query7 = "stmt s1, s2; procedure p; Select p such that Follows(s1, __)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query7).parseQuery());

			string query8 = "stmt s1, s2; procedure p; Select p such that Follows(s1, 0)";
			expectedQueryObject.errorMessage = SEMANTIC_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query8).parseQuery());

			string query9 = "stmt s1, s2; procedure p; Select p such that Calls(p, \"\")";
			expectedQueryObject.errorMessage = SYNTAX_ERROR;
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query9).parseQuery());

			string query10 = "stmt s1, s2; procedure p; Select p such that Calls(p, \"   \")";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query10).parseQuery());

			string query11 = "stmt s1, s2; procedure p; Select p such that Calls(p, \"procedure)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query11).parseQuery());

			string query12 = "stmt s1, s2; procedure p; Select p such that Calls(p, procedure\")";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query12).parseQuery());

			string query13 = "stmt s1, s2; procedure p; Select p with .procName = \"procedure\"";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query13).parseQuery());

			string query14 = "stmt s1, s2; procedure p; Select p with p. = \"procedure\"";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query14).parseQuery());

			string query15 = "stmt s1, s2; procedure p; Select p with p. procName = \"procedure\"";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query15).parseQuery());

			string query16 = "stmt s1, s2; procedure p; Select p with p .procName = \"procedure\"";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query16).parseQuery());

			string query17 = "procedure p; assign assign; variable v; Select p pattern assign(v,   ( x + y ) - ( z * 5 ) \" )";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query17).parseQuery());

			string query18 = "procedure p; assign assign; variable v; Select p pattern assign(v,  \" ( x + y ) - ( z * 5 )  )";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query18).parseQuery());

			string query19 = "procedure p; assign assign; variable v; Select p pattern assign(v,  _ \" ( x + y ) - ( z * 5 ) \" )";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query19).parseQuery());\

			string query20 = "procedure p; assign assign; variable v; Select p pattern assign(v,  \" ( x + y ) - ( z * 5 ) \" _ )";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query20).parseQuery());

			string query21 = "procedure p; assign assign; variable v; Select p pattern assign(v,  _ ( x + y ) - ( z * 5 ) _ )";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query21).parseQuery());

			// Wrong synonym argument type or 
			// UNDERSCORE used as first argument of Modifies/Uses or 
			// invalid combnation of synonym and attribute name in attribute reference
			expectedQueryObject.errorMessage = SEMANTIC_ERROR;

			string query22 = "stmt s1, s2; procedure p; Select p such that Calls(p, s1)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query22).parseQuery());

			string query23 = "stmt s1, s2; procedure p; variable v; Select p such that Modifies(_, v)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query23).parseQuery());

			string query24 = "stmt s1, s2; read r; variable v; Select pr such that Uses(r, v)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query24).parseQuery());

			string query25 = "stmt s1, s2; procedure p; variable v; Select p such that Uses(_, v)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query25).parseQuery());

			string query26 = "procedure p; assign assign; variable v; Select p pattern assign(p, _ \" x + y \" _)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query26).parseQuery());

			string query27 = "procedure p; assign assign; variable v; Select p with p.varName = \"variable\"";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query27).parseQuery());

			// Wrong argument type
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			string query28 = "stmt s1, s2; procedure p; Select p such that Uses(p, 1)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query28).parseQuery());

			string query29 = "procedure p; assign assign; variable v; Select p pattern assign(v, 1)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query29).parseQuery());

			string query30 = "stmt s1, s2; procedure p; Select p with _ = s1.stmt#";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query30).parseQuery());

			// Wrong number of arguments
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			string query31 = "stmt s1, s2; procedure p; Select p such that Next*(1)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query31).parseQuery());

			string query32 = "procedure p; assign assign; variable v; Select p pattern assign(v, _ \" x + y \" _, _)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query32).parseQuery());

			string query33 = "procedure p; if if; variable v; Select p pattern if(\" abc \" , _)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query33).parseQuery());

			string query34 = "procedure p; while while; variable v; Select p pattern while( _ , _ , _ )";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query34).parseQuery());

			string query35 = "stmt s1, s2; procedure p; Select p with s1.stmt# = s1.stmt# = s2.stmt#";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query35).parseQuery());
		}

		TEST_METHOD(parseQuery_validMultipleQueryClauses_success) {
			QueryObject expectedQueryObject;
			expectedQueryObject.errorMessage = NO_ERROR;

			string query1 = "stmt s1, s2; procedure p; Select p such that Next(1, 2) such that Next*(1, 2)";
			QueryObject expectedQueryObject1;
			QuerySelectResult expectedQsr1;
			expectedQsr1.addSelectSynonym(3);
			expectedQsr1.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject1.setSelectResult(expectedQsr1);
			SuchThatClause qo1c1 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject1.addSuchThatClause(qo1c1, 1);
			SuchThatClause qo1c2 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT_T,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject1.addSuchThatClause(qo1c2, 2);
			Assert::IsTrue(expectedQueryObject1 == QueryPreprocessor(query1).parseQuery());

			string query2 = "stmt s1, s2; procedure p; Select p such that Next(1, 2) and Next*(1, 2)";
			QueryObject expectedQueryObject2;
			QuerySelectResult expectedQsr2;
			expectedQsr2.addSelectSynonym(3);
			expectedQsr2.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject2.setSelectResult(expectedQsr2);
			SuchThatClause qo2c1 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject2.addSuchThatClause(qo2c1, 1);
			SuchThatClause qo2c2 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT_T,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject2.addSuchThatClause(qo2c2, 2);
			Assert::IsTrue(expectedQueryObject2 == QueryPreprocessor(query2).parseQuery());

			string query3 = "stmt s1, s2; procedure p; assign assign; variable v; if if; while while; ";
			query3 = query3 + "Select p such that Next(1, 2) and Next*(1, 2) ";
			query3 = query3 + "pattern assign(v, _ \" x + y \" _) and if(\" abc \" , _ , _) and while( _ , _ ) ";
			query3 = query3 + "with v.varName = \" XYZ1 \" ";
			QueryObject expectedQueryObject3;
			QuerySelectResult expectedQsr3;
			expectedQsr3.addSelectSynonym(3);
			expectedQsr3.addToSelectElementList({ NO_ERROR, 3, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject3.setSelectResult(expectedQsr3);
			SuchThatClause qo3c1 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject3.addSuchThatClause(qo3c1, 1);
			SuchThatClause qo3c2 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT_T,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject3.addSuchThatClause(qo3c2, 2);
			PatternClause qo3c3 = PatternClause(
				DESIGN_ENTITY::ASSIGN,
				4,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::PARTIAL_MATCH, "x+y" });
			qo3c3.addClauseSynonym(4);
			qo3c3.addClauseSynonym(5);
			expectedQueryObject3.addPatternClause(qo3c3, 3);
			PatternClause qo3c4 = PatternClause(
				DESIGN_ENTITY::IF,
				6,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::IDENT, "abc", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo3c4.addClauseSynonym(6);
			expectedQueryObject3.addPatternClause(qo3c4, 4);
			PatternClause qo3c5 = PatternClause(
				DESIGN_ENTITY::WHILE,
				7,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo3c5.addClauseSynonym(7);
			expectedQueryObject3.addPatternClause(qo3c5, 5);
			WithClause qo3c6 = WithClause(
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::ATTRIBUTE_REF, "v.varName", QueryElement(NO_ERROR, 5, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NAME)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::IDENT, "XYZ1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			qo3c6.addClauseSynonym(5);
			expectedQueryObject3.addWithClause(qo3c6, 6);
			Assert::IsTrue(expectedQueryObject3 == QueryPreprocessor(query3).parseQuery());

			string query4 = "stmt s1, s2; prog_line n1, n2; procedure p; assign assign; variable v; if if; while while; ";
			query4 = query4 + "Select p such that Next(1, 2) and Next*(n1, n2) ";
			query4 = query4 + "pattern assign(v, _ \" x + y \" _) and if(\" abc \" , _ , _) pattern while( _ , _ ) ";
			query4 = query4 + "with n1 = 100";
			QueryObject expectedQueryObject4;
			QuerySelectResult expectedQsr4;
			expectedQsr4.addSelectSynonym(5);
			expectedQsr4.addToSelectElementList({ NO_ERROR, 5, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject4.setSelectResult(expectedQsr4);
			SuchThatClause qo4c1 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject4.addSuchThatClause(qo4c1, 1);
			SuchThatClause qo4c2 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT_T,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n1", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n2", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE)));
			qo4c2.addClauseSynonym(3);
			qo4c2.addClauseSynonym(4);
			expectedQueryObject4.addSuchThatClause(qo4c2, 2);
			PatternClause qo4c3 = PatternClause(
				DESIGN_ENTITY::ASSIGN,
				6,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 7, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::PARTIAL_MATCH, "x+y" });
			qo4c3.addClauseSynonym(6);
			qo4c3.addClauseSynonym(7);
			expectedQueryObject4.addPatternClause(qo4c3, 3);
			PatternClause qo4c4 = PatternClause(
				DESIGN_ENTITY::IF,
				8,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::IDENT, "abc", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo4c4.addClauseSynonym(8);
			expectedQueryObject4.addPatternClause(qo4c4, 4);
			PatternClause qo4c5 = PatternClause(
				DESIGN_ENTITY::WHILE,
				9,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo4c5.addClauseSynonym(9);
			expectedQueryObject4.addPatternClause(qo4c5, 5);
			WithClause qo4c6 = WithClause(
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n1", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "100", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			qo4c6.addClauseSynonym(3);
			expectedQueryObject4.addWithClause(qo4c6, 6);
			Assert::IsTrue(expectedQueryObject4 == QueryPreprocessor(query4).parseQuery());
		}

		TEST_METHOD(parseQuery_invalidMultipleQueryClauses_failure) {
			QueryObject expectedQueryObject;
			expectedQueryObject.errorMessage = SYNTAX_ERROR;

			// Missing keyword
			string query1 = "stmt s1, s2; procedure p; Select p such that Next(1, 2) Next*(1, 2)";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query1).parseQuery());

			// Invalid combination
			string query2 = "stmt s1, s2; procedure p; assign assign; variable v; if if; while while; ";
			query2 = query2 + "Select p such that Next(1, 2) and Next*(1, 2) ";
			query2 = query2 + "pattern assign(v, _ \" x + y \" _) and if(\" abc \" , _ , _) and while( _ , _ ) and Next*(1, 2) ";
			query2 = query2 + "with 5 = s1.stmt#";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query2).parseQuery());

			// Start query clause with "and"
			string query3 = "stmt s1, s2; procedure p; assign assign; variable v; if if; while while; ";
			query3 = query3 + "Select p and Next(1, 2) and Next*(1, 2) ";
			query3 = query3 + "pattern assign(v, _ \" x + y \" _) and if(\" abc \" , _ , _) and while( _ , _ ) ";
			query3 = query3 + "with 5 = s1.stmt#";
			Assert::IsTrue(expectedQueryObject == QueryPreprocessor(query3).parseQuery());
		}

		TEST_METHOD(parseQuery_validMultipleQueryClausesWhitespaceVariant_success) {
			QueryObject expectedQueryObject;
			expectedQueryObject.errorMessage = NO_ERROR;

			string query1 = "stmt s1 \f , s2  ; prog_line n1, n2; procedure p; assign assign; variable v; if if; while while; ";
			query1 = query1 + "Select p such that Next( 1 , 2 \n )  and Next* ( n1 , n2 ) ";
			query1 = query1 + "pattern assign ( v, _ \" x  \r + y \" _) and if ( \" abc \" , _ , _ ) pattern while ( _ , _ ) ";
			query1 = query1 + "with n1\t=100";
			QueryObject expectedQueryObject1;
			QuerySelectResult expectedQsr1;
			expectedQsr1.addSelectSynonym(5);
			expectedQsr1.addToSelectElementList({ NO_ERROR, 5, DESIGN_ENTITY::PROCEDURE, ATTRIBUTE_TYPE::NONE });
			expectedQueryObject1.setSelectResult(expectedQsr1);
			SuchThatClause qo1c1 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "1", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "2", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			expectedQueryObject1.addSuchThatClause(qo1c1, 1);
			SuchThatClause qo1c2 = SuchThatClause(
				SUCH_THAT_REL_TYPE::NEXT_T,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n1", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n2", QueryElement(NO_ERROR, 4, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE)));
			qo1c2.addClauseSynonym(3);
			qo1c2.addClauseSynonym(4);
			expectedQueryObject1.addSuchThatClause(qo1c2, 2);
			PatternClause qo1c3 = PatternClause(
				DESIGN_ENTITY::ASSIGN,
				6,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "v", QueryElement(NO_ERROR, 7, DESIGN_ENTITY::VARIABLE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::PARTIAL_MATCH, "x+y" });
			qo1c3.addClauseSynonym(6);
			qo1c3.addClauseSynonym(7);
			expectedQueryObject1.addPatternClause(qo1c3, 3);
			PatternClause qo1c4 = PatternClause(
				DESIGN_ENTITY::IF,
				8,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::IDENT, "abc", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo1c4.addClauseSynonym(8);
			expectedQueryObject1.addPatternClause(qo1c4, 4);
			PatternClause qo1c5 = PatternClause(
				DESIGN_ENTITY::WHILE,
				9,
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::UNDERSCORE, "", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)),
				{ PATTERN_MATCH_TYPE::INVALID, "" });
			qo1c5.addClauseSynonym(9);
			expectedQueryObject1.addPatternClause(qo1c5, 5);
			WithClause qo1c6 = WithClause(
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::SYNONYM, "n1", QueryElement(NO_ERROR, 3, DESIGN_ENTITY::PROG_LINE, ATTRIBUTE_TYPE::NONE)),
				QueryArgument(NO_ERROR, ARGUMENT_TYPE::INTEGER, "100", QueryElement(NO_ERROR, NO_SYNONYM_INDEX, DESIGN_ENTITY::NONE, ATTRIBUTE_TYPE::NONE)));
			qo1c6.addClauseSynonym(3);
			expectedQueryObject1.addWithClause(qo1c6, 6);
			Assert::IsTrue(expectedQueryObject1 == QueryPreprocessor(query1).parseQuery());
		}
	};
}
