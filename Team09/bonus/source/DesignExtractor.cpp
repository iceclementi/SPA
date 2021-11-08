#include "DesignExtractor.h"

namespace DesignExtractor {

	DesignExtractor::DesignExtractor(TNode& root, PROC_MATRIX& matrix) : root(root), procMatrix(matrix) {
		reverseSortRootChildrens();
		nodeStack.push(root);
		pkb.setASTRoot(root);
	}

	void DesignExtractor::traverse() {
		depthFirstTraverse(root);
		updateProcModifiesFromCalls(currentProc);
		updateProcUsesFromCalls(currentProc);
	}


	void DesignExtractor::depthFirstTraverse(TNode& node) {
		// change state of node to traversed
		node.traverse();

		if (isContainer(node)) {
			parents.push_back(to_string(node.getStmtNum()));
		}

		for (auto& child : node.getChildren()) {
			if (!child.hasTraversed()) {
				depthFirstTraverse(child);
			}
		}

		if (isContainer(node)) {
			parents.pop_back();
			if (node.getType() == TNode::NODE_TYPE::whileStmt) {
				extractWhileNext(node, node);
			}
		}

		if (isProcedure(node)) {
			extractProcedure(node);
		}

		if (isStmtLst(node)) {
			extractFollows(node);
		}

		if (isStatement(node)) {
			extractStatement(node);
		}
	}

	vector<PROC_NAME> DesignExtractor::toposort() {
		vector<VAR> result;
		vector<VAR> stack;
		posTable = {};

		for (auto& t : procMatrix) {
			string key = t.first;
			if (procMatrix[key][key] != 1) {
				dfs(key, stack);
			}
		}

		int index = 0;
		while (!stack.empty()) {
			string top = stack.back();
			posTable[top] = index;
			result.emplace_back(top);
			stack.pop_back();
			index = index + 1;
		}

		checkCyclic(result);
		result = reverse(result);
		int i = 0;
		extractCalls(result);

		return result;
	}

	vector<PROC_NAME> DesignExtractor::reverse(vector<PROC_NAME>& procNames) {
		vector<PROC_NAME> result;
		while (!procNames.empty()) {
			result.emplace_back(procNames.back());
			procNames.pop_back();
		}
		return result;
	}

	void DesignExtractor::reverseSortRootChildrens() {
		vector<PROC_NAME> sort = toposort();
		NODE_LIST& childs = root.getChildren();
		NODE_LIST sortedChilds;

		for (string t : sort) {
			for (TNode c : childs) {
				if (c.getValue() == t) {
					sortedChilds.emplace_back(c);
				}
			}
		}

		for (auto& c : childs) {
			if (procMatrix.find(c.getValue()) == procMatrix.end()) {
				sortedChilds.emplace_back(c);
			}
		}

		root.setChildren(sortedChilds);
	}

	void DesignExtractor::checkCyclic(vector<PROC_NAME>& result) {
		for (auto& proc : result) {
			for (auto& calls : procMatrix[proc]) {
				PROC_NAME call = calls.first;
				if (posTable[proc] > posTable[call]) {
					throw "Cyclic call detected between " + proc + " and " + call;
				}
			}
		}
	}

	void DesignExtractor::dfs(PROC_NAME proc, vector<PROC_NAME>& stack) {
		procMatrix[proc][proc] = 1;
		for (auto const& calls : procMatrix[proc]) {
			string call = calls.first;
			if (procMatrix[call][call] != 1) {
				dfs(call, stack);
			}
		}
		stack.emplace_back(proc);
	}


	void DesignExtractor::createStmtToNodeMapping(TNode& node) {

		for (TNode proc : node.getChildren()) {
			TNode proc_stmtlst = proc.getChildren()[0];
			createMapping(proc_stmtlst, 0);
		}
	};


	void DesignExtractor::createMapping(TNode& node, int magic) {
		int size = node.getChildren().size();
		for (int i = 0; i < size; i++) {
			TNode s = node.getChildren()[i];

			if (s.getType() == TNode::NODE_TYPE::ifStmt) {
				createMapping(s.getChildren()[0], magic);
				createMapping(s.getChildren()[1], magic);
			}

			if (s.getType() == TNode::NODE_TYPE::whileStmt) {
				if (magic == 0) {
					createMapping(s.getChildren()[0], s.getStmtNum());
				} else {
					createMapping(s.getChildren()[0], magic);
				}
			}

			if (s.getType() == TNode::NODE_TYPE::assignStmt) {
				if (magic == 0) {
					stmtToNodeMapping.insert({ s.getStmtNum(), s.getStmtNum() });
				}
				else {
					stmtToNodeMapping.insert({ s.getStmtNum(), magic });
				}
			}

		}
	}

	bool DesignExtractor::isStmtLst(TNode& node) {
		return (node.getType() == TNode::NODE_TYPE::procedureStmtLst ||
			node.getType() == TNode::NODE_TYPE::thenStmtLst ||
			node.getType() == TNode::NODE_TYPE::elseStmtLst ||
			node.getType() == TNode::NODE_TYPE::whileStmtLst);
	}

	bool DesignExtractor::isStatement(TNode& node) {
		return (node.getType() == TNode::NODE_TYPE::readStmt ||
			node.getType() == TNode::NODE_TYPE::printStmt ||
			node.getType() == TNode::NODE_TYPE::callStmt ||
			node.getType() == TNode::NODE_TYPE::whileStmt ||
			node.getType() == TNode::NODE_TYPE::ifStmt ||
			node.getType() == TNode::NODE_TYPE::assignStmt
			);
	}

	bool DesignExtractor::isContainer(TNode& node) {
		return (node.getType() == TNode::NODE_TYPE::whileStmt ||
			node.getType() == TNode::NODE_TYPE::ifStmt);
	}

	bool DesignExtractor::isStmtContainer(TNode& node) {
		return (node.getType() == TNode::NODE_TYPE::thenStmtLst ||
			node.getType() == TNode::NODE_TYPE::elseStmtLst ||
			node.getType() == TNode::NODE_TYPE::whileStmtLst);
	}

	bool DesignExtractor::isProcedure(TNode& node) {
		return (node.getType() == TNode::NODE_TYPE::procedure);
	}

	void DesignExtractor::extractStatement(TNode& node) {
		DESIGN_ENTITY deType;
		STMT_NUM stmtNum = to_string(node.getStmtNum());

		switch (node.getType()) {
		case TNode::NODE_TYPE::readStmt:
			deType = DESIGN_ENTITY::READ;
			addStatementVar(deType, stmtNum, node.getValue());
			break;
		case TNode::NODE_TYPE::printStmt:
			deType = DESIGN_ENTITY::PRINT;
			addStatementVar(deType, stmtNum, node.getValue());
			break;
		case TNode::NODE_TYPE::callStmt:
			deType = DESIGN_ENTITY::CALL;
			addStatementVar(deType, stmtNum, node.getValue());
			extractCalleeUses(node.getValue(), stmtNum);
			extractCalleeModifies(node.getValue(), stmtNum);
			break;
		case TNode::NODE_TYPE::assignStmt:
			deType = DESIGN_ENTITY::ASSIGN;
			extractAssignPattern(stmtNum, *node.getModifiedVariables().begin(), node.getValue());
			break;
		case TNode::NODE_TYPE::whileStmt:
			deType = DESIGN_ENTITY::WHILE;
			extractWhilePattern(stmtNum, node.getUsedVariables());
			break;
		case TNode::NODE_TYPE::ifStmt:
			deType = DESIGN_ENTITY::IF;
			extractIfPattern(stmtNum, node.getUsedVariables());
			break;
		default:
			throw "Invalid AST statement type";
		}

		addStatement(deType, stmtNum);
		extractUses(stmtNum, node.getUsedVariables());
		extractModifies(stmtNum, node.getModifiedVariables());
		extractConstants(node.getConstants());
		extractParents(node);
	}

	void DesignExtractor::extractProcedure(TNode& node) {
		PROC_NAME procName = node.getValue();

		if (allProcs.count(procName)) {
			throw "Error: Duplicate procedure names";
		}

		addDEValue(DESIGN_ENTITY::PROCEDURE, procName);
		allProcs.insert(procName);

		auto procStmtList = node.getChildren()[0];
		for (auto& child : procStmtList.getChildren()) {
			VAR childStmtNum = to_string(child.getStmtNum());
			copyValues(RELATIONSHIP_TYPE::USES_STMT, childStmtNum, RELATIONSHIP_TYPE::USES_PROC, procName);
			copyValues(RELATIONSHIP_TYPE::MODIFIES_STMT, childStmtNum, RELATIONSHIP_TYPE::MODIFIES_PROC, procName);
		}

		updateProcModifiesFromCalls(currentProc);
		updateProcUsesFromCalls(currentProc);
		currentProc = procName;
	}

	void DesignExtractor::extractWhileNext(TNode& whileNode, TNode& currentNode) {
		STMT_NUM whileNodeNum = to_string(whileNode.getStmtNum());
		NODE_LIST& whileStmtList = whileNode.getChildren()[0].getChildren();
		STMT_NUM firstStatementNum = to_string(whileStmtList.front().getStmtNum());
		// not in recursion
		if (whileNode.isEqual(currentNode)) {
			// last element in while stmt list is an if statement, call recursion for each branch
			if (whileStmtList.back().getType() == TNode::NODE_TYPE::ifStmt) {
				TNode ifNode = whileStmtList.back();
				extractWhileNext(whileNode, ifNode.getChildren()[0].getChildren().back());
				extractWhileNext(whileNode, ifNode.getChildren()[1].getChildren().back());
				addRelationship(RELATIONSHIP_TYPE::NEXT, whileNodeNum, firstStatementNum);
			}
			// basic case
			else {
				STMT_NUM lastStatementNum = to_string(whileStmtList.back().getStmtNum());
				addRelationship(RELATIONSHIP_TYPE::NEXT, whileNodeNum, firstStatementNum);
				addRelationship(RELATIONSHIP_TYPE::NEXT, lastStatementNum, whileNodeNum);
			}
		}
		else {
			// in recursion, current node is a if node
			if (currentNode.getType() == TNode::NODE_TYPE::ifStmt) {
				TNode ifNode = currentNode;
				extractWhileNext(whileNode, ifNode.getChildren()[0].getChildren().back());
				extractWhileNext(whileNode, ifNode.getChildren()[1].getChildren().back());

			}
			// in recursion, current node is not an if node
			else {
				STMT_NUM currentNum = to_string(currentNode.getStmtNum());
				addRelationship(RELATIONSHIP_TYPE::NEXT, currentNum, whileNodeNum);
			}
		}
	}

	void DesignExtractor::extractIfElseNext(TNode& ifNode, TNode& currentNode, STMT_NUM after) {
		STMT_NUM ifNodeNum = to_string(ifNode.getStmtNum());
		NODE_LIST& ifStmtLst = ifNode.getChildren()[0].getChildren();
		NODE_LIST& elseStmtLst = ifNode.getChildren()[1].getChildren();
		if (ifNode.isEqual(currentNode)) {
			//add next for if stmt to first in both stmtlst
			STMT_NUM ifFirstStatementNum = to_string(ifStmtLst.front().getStmtNum());
			STMT_NUM elseFirstStatementNum = to_string(elseStmtLst.front().getStmtNum());
			addRelationship(RELATIONSHIP_TYPE::NEXT, ifNodeNum, ifFirstStatementNum);
			addRelationship(RELATIONSHIP_TYPE::NEXT, ifNodeNum, elseFirstStatementNum);

			// not last statement in statement list
			if (after != to_string(0)) {
				TNode ifStmtLstLastNode = ifStmtLst.back();
				TNode elseStmtLstLastNode = elseStmtLst.back();
				if (ifStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt && elseStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt) {
					extractIfElseNext(ifNode, ifStmtLstLastNode, after);
					extractIfElseNext(ifNode, elseStmtLstLastNode, after);
				}
				else if (ifStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt) {
					STMT_NUM elseLastStatementNum = to_string(elseStmtLstLastNode.getStmtNum());
					addRelationship(RELATIONSHIP_TYPE::NEXT, elseLastStatementNum, after);
					extractIfElseNext(ifNode, ifStmtLstLastNode, after);
				}
				else if (elseStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt) {
					STMT_NUM ifLastStatementNum = to_string(ifStmtLstLastNode.getStmtNum());
					addRelationship(RELATIONSHIP_TYPE::NEXT, ifLastStatementNum, after);
					extractIfElseNext(ifNode, elseStmtLstLastNode, after);
				}
				// add next for last in both stmtlst to next line after if stmt
				else {
					STMT_NUM ifLastStatementNum = to_string(ifStmtLstLastNode.getStmtNum());
					STMT_NUM elseLastStatementNum = to_string(elseStmtLstLastNode.getStmtNum());
					addRelationship(RELATIONSHIP_TYPE::NEXT, ifLastStatementNum, after);
					addRelationship(RELATIONSHIP_TYPE::NEXT, elseLastStatementNum, after);
				}
			}
		}
		// in recursion
		else {
			NODE_LIST& currentIfStmtLst = currentNode.getChildren()[0].getChildren();
			NODE_LIST& currentElseStmtLst = currentNode.getChildren()[1].getChildren();
			TNode ifStmtLstLastNode = currentIfStmtLst.back();
			TNode elseStmtLstLastNode = currentElseStmtLst.back();
			// both branches end in a if-else branch
			if (ifStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt && elseStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt) {
				extractIfElseNext(ifNode, ifStmtLstLastNode, after);
				extractIfElseNext(ifNode, elseStmtLstLastNode, after);
			}
			// only if branch ends in a if-else branch
			else if (ifStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt) {
				STMT_NUM elseLastStatementNum = to_string(elseStmtLstLastNode.getStmtNum());
				addRelationship(RELATIONSHIP_TYPE::NEXT, elseLastStatementNum, after);
				extractIfElseNext(ifNode, ifStmtLstLastNode, after);
			}
			// only else branch ends in a if-else branch
			else if (elseStmtLstLastNode.getType() == TNode::NODE_TYPE::ifStmt) {
				STMT_NUM ifLastStatementNum = to_string(ifStmtLstLastNode.getStmtNum());
				addRelationship(RELATIONSHIP_TYPE::NEXT, ifLastStatementNum, after);
				extractIfElseNext(ifNode, elseStmtLstLastNode, after);
			}
			// none of the branches end in a if-else branch
			else {
				STMT_NUM ifLastStatementNum = to_string(ifStmtLstLastNode.getStmtNum());
				STMT_NUM elseLastStatementNum = to_string(elseStmtLstLastNode.getStmtNum());
				addRelationship(RELATIONSHIP_TYPE::NEXT, ifLastStatementNum, after);
				addRelationship(RELATIONSHIP_TYPE::NEXT, elseLastStatementNum, after);
			}
		}
	}

	void DesignExtractor::extractCalls(vector<PROC_NAME>& toposort) {
		for (auto& caller : toposort) {
			for (auto& t : procMatrix[caller]) {
				PROC_NAME calledName = t.first;
				if (calledName != caller) {
					addRelationship(RELATIONSHIP_TYPE::CALLS, caller, calledName);
					addRelationship(RELATIONSHIP_TYPE::CALLS_STAR, caller, calledName);
					copyValues(RELATIONSHIP_TYPE::CALLS_STAR, calledName, RELATIONSHIP_TYPE::CALLS_STAR, caller);
				}
			}
		}
	}

	void DesignExtractor::extractFollows(TNode& currentNode) {
		NODE_LIST& stmtList = currentNode.getChildren();
		int size = stmtList.size();
		int externalIt = 0;
		int internalIt = 1;
		// if-statement is the only statement in the stmtlst
		if (size == 1 && stmtList[externalIt].getType() == TNode::NODE_TYPE::ifStmt) {
			extractIfElseNext(stmtList[externalIt], stmtList[externalIt], to_string(0));
		}
		// if-statement is the last element in the stmtlst
		if (size != 1 && stmtList.back().getType() == TNode::NODE_TYPE::ifStmt) {
			extractIfElseNext(stmtList.back(), stmtList.back(), to_string(0));
		}

		while (internalIt < size) {
			STMT_NUM first = to_string(stmtList[externalIt].getStmtNum());
			STMT_NUM second = to_string(stmtList[internalIt].getStmtNum());
			addRelationship(RELATIONSHIP_TYPE::FOLLOWS, first, second);
			addRelationship(RELATIONSHIP_TYPE::FOLLOWS_STAR, first, second);
			if (stmtList[externalIt].getType() != TNode::NODE_TYPE::ifStmt) {
				addRelationship(RELATIONSHIP_TYPE::NEXT, first, second);
			}
			else {
				// if-statement is in the middle of the stmtlst
				extractIfElseNext(stmtList[externalIt], stmtList[externalIt], second);
			}
			internalIt++;
			while (internalIt < size) {
				STMT_NUM secondStar = to_string(stmtList[internalIt].getStmtNum());
				addRelationship(RELATIONSHIP_TYPE::FOLLOWS_STAR, first, secondStar);
				internalIt++;
			}
			externalIt++;
			internalIt = externalIt + 1;
		}
	}

	void DesignExtractor::extractParents(TNode& node) {
		if (node.getParent()) {
			STMT_NUM stmtNum = to_string(node.getStmtNum());
			STMT_NUM parentStmtNum = to_string(node.getParent());
			addRelationship(RELATIONSHIP_TYPE::PARENT, parentStmtNum, stmtNum);

			for (auto& parent : parents) {
				addRelationship(RELATIONSHIP_TYPE::PARENT_STAR, parent, stmtNum);
			}
			copyValues(RELATIONSHIP_TYPE::USES_STMT, stmtNum, RELATIONSHIP_TYPE::USES_STMT, parentStmtNum);
			copyValues(RELATIONSHIP_TYPE::MODIFIES_STMT, stmtNum, RELATIONSHIP_TYPE::MODIFIES_STMT, parentStmtNum);
		}
	}

	void DesignExtractor::extractConstants(CONSTS& constants) {
		for (auto& value : constants) {
			addDEValue(DESIGN_ENTITY::CONSTANT, value);
		}
	}

	void DesignExtractor::extractUses(STMT_NUM stmtNum, VAR_NAMES& varNames) {
		for (auto& varName : varNames) {
			addRelationship(RELATIONSHIP_TYPE::USES_STMT, stmtNum, varName);
			addDEValue(DESIGN_ENTITY::VARIABLE, varName);
		}
	}

	void DesignExtractor::extractModifies(STMT_NUM stmtNum, VAR_NAMES& varNames) {
		for (auto& varName : varNames) {
			addRelationship(RELATIONSHIP_TYPE::MODIFIES_STMT, stmtNum, varName);
			addDEValue(DESIGN_ENTITY::VARIABLE, varName);
		}
	}

	void DesignExtractor::extractCalleeUses(PROC_NAME from, STMT_NUM to) {
		copyValues(RELATIONSHIP_TYPE::USES_PROC, from, RELATIONSHIP_TYPE::USES_STMT, to);
	}

	void DesignExtractor::extractCalleeModifies(PROC_NAME from, STMT_NUM to) {
		copyValues(RELATIONSHIP_TYPE::MODIFIES_PROC, from, RELATIONSHIP_TYPE::MODIFIES_STMT, to);
	}

	void DesignExtractor::extractAssignPattern(STMT_NUM stmtNum, VAR_NAME varName, PATTERN pattern) {
		addPattern(stmtNum, varName, pattern);
	}

	void DesignExtractor::extractWhilePattern(STMT_NUM stmtNum, VAR_NAMES& condVars) {
		addCondVar(RELATIONSHIP_TYPE::WHILE_PATTERN, stmtNum, condVars);
	}

	void DesignExtractor::extractIfPattern(STMT_NUM stmtNum, VAR_NAMES& condVars) {
		addCondVar(RELATIONSHIP_TYPE::IF_PATTERN, stmtNum, condVars);
	}

	bool DesignExtractor::CFGTraversal(STMT_NUM before, STMT_NUM after, STMT_NUM a2, VAR_NAMES& var_modified) {
		// check if variables are modified along the way
		for (auto& v : var_modified) {
			// if they are modified, return true
			if (pkb.hasRelationship(RELATIONSHIP_TYPE::MODIFIES_STMT, after, v)) {
				return true;
			}
		}
		// if it reaches a2 without being modified, return false
		if (after == a2) {
			return false;
		}
		else {
			bool result = false;
			//should be all types but idk how
			VALUES next_stmts = pkb.getSecondValuesOf(RELATIONSHIP_TYPE::NEXT, DESIGN_ENTITY::PROG_LINE, before);
			// traverse the CFG
			for (auto& f : next_stmts) {
				// a single true will cause the function to return true
				STMT_NUM nextStmt = pkb.getStringValueOfDE(f);
				result = result || CFGTraversal(after, nextStmt, a2, var_modified);
			}
			return result;
		}
	}

	void DesignExtractor::addStatement(DESIGN_ENTITY deType, STMT_NUM stmtNum) {
		pkb.addStmt(deType, stmtNum);
	}

	void DesignExtractor::addStatementVar(DESIGN_ENTITY deType, STMT_NUM stmtNum, VAR var) {
		pkb.addStmtVar(deType, stmtNum, var);
	}

	void DesignExtractor::addDEValue(DESIGN_ENTITY deType, VAR value) {
		pkb.addDEValue(deType, value);
	}

	void DesignExtractor::addRelationship(RELATIONSHIP_TYPE rType, VAR firstValue, VAR secondValue) {
		pkb.addRelationship(rType, firstValue, secondValue);
	}

	void DesignExtractor::copyValues(RELATIONSHIP_TYPE fromRType, VAR from, RELATIONSHIP_TYPE toRType, VAR to) {
		pkb.copyValues(fromRType, from, toRType, to);
	}

	void DesignExtractor::addPattern(STMT_NUM stmtNum, VAR_NAME varName, PATTERN pattern) {
		pkb.addPattern(stmtNum, varName, pattern);
	}

	void DesignExtractor::addCondVar(RELATIONSHIP_TYPE rType, STMT_NUM stmtNum, VARS& condVars) {
		pkb.addCondVar(rType, stmtNum, condVars);
	}

	void DesignExtractor::updateProcModifiesFromCalls(PROC_NAME to) {
		if (procMatrix.find(to) != procMatrix.end() && to != "") {
			for (auto& t : procMatrix[to]) {
				copyValues(RELATIONSHIP_TYPE::MODIFIES_PROC, t.first, RELATIONSHIP_TYPE::MODIFIES_PROC, to);
			}
		}
	}

	void DesignExtractor::updateProcUsesFromCalls(PROC_NAME to) {
		if (procMatrix.find(to) != procMatrix.end() && to != "") {
			for (auto& t : procMatrix[to]) {
				copyValues(RELATIONSHIP_TYPE::USES_PROC, t.first, RELATIONSHIP_TYPE::USES_PROC, to);
			}
		}
	}

}