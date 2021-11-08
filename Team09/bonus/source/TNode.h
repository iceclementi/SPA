#pragma once

#include "Common.h"

class TNode;
typedef vector<TNode> NODE_LIST;

class TNode {
public:
	enum class NODE_TYPE
	{
		program, procedure, procedureStmtLst, thenStmtLst, elseStmtLst, whileStmtLst, condExpr,
		readStmt, printStmt, callStmt, whileStmt, ifStmt, assignStmt
	};

	TNode(NODE_TYPE type);				// for statement lists
	TNode(NODE_TYPE type, int stmtNum);	// for statements
	TNode(NODE_TYPE type, VAR value);	// for procedures //variables
	TNode(NODE_TYPE type, int stmtNum, VAR value); // for statements 
	NODE_TYPE getType();
	int getParent();
	void setParent();
	void setParent(int parent);
	NODE_LIST getChildren();
	int getStmtNum();
	VAR getValue();
	void traverse();
	bool hasTraversed();

	VAR_NAMES& getUsedVariables();
	VAR_NAMES& getModifiedVariables();
	CONSTS& getConstants();

	void addChild(TNode& child);
	void addUsedVariable(VAR_NAME varName);
	void addUsedVariables(VAR_NAMES& varNames);
	void addModifiedVariable(VAR_NAME varName);
	void addModifiedVariables(VAR_NAMES& varNames);
	void addConstant(CONST constant);
	void addConstants(CONSTS& constants);
	void setChildren(NODE_LIST& childrens);
	
	static void enterContainer(int parentNum);
	static void exitContainer();

	void clear();

	bool isEqual(TNode toCheck);

private:
	static stack<int> parentStack;

	NODE_TYPE type;
	NODE_LIST children;
	int parent = 0;
	int stmtNum = 0;
	string value = "";
	bool traversed = false;
	VAR_NAMES usedVariables;
	VAR_NAMES modifiedVariables;
	CONSTS constants;
};

inline bool operator== (TNode& lhs, TNode& rhs) {
	return lhs.getType() == rhs.getType() && lhs.getParent() == rhs.getParent() && lhs.getStmtNum() == rhs.getStmtNum() 
		&& lhs.getValue() == rhs.getValue() && lhs.getUsedVariables() == rhs.getUsedVariables() 
		&& lhs.getModifiedVariables() == rhs.getModifiedVariables() && lhs.getConstants() == rhs.getConstants();
}

inline bool operator!= (TNode& lhs, TNode& rhs) {
	return !(lhs == rhs);
}
