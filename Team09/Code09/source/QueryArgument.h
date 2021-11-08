#pragma once

#include "Query.h"
#include "QueryElement.h"

using namespace std;

typedef string ARGUMENT;

/*
* Enum to describe a possible argument type
*/
enum class ARGUMENT_TYPE {
	SYNONYM, UNDERSCORE, INTEGER, IDENT, ATTRIBUTE_REF, EXPRESSION_SPEC, INVALID
};

/*
* Represents a query argument within a query clause
*/
class QueryArgument {
public:
	QueryArgument(string errorMessage, ARGUMENT_TYPE argType, ARGUMENT argStr, QueryElement argElem);
	QueryArgument() = default;

	bool hasError();
	string getErrorMessage();
	void setErrorMessage(string errorMessage);

	ARGUMENT_TYPE getArgType();
	void setArgType(ARGUMENT_TYPE argType);

	string getArgStr();
	void setArgStr(string argStr);

	QueryElement getArgElem();
	void setArgElem(QueryElement argElem);

private:
	string errorMessgae = NO_ERROR;
	ARGUMENT_TYPE argType = ARGUMENT_TYPE::INVALID;
	ARGUMENT argStr;
	// Only update for synonym and attribute reference argument types
	QueryElement argElem;
};

inline
bool operator== (QueryArgument lhs, QueryArgument rhs) {
	return lhs.getErrorMessage() == rhs.getErrorMessage() &&
		lhs.getArgType() == rhs.getArgType() &&
		lhs.getArgStr() == rhs.getArgStr() &&
		lhs.getArgElem() == rhs.getArgElem();
}
