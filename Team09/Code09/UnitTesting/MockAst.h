#pragma once

#include <queue>
#include <set>
#include "stdafx.h"
#include "TNode.h"

namespace MockAst {
	TNode createMockReadNode(int stmtNo, string modifiedVar);
	TNode createMockPrintNode(int stmtNo, string usedVar);
	TNode createMockCallNode(int stmtNo, string procedureName);
	TNode createMockAssignNode(int stmtNo, string rpn, string variable, VAR_NAMES vars, VARS con);
	TNode createMockWhileNode(int stmtNo, string condExpr, VAR_NAMES vars, VARS con, TNode whileStmtLst);
	TNode createMockProcedureNode(string var, TNode procedureStmtLst);
	TNode createMockIfNode(int stmtNo, string condExpr, VAR_NAMES vars, VARS con, TNode thenStmtLst, TNode elseStmtLst);

	TNode createMockProcStmtLstNode(vector<TNode> childs);
	TNode createMockWhileStmtLstNode(vector<TNode> childs);
	TNode createMockThenStmtLstNode(vector<TNode> childs);
	TNode createMockElseStmtLstNode(vector<TNode> childs);
	TNode createMockProgramNode(string name, TNode procedure);

	TNode createSampleAst1(string name);
	TNode createSampleAst();

	TNode DE_Sample_Main();
	TNode DE_sample_follow();
	TNode DE_sample_parent();
	TNode DE_sample_variable();
}