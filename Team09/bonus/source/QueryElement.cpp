#include "QueryElement.h"

QueryElement::QueryElement(string em, SYNONYM_INDEX si, DESIGN_ENTITY st, ATTRIBUTE_TYPE at) {
	errorMessage = em;
	synIndex = si;
	synType = st;
	attrType = at;
}

bool QueryElement::hasError() {
	return errorMessage != NO_ERROR;
}

string QueryElement::getErrorMessage() {
	return errorMessage;
}

void QueryElement::setErrorMessage(string em) {
	errorMessage = em;
}

SYNONYM_INDEX QueryElement::getSynIndex() {
	return synIndex;
}

void QueryElement::setSynIndex(SYNONYM_INDEX si) {
	synIndex = si;
}

DESIGN_ENTITY QueryElement::getSynType() {
	return synType;
}

void QueryElement::setSynType(DESIGN_ENTITY st) {
	synType = st;
}

ATTRIBUTE_TYPE QueryElement::getAttrType() {
	return attrType;
}

void QueryElement::setAttrType(ATTRIBUTE_TYPE at) {
	attrType = at;
}
