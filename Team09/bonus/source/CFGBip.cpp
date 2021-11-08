
#include "CFGBip.h"

CFGBip::CFGBip() {
}

CFGBip::CFGBip(PROG_LINE progLine) {
	this->progLine = progLine;
	this->nodeType = NODE_TYPE::BASIC;
}

CFGBip::CFGBip(PROG_LINE progLine, NODE_TYPE nodeType) {
	this->progLine = progLine;
	this->nodeType = nodeType;
}

bool CFGBip::addChildren(CFGBip &node) {
	children.reserve(5);
	children.push_back(node);
	return true;
}


bool CFGBip::addDummyParents(CFGBip* node) {
	dummyParents.push_back(node);
	return true;
}

bool CFGBip::isDummy() {
	return progLine == "0";
}

bool CFGBip::isBasic() {
	return children.size() == 1 && nodeType == NODE_TYPE::BASIC;
}

bool CFGBip::isWhile() {
	return nodeType == NODE_TYPE::WHILE;
}

bool CFGBip::isCall() {
	return nodeType == NODE_TYPE::CALL;
}

bool CFGBip::isIf() {
	return nodeType == NODE_TYPE::IF;
}

bool CFGBip::isIfEnd() {
	return nodeType == NODE_TYPE::IF_END;
}

bool CFGBip::areEqual(CFGBip node) {
	return progLine == node.getProgLine();
}

void CFGBip::setType(NODE_TYPE nodeType) {
	this->nodeType = nodeType;
}


CFGBip& CFGBip::getSmallNext() {
	for (CFGBip& child : children) {
		if (stoi(child.getProgLine()) == stoi(this->progLine) + 1) {
			return child;
		}
	}
	return *this;
}

CFGBip& CFGBip::getLargeNext() {
	for (CFGBip& child : children) {
		if (stoi(child.getProgLine()) > stoi(this->progLine) + 1) {
			return child;
		}
	}
	return *this;
}

vector<CFGBip>& CFGBip::getChildren() {
	return children;
}

vector<CFGBip*> CFGBip::getDummyParents() {
	return dummyParents;
}

PROG_LINE CFGBip::getProgLine() {
	return progLine;
}
