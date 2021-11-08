#include "QueryOptimiser.h"

QueryOptimiser::QueryOptimiser(QueryObject& queryObject) : queryObject(queryObject) {
	clauseIndexToClause = queryObject.getIndexedQueryClauses();
	selectSynonyms = queryObject.getSelectResult().getSelectSynonyms();
}

/*
* Entry method of Optimisation
* START HERE
*/
QueryObject& QueryOptimiser::optimiseQuery() {
	
	createConstantQueryGroup();
	
	createSelectQueryGroups();
	
	createNonSelectQueryGroups();

	return queryObject;
}

/*
* Bypass optimisation of QueryObject.
*/
QueryObject& QueryOptimiser::noOptimiseQuery() {
	CLAUSE_INDICES clauseIndices;
	SYNONYM_COUNTER synonymCounter;
	SYNONYM_INDICES groupSelectSynonyms;
	for (auto& clause : clauseIndexToClause) {
		clauseIndices.push_back(clause.first);
		for (auto& synonym : clause.second.getClauseSynonyms()) {
			synonymCounter[synonym] += 2;
			if (selectSynonyms.count(synonym)) {
				groupSelectSynonyms.insert(synonym);
			}
		}
	}
	SelectQueryGroup queryGroup(clauseIndices, synonymCounter, groupSelectSynonyms);
	queryObject.addSelectQueryGroup(queryGroup);

	return queryObject;
}

int QueryOptimiser::getPriorityOfNoSynonymClause(CLAUSE_INDEX clauseIndex) {
	switch (queryObject.getClauseTypeOf(clauseIndex)) {
	case CLAUSE_TYPE::SUCH_THAT:
		return OptimizerUtility::getPriorityOfNoSynonymSuchThatClause(queryObject.getSuchThatClauseOf(clauseIndex));
	case CLAUSE_TYPE::PATTERN:
		return OptimizerUtility::getPriorityOfNoSynonymPatternClause(queryObject.getPatternClauseOf(clauseIndex));
	case CLAUSE_TYPE::WITH:
		return OptimizerUtility::getPriorityOfNoSynonymWithClause(queryObject.getWithClauseOf(clauseIndex));
	default:
		return 3203;
	}
}

int QueryOptimiser::getPriorityOfhasSynonymClause(CLAUSE_INDEX clauseIndex, int numOfSynonyms) {
	switch (queryObject.getClauseTypeOf(clauseIndex)) {
	case CLAUSE_TYPE::SUCH_THAT:
		return OptimizerUtility::getPriorityOfhasSynonymSuchThatClause(queryObject.getSuchThatClauseOf(clauseIndex), numOfSynonyms);
	case CLAUSE_TYPE::PATTERN:
		return OptimizerUtility::getPriorityOfhasSynonymPatternClause(queryObject.getPatternClauseOf(clauseIndex), numOfSynonyms);
	case CLAUSE_TYPE::WITH:
		return OptimizerUtility::getPriorityOfhasSynonymWithClause(queryObject.getWithClauseOf(clauseIndex), numOfSynonyms);
	default:
		return 3203;
	}
}


/*
* Order clauses for each for the synonym in the unorderedMap synonymToClauses.

* (1) Ensure adjacent clauses have overlapping synonyms
* (2) Implement priority order based on relationship type and number of synonyms in clause.
* Try to implement (2) while enforcing (1)
* In synonymToClauses, the clauses of each synonym will be sorted according to (2)
* Select a1 such that Next*(e,d) and Follows(a,b) and Follows*(a,c) and Parent(d,a)
* Before
* synonymToClauses
* e: 1
* d: 1 4
* a: 2 3 4
* b: 2
* c: 3
* 
* After
* synonymToClauses
* e: 1
* d: 4 1 (optimised)
* a: 2 4 3 (optimised)
* b: 2
* c: 3
*/
void QueryOptimiser::orderClausesOfSynonymToClausesMap() {
	//Sort clause order for each synonym.
	for (auto& synToClausesPair : synonymToClauses) {
		unordered_set<CLAUSE_INDEX> sortedSet;
		WEIGHTED_CLAUSE_INDICES weightedClauseIndices;

		for (auto& clauseIndex : synToClausesPair.second) {
			QueryClause clause = clauseIndexToClause[clauseIndex];
			int numOfSynonyms = clause.getClauseSynonyms().size();
			int priorityValue = getPriorityOfhasSynonymClause(clauseIndex, numOfSynonyms);
			WEIGHTED_CLAUSE_INDICE weightedClauseIndex = make_pair(priorityValue, clauseIndex);
			weightedClauseIndices.push_back(weightedClauseIndex);
		}

		//Sort clauses order based on relationship type 
		sort(weightedClauseIndices.begin(), weightedClauseIndices.end());

		for (auto& weightedClauseIndex : weightedClauseIndices) {
			sortedSet.insert(weightedClauseIndex.second);
		}
		synonymToClauses[synToClausesPair.first] = sortedSet;
	}
}

/*
* Create a query group containing clauses with no synonyms
*/
void QueryOptimiser::createConstantQueryGroup() {
	CLAUSE_INDICES clauseIndices;
	WEIGHTED_CLAUSE_INDICES weightedClauseIndices;

	// Find clauses with no synonyms, otherwise map the synonyms to the clause
	for (auto& indexedClause : clauseIndexToClause) {
		SYNONYM_INDICES clauseSynoynms = indexedClause.second.getClauseSynonyms();
		if (clauseSynoynms.empty()) {
			int priorityValue = getPriorityOfNoSynonymClause(indexedClause.first);
			WEIGHTED_CLAUSE_INDICE weightedClauseIndex = make_pair(priorityValue, indexedClause.first);
			weightedClauseIndices.push_back(weightedClauseIndex);
		}
		else {
			for (auto& synonym : clauseSynoynms) {
				synonymToClauses[synonym].insert(indexedClause.first);
			}
		}
	}
	//Sort clauses order based on relationship type 
	sort(weightedClauseIndices.begin(), weightedClauseIndices.end());
	for (auto& weightedClauseIndex : weightedClauseIndices) {
		clauseIndices.push_back(weightedClauseIndex.second);
	}
	orderClausesOfSynonymToClausesMap();

	if (clauseIndices.empty()) {
		return;
	}

	for (auto& index : clauseIndices) {
		clauseIndexToClause.erase(index);
	}

	queryObject.addConstantQueryGroup(ConstantQueryGroup(clauseIndices));
}

void QueryOptimiser::createSelectQueryGroups() {
	// Iterate until no more select synonyms can be found in the clauses
	bool selectSynonymExists = true;
	while (selectSynonymExists) {
		selectSynonymExists = false;
		// Check if any of the clause contains the select synonym
		for (auto& startSynonym : selectSynonyms) {
			if (synonymToClauses.count(startSynonym)) {
				createConnectedQueryGroup(startSynonym, true);
				selectSynonymExists = true;
			}
		}
	}
}

void QueryOptimiser::createNonSelectQueryGroups() {
	while (!synonymToClauses.empty()) {
		SYNONYM_INDEX startSynonym = (*synonymToClauses.begin()).first; // Take any synonym, in this case the first
		createConnectedQueryGroup(startSynonym, false);
	}
}

void QueryOptimiser::createConnectedQueryGroup(SYNONYM_INDEX startSynonym, bool isSelectQueryGroup) {
	CLAUSE_INDICES clauseIndices;
	queue<SYNONYM_INDEX> queue({ startSynonym });
	SYNONYM_INDICES connectedSynonyms;
	SYNONYM_COUNTER synonymCounter;

	// Find a group of clauses connected with common synonyms
	while (!queue.empty()) {
		SYNONYM_INDEX synonym = queue.front();
		queue.pop();

		// Check if synonym has been processed already
		if (connectedSynonyms.count(synonym)) {
			continue;
		}
		else {
			connectedSynonyms.insert(synonym);
		}

		// Iterate through each clause containing the synonym
		for (auto& clauseIndex : synonymToClauses[synonym]) {
			clauseIndices.push_back(clauseIndex);
			for (auto& connectedSynonym : clauseIndexToClause[clauseIndex].getClauseSynonyms()) {
				++synonymCounter[connectedSynonym]; // Increment synonym count

				// Connected synonym is different from current synonym and not processed yet
				if (connectedSynonym != synonym && !connectedSynonyms.count(connectedSynonym)) {
					queue.emplace(connectedSynonym);
					synonymToClauses[connectedSynonym].erase(clauseIndex); // Remove clause duplicates
				}
			}
		}
		cout << endl;
		synonymToClauses.erase(synonym); // Remove all clauses with the synonym
	}

	if (isSelectQueryGroup) {
		// Get connected select synonym(s)
		SYNONYM_INDICES groupSelectSynonyms;
		for (auto& selectSynoynm : selectSynonyms) {
			if (connectedSynonyms.count(selectSynoynm)) {
				groupSelectSynonyms.insert(selectSynoynm);
			}
		}
		SelectQueryGroup queryGroup(clauseIndices, synonymCounter, groupSelectSynonyms);
		queryObject.addSelectQueryGroup(queryGroup);
		// Delete connected select synonym(s)
		selectSynonyms.erase(groupSelectSynonyms.begin(), groupSelectSynonyms.end());
	}
	else {
		queryObject.addNonSelectQueryGroup(NonSelectQueryGroup(clauseIndices, synonymCounter));
	}
}
