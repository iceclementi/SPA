#include "stdafx.h"
#include "MockAst.h"

namespace MockAst {
	TNode MockAst::createMockProgramNode(string name, TNode procedure) {
		TNode progNode = TNode(TNode::NODE_TYPE::program, name);
		progNode.addChild(procedure);
		return progNode;
	};
	TNode MockAst::createMockReadNode(int stmtNo, string modifiedVar) {
		TNode readNode = TNode(TNode::NODE_TYPE::readStmt, stmtNo, modifiedVar);
		readNode.addModifiedVariable(modifiedVar);
		return readNode;
	};
	TNode MockAst::createMockPrintNode(int stmtNo, string usedVar) {
		TNode printNode = TNode(TNode::NODE_TYPE::printStmt, stmtNo, usedVar);
		printNode.addUsedVariable(usedVar);
		return printNode;
	};
	TNode MockAst::createMockCallNode(int stmtNo, string procedureName) {
		TNode callNode = TNode(TNode::NODE_TYPE::callStmt, stmtNo, procedureName);
		return callNode;
	};
	TNode MockAst::createMockAssignNode(int stmtNo, string rpn, string variable, VAR_NAMES vars, VARS con) {
		TNode assignNode = TNode(TNode::NODE_TYPE::assignStmt, stmtNo, rpn);
		assignNode.addUsedVariables(vars);
		assignNode.addConstants(con);
		assignNode.addModifiedVariable(variable);
		return assignNode;
	};
	TNode MockAst::createMockProcedureNode(string name, TNode procedureStmtLst) {
		TNode procedureNode = TNode(TNode::NODE_TYPE::procedure, name);
		procedureNode.addChild(procedureStmtLst);

		return procedureNode;
	};
	TNode MockAst::createMockWhileNode(int stmtNo, string condExpr, VAR_NAMES vars, VARS con, TNode whileStmtLst) {
		TNode whileNode = TNode(TNode::NODE_TYPE::whileStmt, stmtNo, condExpr);
		whileNode.addUsedVariables(vars);
		whileNode.addConstants(con);
		whileNode.addChild(whileStmtLst);

		return whileNode;
	}
	TNode MockAst::createMockIfNode(int stmtNo, string condExpr, VAR_NAMES vars, VARS con, TNode thenStmtLst, TNode elseStmtLst) {
		TNode ifNode = TNode(TNode::NODE_TYPE::ifStmt, stmtNo, condExpr);
		ifNode.addUsedVariables(vars);
		ifNode.addConstants(con);
		ifNode.addChild(thenStmtLst);
		ifNode.addChild(elseStmtLst);
		return ifNode;
	};

	TNode MockAst::createMockProcStmtLstNode(vector<TNode> childs) {
		TNode procStmtLst = TNode(TNode::NODE_TYPE::procedureStmtLst);
		for (TNode child : childs) {
			procStmtLst.addChild(child);
		}

		return procStmtLst;
	};
	TNode MockAst::createMockWhileStmtLstNode(vector<TNode> childs) {
		TNode whileStmtLst = TNode(TNode::NODE_TYPE::whileStmtLst);
		for (TNode child : childs) {
			whileStmtLst.addChild(child);
		}

		return whileStmtLst;
	};
	TNode MockAst::createMockThenStmtLstNode(vector<TNode> childs) {
		TNode thenStmtLst = TNode(TNode::NODE_TYPE::thenStmtLst);
		for (TNode child : childs) {
			thenStmtLst.addChild(child);
		}

		return thenStmtLst;
	};

	TNode MockAst::createMockElseStmtLstNode(vector<TNode> childs) {
		TNode elseStmtLst = TNode(TNode::NODE_TYPE::elseStmtLst);
		for (TNode child : childs) {
			elseStmtLst.addChild(child);
		}

		return elseStmtLst;
	};

	TNode MockAst::createSampleAst1(string name) {
		/*
		procedure sumDigits	{
		1.  read number;
		2.  sum = 0;
		3.  call mummy;

		4.  while (number > 0) {
		5.	  digit = number % 10;
		6.	  sum = sum + digit;
		7.	  number = number / 10;
			}

		8.  if (number < 9000) then {
		9.	    shopee = 1 + 1;
			} else {
		10.	    lazada = 1;
			}
		11.	print sum;
		}
		*/

		TNode stmt1 = createMockReadNode(1, "number");
		VAR_NAMES v2 = {};
		VARS c2 = { "0" };
		TNode stmt2 = createMockAssignNode(2, " 0 ", "sum", v2, c2);
		TNode stmt3 = createMockPrintNode(3, "mummy");

		VAR_NAMES v5 = { "number" };
		VARS c5 = { "10" };
		TNode stmt5 = createMockAssignNode(5, " number  10  % ", "digit", v5, c5);
		VAR_NAMES v6 = { "sum", "digit" };
		VARS c6 = {};
		TNode stmt6 = createMockAssignNode(6, " sum  digit  + ", "sum", v6, c6);
		VAR_NAMES v7 = { "number" };
		VARS c7 = { "10" };
		TNode stmt7 = createMockAssignNode(7, " number  10  / ", "number", v7, c7);
		stmt5.setParent(4);
		stmt6.setParent(4);
		stmt7.setParent(4);
		vector<TNode> stmt4babies = { stmt5, stmt6, stmt7 };
		TNode stmt4whileLst = createMockWhileStmtLstNode(stmt4babies);
		VAR_NAMES v4 = { "number" };
		VARS c4 = { "0" };
		TNode stmt4 = createMockWhileNode(4, "number  > 0 ", v4, c4, stmt4whileLst);

		VAR_NAMES v9 = {};
		VARS c9 = { "1" };
		TNode stmt9 = createMockAssignNode(9, " 1  1  + ", "shopee", v9, c9);
		VAR_NAMES v10 = {};
		VARS c10 = { "1" };
		TNode stmt10 = createMockAssignNode(10, " 1 ", "lazada", v10, c10);
		stmt9.setParent(8);
		stmt10.setParent(8);
		vector<TNode> slt8 = { { stmt9 } };
		vector<TNode> sle8 = { { stmt10 } };
		TNode stmt8thenLst = createMockThenStmtLstNode(slt8);
		TNode stmt8elseLst = createMockElseStmtLstNode({ sle8 });

		VAR_NAMES v8 = { "number" };
		VARS c8 = { "9000" };
		TNode stmt8 = createMockIfNode(8, "number  < 9000 ", v8, c8, stmt8thenLst, stmt8elseLst);

		TNode stmt11 = createMockPrintNode(11, "sum");

		vector<TNode> procBabies = { stmt1,stmt2,stmt3,stmt4,stmt8,stmt11 };
		TNode procLstNode = createMockProcStmtLstNode(procBabies);
		TNode procedureNode = createMockProcedureNode("sumDigits", procLstNode);

		TNode root = createMockProgramNode(name, procedureNode);

		return root;
	}

	TNode MockAst::createSampleAst() {
		/*
		procedure sumDigits	{
		1.  read number;
		}
		*/

		TNode stmt1 = createMockReadNode(1, "number");
		vector<TNode> procBabies = { stmt1 };
		TNode procLstNode = createMockProcStmtLstNode(procBabies);
		TNode procedureNode = createMockProcedureNode("sumDigits", procLstNode);
		TNode root = createMockProgramNode("main", procedureNode);

		return root;
	}

	TNode MockAst::DE_Sample_Main() {
		/*
		procedure sumDigits	{
		1.  read number;
		2.  sum = 0;
		3.  call mummy;

		4.  while (number > 0) {
		5.	  digit = number % 10;
		6.	  sum = sum + digit;
		7.	  number = number / 10;
			}

		8.  if (number < 9000) then {
		9.	    shopee = 1 + 1;
			} else {
		10.	    lazada = 1;
			}
		11.	print sum;
		}
		*/

		TNode stmt1 = createMockReadNode(1, "number");
		VAR_NAMES v2 = {};
		VARS c2 = { "0" };
		TNode stmt2 = createMockAssignNode(2, "0", "sum", v2, c2);
		TNode stmt3 = createMockCallNode(3, "mummy");

		VAR_NAMES v5 = { "number" };
		VARS c5 = { "10" };
		TNode stmt5 = createMockAssignNode(5, "number 10 % ", "digit", v5, c5);
		VAR_NAMES v6 = { "sum", "digit" };
		VARS c6 = {};
		TNode stmt6 = createMockAssignNode(6, "sum digit +", "sum", v6, c6);
		VAR_NAMES v7 = { "number" };
		VARS c7 = { "10" };
		TNode stmt7 = createMockAssignNode(7, "number 10 /", "number", v7, c7);
		vector<TNode> stmt4babies = { stmt5, stmt6, stmt7 };
		TNode stmt4whileLst = createMockWhileStmtLstNode(stmt4babies);
		VAR_NAMES v4 = { "number" };
		VARS c4 = { "0" };
		TNode stmt4 = createMockWhileNode(4, "(number > 10)", v4, c4, stmt4whileLst);

		VAR_NAMES v9 = {};
		VARS c9 = { "1" };
		TNode stmt9 = createMockAssignNode(9, "1 1 +", "shopee", v9, c9);
		VAR_NAMES v10 = {};
		VARS c10 = { "1" };
		TNode stmt10 = createMockAssignNode(10, "1", "lazada", v10, c10);

		vector<TNode> slt8 = { { stmt9 } };
		vector<TNode> sle8 = { { stmt10 } };
		TNode stmt8thenLst = createMockThenStmtLstNode(slt8);
		TNode stmt8elseLst = createMockElseStmtLstNode({ sle8 });

		VAR_NAMES v8 = { "number" };
		VARS c8 = { "9000" };
		TNode stmt8 = createMockIfNode(8, "(number < 9000)", v8, c8, stmt8thenLst, stmt8elseLst);

		TNode stmt11 = createMockPrintNode(11, "sum");

		vector<TNode> procBabies = { stmt1,stmt2,stmt3,stmt4,stmt8,stmt11 };
		TNode procLstNode = createMockProcStmtLstNode(procBabies);
		TNode procedureNode = createMockProcedureNode("sumDigits", procLstNode);

		TNode root = createMockProgramNode("main", procedureNode);

		return root;
	}

	TNode MockAst::DE_sample_follow() {
		/*
		procedure sumDigits	{
		1.  read number;
		2.  print sum;
		}
		*/

		TNode stmt1 = createMockReadNode(1, "number");
		TNode stmt2 = createMockPrintNode(2, "sum");
		TNode stmt3 = createMockCallNode(3, "mummy");
		vector<TNode> procBabies = { stmt1 , stmt2, stmt3 };
		TNode procLstNode = createMockProcStmtLstNode(procBabies);
		TNode procedureNode = createMockProcedureNode("test", procLstNode);
		TNode root = createMockProgramNode("main", procedureNode);

		return root;
	}

	TNode MockAst::DE_sample_parent() {

		/*
		procedure sumDigits	{
		1.	while (number > 0) {
		2.		read number;
			}
		}
		*/
		
		TNode stmt2 = createMockReadNode(2, "number");
		vector<TNode> stmt1babies = { stmt2 };
		TNode stmt1whileLst = createMockWhileStmtLstNode(stmt1babies);
		VAR_NAMES v1 = { "number" };
		VARS c1 = { "0" };
		TNode stmt1 = createMockWhileNode(1, "(number > 10)", v1, c1, stmt1whileLst);
		vector<TNode> procBabies = { stmt1 };
		TNode procLstNode = createMockProcStmtLstNode(procBabies);
		TNode procedureNode = createMockProcedureNode("test", procLstNode);
		TNode root = createMockProgramNode("main", procedureNode);
		return root;
	}

	TNode MockAst::DE_sample_variable() {
		/*
		procedure test	{
		1.  while (number > 0) {
		2.		print sum;
		3.		read number;
		4.		digit = number % 10;
		5.		sum = sum + digit;
		6.		number = number / 10;
			}
			
		}
		*/
		
		TNode stmt2 = createMockPrintNode(2, "sum");
		TNode stmt3 = createMockReadNode(3, "number");
		VAR_NAMES v4 = { "number" };
		VARS c4 = { "10" };
		TNode stmt4 = createMockAssignNode(4, "number 10 % ", "digit", v4, c4);
		VAR_NAMES v5 = { "sum", "digit" };
		VARS c5 = {};
		TNode stmt5 = createMockAssignNode(5, "sum digit +", "sum", v5, c5);
		VAR_NAMES v6 = { "number" };
		VARS c6 = { "10" };
		TNode stmt6 = createMockAssignNode(6, "number 10 /", "number", v6, c6);
		vector<TNode> stmt1babies = { stmt2, stmt3, stmt4, stmt5, stmt6 };
		TNode stmt1whileLst = createMockWhileStmtLstNode(stmt1babies);
		VAR_NAMES v1 = { "number" };
		VARS c1 = { "0" };
		TNode stmt1 = createMockWhileNode(1, "(number > 10)", v1, c1, stmt1whileLst);

		VAR_NAMES v9 = {};
		VARS c9 = { "1" };
		TNode stmt9 = createMockAssignNode(9, "1 1 +", "shopee", v9, c9);
		VAR_NAMES v10 = {};
		VARS c10 = { "1" };
		TNode stmt10 = createMockAssignNode(10, "1", "lazada", v10, c10);

		vector<TNode> slt8 = { { stmt9 } };
		vector<TNode> sle8 = { { stmt10 } };
		TNode stmt8thenLst = createMockThenStmtLstNode(slt8);
		TNode stmt8elseLst = createMockElseStmtLstNode({ sle8 });

		VAR_NAMES v8 = { "number" };
		VARS c8 = { "9000" };
		TNode stmt8 = createMockIfNode(8, "(number < 9000)", v8, c8, stmt8thenLst, stmt8elseLst);

		vector<TNode> procBabies = { stmt1 };
		TNode procLstNode = createMockProcStmtLstNode(procBabies);
		TNode procedureNode = createMockProcedureNode("test", procLstNode);
		TNode root = createMockProgramNode("main", procedureNode);

		return root;
	}
}
