#pragma once

#include "Common.h"
#include "TNode.h"
#include "PKB.h"

typedef unordered_map<string, unordered_map<string, int>> PROC_MATRIX;
typedef unordered_map<string, int> POS_TABLE;

namespace DesignExtractor {

	class DesignExtractor {
	public:
		/*-------------------------------------*\
		|            DE Constructor             |
		\*-------------------------------------*/

		DesignExtractor(TNode& root, PROC_MATRIX& procMatrix);

		/*------------------------------------------*\
		|            Parser-PKB functions            |
		\*------------------------------------------*/

		void traverse();
		void addStatement(DESIGN_ENTITY stmtType, STMT_NUM stmtNum);
		void addStatementVar(DESIGN_ENTITY stmtType, STMT_NUM stmtNum, VAR var);
		void addDEValue(DESIGN_ENTITY deType, VAR value);
		void addRelationship(RELATIONSHIP_TYPE rType, VAR firstValue, VAR secondValue);
		void copyValues(RELATIONSHIP_TYPE fromRType, VAR from, RELATIONSHIP_TYPE toRType, VAR to);
		void addPattern(STMT_NUM stmtNum, VAR_NAME varName, PATTERN pattern);
		void addCondVar(RELATIONSHIP_TYPE rType, STMT_NUM stmtNum, VARS& condVars);

	private:
		/*----------------------------*\
		|           Objects            |
		\*----------------------------*/

		TNode& root;
		PKB pkb;
		PROC_MATRIX procMatrix;
		POS_TABLE posTable;
		vector<VAR> parents;
		stack<TNode> nodeStack;
		VAR currentProc;
		VARS allProcs;
		unordered_map<VALUE, VALUE> stmtToNodeMapping;

		/*--------------------------------------------*\
		|           Type-Checking Functions            |
		\*--------------------------------------------*/

		bool isStmtLst(TNode& node);
		bool isStatement(TNode& node);
		bool isContainer(TNode& node);
		bool isStmtContainer(TNode& node);
		bool isProcedure(TNode& node);

		/*-----------------------------------------*\
		|            Extractor Functions            |
		\*-----------------------------------------*/

		void extractFollows(TNode& node);
		void extractParents(TNode& node);
		void extractProcedure(TNode& node);
		void extractStatement(TNode& node);
		void extractConstants(CONSTS& values);
		void extractUses(STMT_NUM stmtNum, VAR_NAMES& varNames);
		void extractModifies(STMT_NUM stmtNum, VAR_NAMES& varNames);
		void extractAssignPattern(STMT_NUM stmtNum, VAR_NAME varName, PATTERN pattern);
		void extractWhilePattern(STMT_NUM stmtNum, VAR_NAMES& condVars);
		void extractIfPattern(STMT_NUM stmtNum, VAR_NAMES& condVars);
		void extractCalleeUses(PROC_NAME from, STMT_NUM to);
		void extractCalleeModifies(PROC_NAME from, STMT_NUM to);
		void extractCalls(vector<PROC_NAME>& toposort);
		void extractIfElseNext(TNode& ifNode, TNode& currentNode, STMT_NUM next);
		void extractWhileNext(TNode& whileNode, TNode& currentNode);
		void checkCyclic(vector<PROC_NAME>& stack);
		void createStmtToNodeMapping(TNode& node);
		void createMapping(TNode& node, int flag);


		/*---------------------------------------------*\
		|            Miscellaneous Functions            |
		\*---------------------------------------------*/

		void depthFirstTraverse(TNode& node);
		void dfs(PROC_NAME proc, vector<PROC_NAME>& stack);
		vector<PROC_NAME> reverse(vector<PROC_NAME>& list);
		void updateProcModifiesFromCalls(PROC_NAME to);
		void updateProcUsesFromCalls(PROC_NAME to);
		void reverseSortRootChildrens();
		bool CFGTraversal(STMT_NUM before, STMT_NUM after, STMT_NUM a2, VAR_NAMES& var_modified);
		vector<VAR> toposort();
	};
}
