#include "QueryArgument.h"

QueryArgument::QueryArgument(string em, ARGUMENT_TYPE at, ARGUMENT as, QueryElement ae) {
	errorMessgae = em;
	argType = at;
	argStr = as;
	argElem = ae;
}

bool QueryArgument::hasError() {
	return errorMessgae != NO_ERROR;
}

string QueryArgument::getErrorMessage() {
	return errorMessgae;
}

void QueryArgument::setErrorMessage(string em) {
	errorMessgae = em;
}

ARGUMENT_TYPE QueryArgument::getArgType() {
	return argType;
}

void QueryArgument::setArgType(ARGUMENT_TYPE at) {
	argType = at;
}

string QueryArgument::getArgStr() {
	return argStr;
}

void QueryArgument::setArgStr(string as) {
	argStr = as;
}

QueryElement QueryArgument::getArgElem() {
	return argElem;
}

void QueryArgument::setArgElem(QueryElement ae) {
	argElem = ae;
}
