#include "OnDemandExtractor.h"

OnDemandExtractor::OnDemandExtractor() {
	affectsPopulated = false;
	affectsStarPopulated = false;
}

void OnDemandExtractor::extractNextStar(INDEX progLine, RelationshipCache& cache) {
	return NextStarExtractor(cache).extractNextStar(progLine);
}

void OnDemandExtractor::extractNextStar(RelationshipCache& cache) {
	return NextStarExtractor(cache).extractNextStar();
}

void OnDemandExtractor::extractAffects(INDEX progline, RelationshipCache& cache) {
	return AffectsExtractor(cache).extractAffects(progline);
}

void OnDemandExtractor::extractAffects(RelationshipCache& cache) {
	return AffectsExtractor(cache).extractAffects();
}

void OnDemandExtractor::extractAffectsStar(RelationshipCache& cache) {
	return AffectsStarExtractor(cache).extractAffectsStar();
}

void OnDemandExtractor::extractsAffectsStar(INDEX prog_line, RelationshipCache& cache) {
	return AffectsStarExtractor(cache).extractAffectsStar(prog_line);
}

void OnDemandExtractor::extractsReverseAffectsStar(RelationshipCache& cache, INDEX prog_line) {
	return AffectsStarExtractor(cache).extractReverseAffectsStar(prog_line);
}

#pragma region Next Star Extractor

NextStarExtractor::NextStarExtractor(RelationshipCache& cache) : nStore(pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::NEXT)), nStarCache(cache) {}

void NextStarExtractor::extractNextStar(INDEX progLine) {
	if (nStarCache.isValuePopulated(progLine)) return;

	extractedProgLines.insert(progLine);

	for (auto nextProgLine : nStore.getSecondValuesOf(progLine)) {
		if (extractedProgLines.count(nextProgLine)) {
			if (nStarCache.isValuePopulated(nextProgLine)) {
				nStarCache.addRelationship(progLine, nextProgLine);
				copyNextStar(nextProgLine, progLine);
			}
			else {
				cycleTracker.addRelationship(progLine, nextProgLine);
			}
			continue;
		}
		nStarCache.addRelationship(progLine, nextProgLine);
		extractNextStar(nextProgLine);
		copyNextStar(nextProgLine, progLine);
	}


	if (cycleTracker.getSecondValuesOf(progLine).empty()) {
		nStarCache.addPopulatedValue(progLine);
		replaceCycle(progLine);
	}

	return;
}

void NextStarExtractor::extractNextStar() {
	if (nStarCache.isPopulated()) return;

	for (auto progLine : nStore.getFirstValues()) {
		extractNextStar(progLine);
	}
}

void NextStarExtractor::copyNextStar(INDEX from, INDEX to) {
	for (auto progLine : nStarCache.getSecondValuesOf(from)) {
		nStarCache.addRelationship(to, progLine);
	}

	for (auto progLine : cycleTracker.getSecondValuesOf(from)) {
		// Cycles to self
		if (progLine == to) {
			nStarCache.addRelationship(progLine, progLine);
		}
		else if (nStarCache.hasRelationship(to, progLine)) {
			continue;
		}
		else {
			cycleTracker.addRelationship(to, progLine);
		}
	}
}

void NextStarExtractor::replaceCycle(INDEX cycleProgLine) {
	INDICES progLinesWithCycles = cycleTracker.getFirstValuesOf(cycleProgLine);
	for (auto progLine : progLinesWithCycles) {
		// Replace the cycle
		copyNextStar(cycleProgLine, progLine);

		// Remove cycle
		cycleTracker.removeRelationship(progLine, cycleProgLine);

		// Remove implicitly resolved cycles
		INDICES otherProgLinesWithCycles = cycleTracker.getSecondValuesOf(progLine);
		for (auto otherProgLine : otherProgLinesWithCycles) {
			if (nStarCache.hasRelationship(progLine, otherProgLine)) {
				cycleTracker.removeRelationship(progLine, otherProgLine);
			}
		}

		// Program line is completely extracted if no cycles exists
		if (cycleTracker.getSecondValuesOf(progLine).empty()) {
			nStarCache.addPopulatedValue(progLine);
			replaceCycle(progLine);
		}
	}
}

#pragma endregion

#pragma region Affects Extractor

AffectsExtractor::AffectsExtractor(RelationshipCache& cache) : aCache(cache), root(pkb.getASTRoot()) {}

void AffectsExtractor::extractAffects() {
	for (TNode proc : root.getChildren()) {
		unordered_map<VALUE, unordered_set<int>> history;
		traverseAffects(proc.getChildren()[0], &history);
	}
}

//get all the variables modified by a node
VALUES AffectsExtractor::getModifies(TNode* node) {
	VALUE num = node->getStmtNum();
	VALUES modifies = pkb.getSecondValuesOf(RELATIONSHIP_TYPE::MODIFIES_STMT, DESIGN_ENTITY::VARIABLE, to_string(num));
	return modifies;
}
//get all the variables used by a node
VALUES AffectsExtractor::getUses(TNode* node) {
	VALUE num = node->getStmtNum();
	VALUES uses = pkb.getSecondValuesOf(RELATIONSHIP_TYPE::USES_STMT, DESIGN_ENTITY::VARIABLE, to_string(num));
	return uses;
}

//populate everything from the start of the proc to the end of the proc
void AffectsExtractor::traverseAffects(TNode& node, unordered_map<VALUE, unordered_set<int>>* modifiedHistory) {
	for (TNode s : node.getChildren()) {

		//read statement
		if (s.getType() == TNode::NODE_TYPE::readStmt) {
			for (VALUE v : getModifies(&s)) {
				modifiedHistory->erase(v);
			}
		}

		//call statement
		else if (s.getType() == TNode::NODE_TYPE::callStmt) {
			VALUES modifiedVars = getModifies(&s);

			for (VALUE v : modifiedVars) {
				modifiedHistory->erase(v);
			}
		}

		//assignment
		else if (s.getType() == TNode::NODE_TYPE::assignStmt) {
			VALUES modifiedVars = getModifies(&s);
			VALUES usedVars = getUses(&s);
			string nodeStmt = to_string(s.getStmtNum());
			for (VALUE usedVar : usedVars) {
				if (modifiedHistory->find(usedVar) != modifiedHistory->end()) {
					unordered_set<int> assignStmts = modifiedHistory->at(usedVar);
					for (int stmt : assignStmts) {
						INDEX from = pkb.getIndexOfDEValue(to_string(stmt));
						INDEX to = pkb.getIndexOfDEValue(nodeStmt);
						aCache.addRelationship(from, to);
					}
				}
			}
			for (VALUE v : modifiedVars) {
				modifiedHistory->erase(v);
				modifiedHistory->insert({ v, {s.getStmtNum()} });
			}
		}

		//if case
		else if (s.getType() == TNode::NODE_TYPE::ifStmt) {
			unordered_map<VALUE, unordered_set<int>> seperateHistory = *modifiedHistory;
			TNode leftChild = s.getChildren()[0];
			TNode rightChild = s.getChildren()[1];
			traverseAffects(leftChild, modifiedHistory);
			traverseAffects(rightChild, &seperateHistory);

			for (auto const& pair : seperateHistory) {
				if (modifiedHistory->find(pair.first) != modifiedHistory->end()) {
					for (int v : pair.second) {
						modifiedHistory->at(pair.first).insert(v);
					}
				}
				else {
					modifiedHistory->insert({ pair.first, pair.second });
				}
			}
		}

		//while case
		else if (s.getType() == TNode::NODE_TYPE::whileStmt) {
			unordered_map<VALUE, unordered_set<int>> previous;
			while (1) {
				unordered_map<VALUE, unordered_set<int>> copy = *modifiedHistory;
				traverseAffects(s.getChildren()[0], &copy);
				if (previous == copy) {
					break;
				}
				for (auto const& pair : copy) {
					if (modifiedHistory->find(pair.first) != modifiedHistory->end()) {
						for (int v : pair.second) {
							modifiedHistory->at(pair.first).insert(v);
						}
					}
					else {
						modifiedHistory->insert({ pair.first, pair.second });
					}
				}
				previous = copy;
			}

		}
	}
}

//find all affects for a given prog line
void AffectsExtractor::extractAffects(INDEX start) {
	if (aCache.isValuePopulated(start)) {
		return;
	}
	aCache.addPopulatedValue(start);
	traverseAffects(start);
}

bool AffectsExtractor::isAssignment(INDEX i) {
	INDICES& assignStatementsIndex = pkb.getIndexedStmtsOf(DESIGN_ENTITY::ASSIGN);
	return assignStatementsIndex.find(i) != assignStatementsIndex.end();
}

bool AffectsExtractor::isRead(INDEX i) {
	INDICES& assignStatementsIndex = pkb.getIndexedStmtsOf(DESIGN_ENTITY::READ);
	return assignStatementsIndex.find(i) != assignStatementsIndex.end();
}

bool AffectsExtractor::isCall(INDEX i) {
	INDICES& assignStatementsIndex = pkb.getIndexedStmtsOf(DESIGN_ENTITY::CALL);
	return assignStatementsIndex.find(i) != assignStatementsIndex.end();
}

void AffectsExtractor::traverseAffects(INDEX s) {
	unordered_set<INDEX> visited;
	stack<INDEX> frontier;

	INDEX start = s;
	SYNONYM_INDICES modifiedVars = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::MODIFIES_STMT).getSecondValuesOf(start);
	INDEX modifiedVar = -2;

	for (auto const v : modifiedVars) {
		modifiedVar = v;
	}

	frontier.push(start);

	while (!frontier.empty()) {
		INDEX cur = frontier.top();
		frontier.pop();
		visited.insert(cur);

		if (isAssignment(cur) && (cur != start) && isAssignment(start)) {
			SYNONYM_INDICES usedVars = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::USES_STMT).getSecondValuesOf(cur);
			if (usedVars.find(modifiedVar) != usedVars.end()) {
				VAR from = pkb.getStringValueOfDE(s);
				VAR to = pkb.getStringValueOfDE(cur);
				pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS, from, to);
			}
		}

		SYNONYM_INDICES mVars = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::MODIFIES_STMT).getSecondValuesOf(cur);
		bool flag = true;

		if ((isAssignment(cur) || isRead(cur) || isCall(cur)) && (cur != start)) {
			for (auto const v : mVars) {
				if (v == modifiedVar) {
					flag = false;
				}
			}
		}

		if (flag) {
			SYNONYM_INDICES neighbours = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::NEXT).getSecondValuesOf(cur);
			for (auto const n : neighbours) {
				if (visited.find(n) == visited.end()) {
					frontier.push(n);
				}
				else if (isAssignment(n) && isAssignment(start)) {
					SYNONYM_INDICES usedVars = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::USES_STMT).getSecondValuesOf(n);
					if (usedVars.find(modifiedVar) != usedVars.end()) {
						VAR from = pkb.getStringValueOfDE(s);
						VAR to = pkb.getStringValueOfDE(n);
						pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS, from, to);
					}
				}
			}
		}
	}
}

#pragma endregion

#pragma region Affects Star Extractor

AffectsStarExtractor::AffectsStarExtractor(RelationshipCache& cache) : asCache(cache) {}

void AffectsStarExtractor::extractAffectsStar() {
	traverseAffectsStar();
}

void AffectsStarExtractor::extractAffectsStar(INDEX prog_line) {
	if (!asCache.isValuePopulated(prog_line)) {
		traverseAffectsStar(prog_line);
		asCache.addPopulatedValue(prog_line);
	}
}

void AffectsStarExtractor::extractReverseAffectsStar(INDEX prog_line) {
	traverseReverseAffectsStar(prog_line);
}

// populate the entire affects table
// transitive closure on affects table to populate affects table
void AffectsStarExtractor::traverseAffectsStar() {
	INDICES assignStatements = pkb.getIndexedStmtsOf(DESIGN_ENTITY::ASSIGN);
	for (INDEX prog_line : assignStatements) {
		extractAffectsStar(prog_line);
	}
}

// populate the entire affects table
// populate the entire affects star table
void AffectsStarExtractor::traverseAffectsStar(INDEX prog_line) {
	unordered_set<VALUE> visited;

	stack<VALUE> stack = {};
	stack.push(prog_line);
	VALUE cur;

	while (!stack.empty()) {
		cur = stack.top();
		stack.pop();
		if (visited.find(cur) == visited.end()) {
			visited.insert(cur);
			VALUES neighbour = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::AFFECTS).getSecondValuesOf(cur);
			for (VALUE sn : neighbour) {
				pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_STAR, pkb.getStringValueOfDE(prog_line), pkb.getStringValueOfDE(sn));
				stack.push(sn);
			}
		}
	}
}


// populate a single column
void AffectsStarExtractor::traverseReverseAffectsStar(INDEX prog_line) {
	unordered_set<VALUE> visited;

	stack<VALUE> stack = {};
	stack.push(prog_line);
	VALUE cur;

	while (!stack.empty()) {
		cur = stack.top();
		stack.pop();
		if (visited.find(cur) == visited.end()) {
			visited.insert(cur);
			VALUES neighbour = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::AFFECTS).getFirstValuesOf(cur);
			for (VALUE sn : neighbour) {
				pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_STAR, pkb.getStringValueOfDE(sn), pkb.getStringValueOfDE(prog_line));
				stack.push(sn);
			}
		}
	}
}

#pragma endregion
