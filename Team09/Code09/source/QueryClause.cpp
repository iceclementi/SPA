#include "QueryClause.h"

QueryClause::QueryClause(SYNONYM_INDICES csi) {
	clauseSynonyms = csi;
}

SYNONYM_INDICES& QueryClause::getClauseSynonyms() {
	return clauseSynonyms;
}

void QueryClause::addClauseSynonym(SYNONYM_INDEX synIndex) {
	clauseSynonyms.insert(synIndex);
}
