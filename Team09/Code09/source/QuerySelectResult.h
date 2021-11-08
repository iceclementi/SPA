#pragma once

#include "Query.h"
#include "QueryElement.h"

using namespace std;

typedef vector<QueryElement> ELEMENT_LIST;

/*
* Represents a result clause of a query
*/
class QuerySelectResult {
public:
	QuerySelectResult(bool isSelectBoolean, SYNONYM_INDICES& selectSynonyms, ELEMENT_LIST& selectElementList);
	QuerySelectResult() = default;

	bool getSelectBoolean();
	void setSelectBoolean(bool isSelectBoolean);

	SYNONYM_INDICES& getSelectSynonyms();
	void addSelectSynonym(SYNONYM_INDEX synIndex);
	
	ELEMENT_LIST& getSelectElementList();
	void addToSelectElementList(QueryElement element);

private:
	bool isSelectBoolean = false;
	SYNONYM_INDICES selectSynonyms;
	ELEMENT_LIST selectElementList;
};

inline
bool operator== (QuerySelectResult lhs, QuerySelectResult rhs) {
	return lhs.getSelectBoolean() == rhs.getSelectBoolean() &&
		lhs.getSelectSynonyms() == rhs.getSelectSynonyms() &&
		lhs.getSelectElementList() == rhs.getSelectElementList();
}
