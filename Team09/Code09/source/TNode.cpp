#include "TNode.h"

stack<int> TNode::parentStack = stack<int>();

TNode::TNode(NODE_TYPE type) : type(type) {}

TNode::TNode(NODE_TYPE type, int stmtNum, VAR value) : type(type), stmtNum(stmtNum), value(value) {
	setParent();
}

TNode::TNode(NODE_TYPE type, int stmtNum) : TNode::TNode(type, stmtNum, "") {}

TNode::TNode(NODE_TYPE type, VAR value) : type(type), value(value) {}

TNode::NODE_TYPE TNode::getType() {
	return type;
}

int TNode::getParent() {
	return parent;
}

void TNode::setParent() {
	if (!parentStack.empty()) {
		parent = parentStack.top();
	}
}

void TNode::setParent(int parent) {
	this->parent = parent;
}

NODE_LIST TNode::getChildren() {
	return children;
}

int TNode::getStmtNum() {
	return stmtNum;
}

VAR TNode::getValue() {
	return value;
}

void TNode::traverse() {
	traversed = true;
}

bool TNode::hasTraversed() {
	return traversed;
}

VAR_NAMES& TNode::getUsedVariables() {
	return usedVariables;
}
VAR_NAMES& TNode::getModifiedVariables() {
	return modifiedVariables;
}

CONSTS& TNode::getConstants() {
	return constants;
}

void TNode::addChild(TNode &child) {
	children.push_back(child);
}

void TNode::addUsedVariable(VAR_NAME varName) {
	usedVariables.insert(varName);
}

void TNode::addUsedVariables(VAR_NAMES& varNames) {
	for (auto& name : varNames) {
		addUsedVariable(name);
	}
}

void TNode::addModifiedVariable(VAR_NAME varName) {
	modifiedVariables.insert(varName);
}

void TNode::addModifiedVariables(VAR_NAMES& varNames) {
	for (auto& name : varNames) {
		addModifiedVariable(name);
	}
}

void TNode::addConstant(CONST constant) {
	constants.insert(constant);
}

void TNode::addConstants(CONSTS& constants) {
	for (auto& value : constants) {
		addConstant(value);
	}
}

void TNode::enterContainer(int parentNum) {
	parentStack.push(parentNum);
}

void TNode::exitContainer() {
	parentStack.pop();
}

void TNode::clear() {
	parentStack = stack<int>();
	children.clear();
	parent = 0;
	stmtNum = 0;
	value = "";
	traversed = false;
	usedVariables.clear();
	modifiedVariables.clear();
	constants.clear();
}

void TNode::setChildren(NODE_LIST& update) {
	children = update;
}

bool TNode::isEqual(TNode toCheck) {
	if (*this != toCheck) {
		return false;
	}

	if (children.size() != toCheck.children.size()) {
		return false;
	}

	for (unsigned int i = 0; i < children.size(); i++) {
		if (!children[i].isEqual(toCheck.children[i])) {
			return false;
		}
	}

	return true;
}
