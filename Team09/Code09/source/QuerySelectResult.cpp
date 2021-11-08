#include "QuerySelectResult.h"

QuerySelectResult::QuerySelectResult(bool isb, SYNONYM_INDICES& ss, ELEMENT_LIST& sel) {
	isSelectBoolean = isb;
	selectSynonyms = ss;
	selectElementList = sel;
}

bool QuerySelectResult::getSelectBoolean() {
	return isSelectBoolean;
}

void QuerySelectResult::setSelectBoolean(bool isb) {
	isSelectBoolean = isb;
}

SYNONYM_INDICES& QuerySelectResult::getSelectSynonyms() {
	return selectSynonyms;
}

void QuerySelectResult::addSelectSynonym(SYNONYM_INDEX synIndex) {
	selectSynonyms.insert(synIndex);
}

ELEMENT_LIST& QuerySelectResult::getSelectElementList() {
	return selectElementList;
}

void QuerySelectResult::addToSelectElementList(QueryElement element) {
	selectElementList.push_back(element);
}
