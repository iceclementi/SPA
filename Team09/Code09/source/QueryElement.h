#pragma once

#include "Query.h"

using namespace std;

/*
* Describes a synonym or an attribute reference consisting of a synonym and its attribute type
* (for selecting as a result or for an argument in the query clause)
*/
class QueryElement {
public:
	QueryElement(string errorMessage, SYNONYM_INDEX synIndex, DESIGN_ENTITY synType, ATTRIBUTE_TYPE attrType);
	QueryElement() = default;

	bool hasError();
	string getErrorMessage();
	void setErrorMessage(string errorMessage);

	SYNONYM_INDEX getSynIndex();
	void setSynIndex(SYNONYM_INDEX synIndex);

	DESIGN_ENTITY getSynType();
	void setSynType(DESIGN_ENTITY synType);

	ATTRIBUTE_TYPE getAttrType();
	void setAttrType(ATTRIBUTE_TYPE attrType);

private:
	string errorMessage = NO_ERROR;
	SYNONYM_INDEX synIndex = NO_SYNONYM_INDEX;
	DESIGN_ENTITY synType = DESIGN_ENTITY::NONE;
	// Only update attrType for attribute reference
	ATTRIBUTE_TYPE attrType = ATTRIBUTE_TYPE::NONE;
};

inline
bool operator== (QueryElement lhs, QueryElement rhs) {
	return lhs.getErrorMessage() == rhs.getErrorMessage() &&
		lhs.getSynIndex() == rhs.getSynIndex() &&
		lhs.getSynType() == rhs.getSynType() &&
		lhs.getAttrType() == rhs.getAttrType();
}
