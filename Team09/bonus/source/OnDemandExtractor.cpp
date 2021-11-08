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

void OnDemandExtractor::extractNextBip(RelationshipBipCache& cache) {
	NextBipExtractor(cache).extractNextBip();
}

void OnDemandExtractor::extractNextBipStar(RelationshipBipCache& cache) {
	pkb.populateNextBipCache();
	NextBipStarExtractor(cache).extractNextBipStar();
}

void OnDemandExtractor::extractAffectsBip() {
	AffectsBipExtractor().extractAffectsBip();
}
void OnDemandExtractor::extractAffectsBipStar() {
	AffectsBipExtractor().extractAffectsBip();
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

#pragma region Extensions

#pragma region Next Bip Extractor

NextBipExtractor::NextBipExtractor(RelationshipBipCache& cache) : nStore(pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::NEXT)), nBipCache(cache), stmtStore(pkb.getStmtStore()) {
	initProcedures();
}

void NextBipExtractor::extractNextBip() {
	INDEX_LIST procedures = generateOrderedProcedures();

	// Extract Next Bip relationships by procedures
	for (auto& procedure : procedures) {
		INDICES calledProcedures;
		extractNextBip(procedure, nBipCache.getFirstLineOf(procedure), calledProcedures);

		// Copy program lines in called procedures
		for (auto& calledProcedure : calledProcedures) {
			for (auto& progLine : nBipCache.getLinesOf(calledProcedure)) {
				nBipCache.addLineToProcedure(procedure, progLine);
			}
		}
	}
}

void NextBipExtractor::extractNextBip(INDEX procedure, INDEX progLine, INDICES& calledProcedures) {
	if (nBipCache.isValuePopulated(progLine)) return;
	nBipCache.addLineToProcedure(procedure, progLine);
	nBipCache.addPopulatedValue(progLine);
	
	if (pkb.getIndexedStmtsOf(DESIGN_ENTITY::CALL).count(progLine)) {
		// Current line is a call statement
		INDEX calledProcedure = stmtStore.getIndexedVarOf(DESIGN_ENTITY::CALL, progLine);
		// Connect to first line of called procedure
		nBipCache.addRelationship(progLine, nBipCache.getFirstLineOf(calledProcedure));
		calledProcedures.insert(calledProcedure);
		
		// Connect from last line(s) of called procedure
		for (auto& nextLine : nStore.getSecondValuesOf(progLine)) {
			for (auto& lastLine : nBipCache.getLastLinesOf(calledProcedure)) {
				nBipCache.addRelationship(lastLine, nextLine);
			}
			// Recurse to next line
			extractNextBip(procedure, nextLine, calledProcedures);
		}
	}
	else {
		// Normal next
		for (auto& nextLine : nStore.getSecondValuesOf(progLine)) {
			nBipCache.addRelationship(progLine, nextLine);
			// Recurse to next line
			extractNextBip(procedure, nextLine, calledProcedures);
		}
	}
}

void NextBipExtractor::initProcedures() {
	// Find the first line for all the procedures
	NODE_LIST& procNodes = pkb.getASTRoot().getChildren();
	for (auto& procNode : procNodes) {
		TNode firstLineNode = procNode.getChildren().front().getChildren().front();
		INDEX procedure = pkb.getIndexOfDEValue(procNode.getValue());
		INDEX firstLine = pkb.getIndexOfDEValue(to_string(firstLineNode.getStmtNum()));
		nBipCache.addFirstLineOfProcedure(procedure, firstLine);
	}

	INDICES& whileStmts = pkb.getIndexedStmtsOf(DESIGN_ENTITY::WHILE);

	// Find the last line(s) for all the procedures
	for (auto& nextLine : pkb.getIndexedDEValuesOf(DESIGN_ENTITY::PROG_LINE)) {
		if (!nStore.hasRelationshipWithFirstValue(nextLine)) {
			// Don't have next line, i.e. last line
			nBipCache.addLastLineOfProcedure(getProcedureContainingLine(nextLine), nextLine);
		}
		else if (whileStmts.count(nextLine) && nStore.getSecondValuesOf(nextLine).size() < 2) {
			// While statement is the last line
			nBipCache.addLastLineOfProcedure(getProcedureContainingLine(nextLine), nextLine);
		}
	}

	// Replace all last line(s) for procedures ending with call statement
	for (auto& procedure : generateOrderedProcedures()) {
		replaceLastLinesOf(procedure);
	}
}

INDEX_LIST NextBipExtractor::generateOrderedProcedures() {
	auto& cStarStore = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::CALLS_STAR);
	INDICES procedures = pkb.getIndexedDEValuesOf(DESIGN_ENTITY::PROCEDURE);
	INDEX_LIST orderedProcedures;
	INDICES addedProcedures;

	// Sort by call dependency --> from procedure that doesn't call to procedure that is not called
	// Find procedures that does not call other procedures first
	for (auto& procedure : procedures) {
		if (!cStarStore.hasRelationshipWithFirstValue(procedure)) {
			orderedProcedures.push_back(procedure);
			addedProcedures.insert(procedure);
		}
	}
	for (auto& addedProcedure : addedProcedures) {
		procedures.erase(addedProcedure);
	}
	addedProcedures = INDICES();

	// Order remaining procedures in increasing dependency
	while (!procedures.empty()) {
		for (auto& procedure : procedures) {
			bool hasLowDependency = true;
			for (auto& calledProcedure : cStarStore.getSecondValuesOf(procedure)) {
				if (find(orderedProcedures.begin(), orderedProcedures.end(), calledProcedure) == orderedProcedures.end()) {
					hasLowDependency = false;
					break;
				}
			}
			if (hasLowDependency) {
				orderedProcedures.push_back(procedure);
				addedProcedures.insert(procedure);
			}
		}
		for (auto& addedProcedure : addedProcedures) {
			procedures.erase(addedProcedure);
		}
		addedProcedures = INDICES();
	}
	
	return orderedProcedures;
}

INDEX NextBipExtractor::getProcedureContainingLine(INDEX line) {
	INDICES procedures = pkb.getIndexedDEValuesOf(DESIGN_ENTITY::PROCEDURE);
	int lineValue = stoi(pkb.getStringValueOfDE(line));
	int difference = INT_MAX;
	INDEX closestProcedure = 0;
	for (auto& procedure : procedures) {
		INDEX firstLine = nBipCache.getFirstLineOf(procedure);

		//Line is first statement of procedure
		if (line == firstLine) return procedure;
		int currentDifference = lineValue - stoi(pkb.getStringValueOfDE(firstLine));
		if (currentDifference > 0 && currentDifference < difference) {
			closestProcedure = procedure;
			difference = currentDifference;
		}
	}
	return closestProcedure;
}

void NextBipExtractor::replaceLastLinesOf(INDEX procedure) {
	INDICES callStmts = pkb.getIndexedStmtsOf(DESIGN_ENTITY::CALL);
	INDICES lastLines = nBipCache.getLastLinesOf(procedure);
	for (auto& lastLine : lastLines) {
		// Last line is a call statement --> Extend to last line of called procedure
		if (callStmts.count(lastLine)) {
			INDEX calledProcedure = stmtStore.getIndexedVarOf(DESIGN_ENTITY::CALL, lastLine);
			for (auto& newLastLine : nBipCache.getLastLinesOf(calledProcedure)) {
				nBipCache.replaceLastLineOfProcedure(procedure, lastLine, newLastLine);
			}
		}
	}
}

#pragma endregion

#pragma region Next Bip Star Extractor

NextBipStarExtractor::NextBipStarExtractor(RelationshipBipCache& cache) 
	: nStore(pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::NEXT)), nBipStarCache(cache), stmtStore(pkb.getStmtStore()), startLine(0) {}

void NextBipStarExtractor::extractNextBipStar() {
	if (nBipStarCache.isPopulated()) return;

	for (auto& procedure : generateOrderedProcedures()) {
		extractNextBipStar(procedure);
	}
}

void NextBipStarExtractor::extractNextBipStar(INDEX procedure) {
	extractNextBipStar(procedure, nBipStarCache.getFirstLineOf(procedure));
}

void NextBipStarExtractor::extractNextBipStar(INDEX procedure, INDEX progLine) {
	startLine = progLine;
	recurseExtractNextBipStar(progLine);
}

void NextBipStarExtractor::recurseExtractNextBipStar(INDEX progLine) {
	if (nBipStarCache.isValuePopulated(progLine)) return;

	extractedProgLines.insert(progLine);
	
	if (pkb.getIndexedDEValuesOf(DESIGN_ENTITY::CALL).count(progLine)) {
		// Current program line is a call statement --> Join to called procedure
		INDEX calledProcedure = stmtStore.getIndexedVarOf(DESIGN_ENTITY::CALL, progLine);
		nBipStarCache.addRelationship(progLine, nBipStarCache.getFirstLineOf(calledProcedure));
		for (auto nextProgLine : nStore.getSecondValuesOf(progLine)) {
			if (extractedProgLines.count(nextProgLine)) {
				if (nBipStarCache.isValuePopulated(nextProgLine)) {
					nBipStarCache.addRelationship(progLine, nextProgLine);
					copyLineToProcedure(nextProgLine, calledProcedure);
					copyProcedureToLine(calledProcedure, progLine);
					copyNextBipStar(nextProgLine, progLine);
				}
				else {
					cycleTracker.addRelationship(progLine, nextProgLine);
				}
				continue;
			}

			nBipStarCache.addRelationship(progLine, nextProgLine);
			for (auto& lastLine : nBipStarCache.getLastLinesOf(calledProcedure)) {
				nBipStarCache.addRelationship(lastLine, nextProgLine);
				recurseExtractNextBipStar(nextProgLine);

				// Copy values to both called procedure and call statement
				copyLineToProcedure(nextProgLine, calledProcedure);
				copyProcedureToLine(calledProcedure, progLine);
				copyNextBipStar(nextProgLine, progLine);
			}
		}

		// Call statement is last line of procedure
		if (nStore.getSecondValuesOf(progLine).empty()) {
			copyProcedureToLine(calledProcedure, progLine);
		}
	}
	else {
		for (auto nextProgLine : nStore.getSecondValuesOf(progLine)) {
			if (extractedProgLines.count(nextProgLine)) {
				if (nBipStarCache.isValuePopulated(nextProgLine)) {
					nBipStarCache.addRelationship(progLine, nextProgLine);
					copyNextBipStar(nextProgLine, progLine);
				}
				else {
					cycleTracker.addRelationship(progLine, nextProgLine);
				}
				continue;
			}
			nBipStarCache.addRelationship(progLine, nextProgLine);
			recurseExtractNextBipStar(nextProgLine);
			copyNextBipStar(nextProgLine, progLine);
		}
	}

	if (cycleTracker.getSecondValuesOf(progLine).empty()) {
		nBipStarCache.addPopulatedValue(progLine);
		replaceCycle(progLine);
	}

	return;
}

void NextBipStarExtractor::copyNextBipStar(INDEX from, INDEX to) {
	for (auto progLine : nBipStarCache.getSecondValuesOf(from)) {
		nBipStarCache.addRelationship(to, progLine);
	}

	for (auto progLine : cycleTracker.getSecondValuesOf(from)) {
		// Cycles to self
		if (progLine == to) {
			nBipStarCache.addRelationship(progLine, progLine);
		}
		else if (nBipStarCache.hasRelationship(to, progLine)) {
			continue;
		}
		else {
			cycleTracker.addRelationship(to, progLine);
		}
	}
}

void NextBipStarExtractor::copyProcedureToLine(INDEX procedure, INDEX progLine) {
	for (auto& nextLine : nBipStarCache.getLinesOf(procedure)) {
		nBipStarCache.addRelationship(progLine, nextLine);
	}
}

void NextBipStarExtractor::copyLineToProcedure(INDEX progLine, INDEX procedure) {
	for (auto& previousLine : nBipStarCache.getLinesOf(procedure)) {
		nBipStarCache.addRelationship(previousLine, progLine);
	}
	for (auto nextLine : nBipStarCache.getSecondValuesOf(progLine)) {
		for (auto& previousLine : nBipStarCache.getLinesOf(procedure)) {
			nBipStarCache.addRelationship(previousLine, nextLine);
		}
	}
}

void NextBipStarExtractor::replaceCycle(INDEX cycleProgLine) {
	INDICES progLinesWithCycles = cycleTracker.getFirstValuesOf(cycleProgLine);
	for (auto progLine : progLinesWithCycles) {
		// Replace the cycle
		copyNextBipStar(cycleProgLine, progLine);

		if (pkb.getIndexedDEValuesOf(DESIGN_ENTITY::CALL).count(progLine)) {
			// Current program line is a call statement --> Join to called procedure
			INDEX calledProcedure = stmtStore.getIndexedVarOf(DESIGN_ENTITY::CALL, progLine);
			copyLineToProcedure(cycleProgLine, calledProcedure);
		}

		// Remove cycle
		cycleTracker.removeRelationship(progLine, cycleProgLine);

		// Remove implicitly resolved cycles
		INDICES otherProgLinesWithCycles = cycleTracker.getSecondValuesOf(progLine);
		for (auto otherProgLine : otherProgLinesWithCycles) {
			if (nBipStarCache.hasRelationship(progLine, otherProgLine)) {
				cycleTracker.removeRelationship(progLine, otherProgLine);
			}
		}

		// Program line is completely extracted if no cycles exists
		if (cycleTracker.getSecondValuesOf(progLine).empty()) {
			nBipStarCache.addPopulatedValue(progLine);
			replaceCycle(progLine);
		}
	}
}

INDEX_LIST NextBipStarExtractor::generateOrderedProcedures() {
	auto& cStarStore = pkb.getRelationshipStoreOf(RELATIONSHIP_TYPE::CALLS_STAR);
	INDICES procedures = pkb.getIndexedDEValuesOf(DESIGN_ENTITY::PROCEDURE);
	INDEX_LIST orderedProcedures;
	INDICES addedProcedures;

	// Sort by call dependency --> from procedure that doesn't call to procedure that is not called
	// Find procedures that does not call other procedures first
	for (auto& procedure : procedures) {
		if (!cStarStore.hasRelationshipWithFirstValue(procedure)) {
			orderedProcedures.push_back(procedure);
			addedProcedures.insert(procedure);
		}
	}
	for (auto& addedProcedure : addedProcedures) {
		procedures.erase(addedProcedure);
	}
	addedProcedures = INDICES();

	// Order remaining procedures in increasing dependency
	while (!procedures.empty()) {
		for (auto& procedure : procedures) {
			bool hasLowDependency = true;
			for (auto& calledProcedure : cStarStore.getSecondValuesOf(procedure)) {
				if (find(orderedProcedures.begin(), orderedProcedures.end(), calledProcedure) == orderedProcedures.end()) {
					hasLowDependency = false;
					break;
				}
			}
			if (hasLowDependency) {
				orderedProcedures.push_back(procedure);
				addedProcedures.insert(procedure);
			}
		}
		for (auto& addedProcedure : addedProcedures) {
			procedures.erase(addedProcedure);
		}
		addedProcedures = INDICES();
	}

	return orderedProcedures;
}

#pragma endregion

#pragma region Affects Bip Extractor

AffectsBipExtractor::AffectsBipExtractor() {
	branchCount = 0;
};

NODE_LIST AffectsBipExtractor::getProcs() {
	TNode root = pkb.getASTRoot();
	NODE_LIST procNodes = root.getChildren();
	NODE_LIST procToTraverse;
	for (TNode& procNode : procNodes) {
		VAR procName = procNode.getValue();
		procMap.insert({ procName, procNode });
		VALUES procsCalling = pkb.getFirstValuesOf(RELATIONSHIP_TYPE::CALLS_STAR, DESIGN_ENTITY::PROCEDURE, procName);
		if (procsCalling.size() == 0) {
			procToTraverse.push_back(procNode);
		}
	}
	return procToTraverse;
}

//map<VAR, set<PROG_LINE>> modifiedMap, vector<pair<PROG_LINE, PROG_LINE>> affectsVec

void AffectsBipExtractor::extractAffectsBip() {
	NODE_LIST procToTraverse = getProcs();
	for (TNode& proc : procToTraverse) {
		unordered_map<VALUE, unordered_set<pair<int,int>>> history;
		vector<pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>>> affected;
		branchCount = 0;
		traverseAffects(proc.getChildren()[0], &history, &affected, 0);
		helperAffectsStar(&affected);
	}
}

//populate everything from the start of the proc to the end of the proc
void AffectsBipExtractor::traverseAffects(TNode& node, unordered_map<VALUE, unordered_set<pair<int,int>>>* modifiedHistory, 
	vector<pair<pair<PROG_LINE,int>, pair<PROG_LINE,int>>>* affected, int currentBranch) {
	for (TNode s : node.getChildren()) {
		//read statement
		if (s.getType() == TNode::NODE_TYPE::readStmt) {
			for (VALUE v : getModifies(&s)) {
				modifiedHistory->erase(v);
			}
		}

		//call statement
		if (s.getType() == TNode::NODE_TYPE::callStmt) {
			PROC_NAME procCalled = s.getValue();
			TNode& nodeCalled = procMap.at(procCalled);
			branchCount += 1;
			traverseAffects(nodeCalled.getChildren()[0], modifiedHistory, affected, branchCount);
		}

		//assignment
		if (s.getType() == TNode::NODE_TYPE::assignStmt) {
			VALUES modifiedVars = getModifies(&s);
			VALUES usedVars = getUses(&s);
			PROG_LINE nodeNum = to_string(s.getStmtNum());
			for (VALUE usedVar : usedVars) {
			
				if (modifiedHistory->find(usedVar) != modifiedHistory->end()) {
					unordered_set<pair<int,int>> assignStmts = modifiedHistory->at(usedVar);
					
					for (pair<int,int> vPair : assignStmts) {
						int stmtNum = vPair.first;
						PROG_LINE to = to_string(stmtNum);
						pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_BIP, to, nodeNum);
						pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_BIP_STAR, to, nodeNum);
						pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>> affectPair = { {to,vPair.second}, {nodeNum,currentBranch} };
						if ((std::find((*affected).begin(), (*affected).end(), affectPair) == (*affected).end())) {
							affected->push_back(affectPair);
						}
					}
				}
			}

			for (const VALUE v : modifiedVars) {
				modifiedHistory->erase(v);
				modifiedHistory->insert(std::unordered_map<VALUE, unordered_set<pair<int, int>>>::value_type(v, { make_pair(s.getStmtNum(),currentBranch) }));
			}
		}

		//if case
		if (s.getType() == TNode::NODE_TYPE::ifStmt) {
			unordered_map<VALUE, unordered_set<pair<int,int>>> seperateHistory = *modifiedHistory;
			TNode leftChild = s.getChildren()[0];
			TNode rightChild = s.getChildren()[1];
			traverseAffects(leftChild, modifiedHistory, affected, currentBranch);
			traverseAffects(rightChild, &seperateHistory, affected, currentBranch);

			for (auto const& line : seperateHistory) {
				if (modifiedHistory->find(line.first) != modifiedHistory->end()) {
					for (pair<int,int> vPair : line.second) {
						modifiedHistory->at(line.first).insert(vPair);
					}
				}
				else {
					modifiedHistory->insert({ line.first, line.second });
				}
			}
		}

		//while case
		if (s.getType() == TNode::NODE_TYPE::whileStmt) {
			unordered_map<VALUE, unordered_set<pair<int,int>>> previous;
			int savedBranchCount = branchCount;
			while (1) {
				branchCount = savedBranchCount;
				unordered_map<VALUE, unordered_set<pair<int,int>>> copy = *modifiedHistory;
				traverseAffects(s.getChildren()[0], &copy, affected, currentBranch);
				if (previous == copy) {
					break;
				}
				for (auto const& line : copy) {
					if (modifiedHistory->find(line.first) != modifiedHistory->end()) {
						for (pair<int,int> vPair : line.second) {
							modifiedHistory->at(line.first).insert(vPair);
						}
					}
					else {
						modifiedHistory->insert({ line.first, line.second });
					}
				}
				previous = copy;
			}

		}
	}
}

//get all the variables modified by a node
VALUES AffectsBipExtractor::getModifies(TNode* node) {
	VALUE num = node->getStmtNum();
	VALUES modifies = pkb.getSecondValuesOf(RELATIONSHIP_TYPE::MODIFIES_STMT, DESIGN_ENTITY::VARIABLE, to_string(num));
	return modifies;
}

//get all the variables used by a node
VALUES AffectsBipExtractor::getUses(TNode* node) {
	VALUE num = node->getStmtNum();
	VALUES uses = pkb.getSecondValuesOf(RELATIONSHIP_TYPE::USES_STMT, DESIGN_ENTITY::VARIABLE, to_string(num));
	return uses;
}

void AffectsBipExtractor::helperAffectsStar(vector<pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>>>* affected) {
	// affects star
	vector<pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>>>::iterator it1 = (*affected).begin();
	for (pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>> affectedPairs : (*affected)) {
		pair<PROG_LINE, int> affectedLine = affectedPairs.second;
		set<pair<PROG_LINE, int>> affectedLines;
		pair<PROG_LINE, int> affectsLine = affectedPairs.first;
		set<pair<PROG_LINE, int>> affectsLines;
		affectedLines.insert(affectedPairs.first);
		affectsLines.insert(affectedPairs.second);
		while (true) {
			set<pair<PROG_LINE, int>> copy1 = affectedLines;
			set<pair<PROG_LINE, int>> copy2 = affectsLines;

			for (pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>> currentPair : (*affected)) {
				{
					if (affectedLines.count(currentPair.second)) {
						pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_BIP_STAR, (currentPair.first).first, affectedLine.first);
						affectedLines.insert(currentPair.first);
					}
					if (affectsLines.count(currentPair.first)) {
						pkb.addRelationship(RELATIONSHIP_TYPE::AFFECTS_BIP_STAR, (affectsLine.first), currentPair.second.first);
						affectsLines.insert(currentPair.second);
					}
				}
			}
			if (copy1 == affectedLines && copy2 == affectsLines) {
				vector<pair<pair<PROG_LINE, int>, pair<PROG_LINE, int>>>::iterator it2 = (*affected).begin();
				goto end;
			}
		}
	end:
		continue;
	}
}


#pragma endregion

#pragma endregion
