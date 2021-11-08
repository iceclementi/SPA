#include "QueryPreprocessor.h"

string WHITESPACE = " \f\n\r\t\v";
set<char> WHITESPACE_CHARS = { ' ', '\f', '\n', '\r', '\t', '\v' };
set<char> TERMINAL_SYMBOLS = { '"', ',', ';', '<', '>', '(', ')', '_', '=' };

QueryPreprocessor::QueryPreprocessor(string query) : query(query) {}

/*
* Returns a string with any leading whitespace trimmed
*/
string QueryPreprocessor::trimLeadingWhitespace(string str) {
	if (str.empty()) {
		return str;
	}

	size_t start = str.find_first_not_of(WHITESPACE);
	return start == string::npos ? "" : str.substr(start);
}

/*
* Returns a string with any trailing whitespace trimmed
*/
string QueryPreprocessor::trimTrailingWhitespace(string str) {
	if (str.empty()) {
		return str;
	}

	size_t end = str.find_last_not_of(WHITESPACE);
	return end == string::npos ? "" : str.substr(0, end + 1);
}

/*
* Parses and validates name
* Returns the name or an error message if name is invalid
*/
string QueryPreprocessor::parseName(string name) {
	if (name.size() < 1) {
		// Size is not at least 1
		return SYNTAX_ERROR;
	}
	for (string::size_type i = 0; i < name.size(); i++) {
		bool valid;
		if (i == 0) {
			// First character must be a letter
			valid = isalpha(name[i]);
		}
		else {
			valid = isalpha(name[i]) || isdigit(name[i]);
		}
		if (!valid) {
			return SYNTAX_ERROR;
		}
	}
	return name;
}

/*
* Parses and validates ident with quotation marks
* Returns the ident (without quotation marks) or an error message if ident is invalid
*/
string QueryPreprocessor::parseIdentWithQuotationMarks(string ident) {
	if (ident.size() <= 2 || ident.front() != '"' || ident.back() != '"') {
		// No letters and/or digits between quotation marks or no start and/or end quotation marks
		return SYNTAX_ERROR;
	}
	ident = ident.substr(1, ident.size() - 2);
	ident = trimLeadingWhitespace(ident);
	ident = trimTrailingWhitespace(ident);
	return parseName(ident);
}

/*
* Parses and validates integer
* Returns the integer or an error message if integer is invalid
*/
string QueryPreprocessor::parseInteger(string integer) {
	if (integer.size() < 1) {
		// Size is not at least 1
		return SYNTAX_ERROR;
	}
	if (integer[0] == '0') {
		// Invalid integer starting with 0
		return SEMANTIC_ERROR;
	}
	for (string::size_type i = 0; i < integer.size(); i++) {
		if (!isdigit(integer[i])) {
			return SYNTAX_ERROR;
		}
	}
	return integer;
}

/*
* Parses and validates synonym
* Returns the synonym or an error message if synonym is invalid
*/
string QueryPreprocessor::parseSynonym(string synonym) {
	return parseName(synonym);
}

/*
* Parses and validates element as attribute reference
* Returns a QueryElement consisting of:
* (1) - error message (NO_ERROR by default)
* (2) - synonym index
* (3) - synonym type
* (4) - attribute name type (compulsory)
*/
QueryElement QueryPreprocessor::parseAttrRef(string attrRef) {
	QueryElement elem;
	string delimiter = ".";
	size_t delimiterLength = delimiter.length();
	size_t pos = attrRef.find(delimiter);

	if (pos == string::npos || pos == 0 || pos + delimiterLength == attrRef.length()) {
		// No "." or no synonym or no attribute name
		elem.setErrorMessage(SYNTAX_ERROR);
		return elem;
	}

	// Extract and validate synonym
	string synonym = attrRef.substr(0, pos);
	if (!isSynonymDeclared(synonym)) {
		if (parseSynonym(synonym) == SYNTAX_ERROR) {
			// Invalid synonym
			elem.setErrorMessage(SYNTAX_ERROR);
		}
		else {
			// Synonym is not declared
			elem.setErrorMessage(SEMANTIC_ERROR);
		}
		return elem;
	}
	int synIndex = declaredSynonymsToIndex[synonym];
	elem.setSynIndex(synIndex);
	DESIGN_ENTITY synDesignEntity = declaredSynIndexToDesignEntity[synIndex];
	elem.setSynType(synDesignEntity);

	// Extract and validate attribute name type
	string attrName = attrRef.substr(pos + delimiterLength);
	if (mapStrToAttrType.find(attrName) == mapStrToAttrType.end()) {
		// Invalid attribute name
		elem.setErrorMessage(SYNTAX_ERROR);
		return elem;
	}
	set<DESIGN_ENTITY> validSynTypes = mapStrToAttrNameSynTypes[attrName];
	if (validSynTypes.find(synDesignEntity) == validSynTypes.end()) {
		// Invalid combination of synonym and attribute name
		elem.setErrorMessage(SEMANTIC_ERROR);
		return elem;
	}
	elem.setAttrType(mapStrToAttrType[attrName]);

	return elem;
}

/*
* Parses and validates element as synonym
* Returns a QueryElement consisting of:
* (1) - error message (NO_ERROR by default)
* (2) - synonym index
* (3) - synonym type
* (4) - ATTRIBUTE_TYPE::NONE as attribute name type
*/
QueryElement QueryPreprocessor::parseSyn(string syn) {
	QueryElement elem;
	if (!isSynonymDeclared(syn)) {
		if (parseSynonym(syn) == SYNTAX_ERROR) {
			// Invalid synonym
			elem.setErrorMessage(SYNTAX_ERROR);
		}
		else {
			// Synonym is not declared
			elem.setErrorMessage(SEMANTIC_ERROR);
		}
		return elem;
	}
	int synIndex = declaredSynonymsToIndex[syn];
	elem.setSynIndex(synIndex);
	elem.setSynType(declaredSynIndexToDesignEntity[synIndex]);
	return elem;
}

/*
* Parses and validates element
* Returns a QueryElement consisting of:
* (1) - error message (NO_ERROR by default)
* (2) - synonym index
* (3) - synonym type
* (4) - attribute name type (if element is an attribute reference)
*/
QueryElement QueryPreprocessor::parseElem(string element) {
	// Parse element as attribute reference
	string delimiter = ".";
	if (element.find(delimiter) != string::npos) {
		return parseAttrRef(element);
	}

	// Parse element as synonym
	return parseSyn(element);
}

/*
* Parses and validates expression spec
* Returns a QueryPattern consisting of:
* (1) - type of pattern match
* (2) - expression spec or an error message if expression spec is invalid
*/
QueryPattern QueryPreprocessor::parseExpressionSpec(string expressionSpec) {
	size_t len = expressionSpec.size();
	if (expressionSpec == "_") {
		return { PATTERN_MATCH_TYPE::UNRESTRICTED, ""};
	}
	if (len > 2 && expressionSpec.front() == '"' && expressionSpec.back() == '"') {
		expressionSpec = expressionSpec.substr(1, len - 2);
		return { PATTERN_MATCH_TYPE::EXACT_MATCH, expressionSpec };
	}
	if (len > 4 && expressionSpec.front() == '_' && expressionSpec.back() == '_') {
		expressionSpec = expressionSpec.substr(1, len - 2);
		expressionSpec = trimLeadingWhitespace(expressionSpec);
		expressionSpec = trimTrailingWhitespace(expressionSpec);
		len = expressionSpec.size();
		if (len > 2 && expressionSpec.front() == '"' && expressionSpec.back() == '"') {
			expressionSpec = expressionSpec.substr(1, len - 2);
			return { PATTERN_MATCH_TYPE::PARTIAL_MATCH, expressionSpec };
		}
	}
	return { PATTERN_MATCH_TYPE::INVALID, SYNTAX_ERROR };
}

/*
* Parses and validates design entity
* Returns the design entity or an error message if design entity is invalid
*/
string QueryPreprocessor::parseDesignEntity(string designEntity) {
	if (mapStrToDesignEntity.find(designEntity) == mapStrToDesignEntity.end()) {
		return SYNTAX_ERROR;
	}
	return designEntity;
}

/*
* Returns whether synonym is in declared synonyms table or not
*/
bool QueryPreprocessor::isSynonymDeclared(SYNONYM synonym) {
	return declaredSynonymsToIndex.count(synonym);
}

/*
* Validates argument based on its argument type
* Returns a query argument containing information from the validated argument
*/
QueryArgument QueryPreprocessor::validateArgument(ARGUMENT_TYPE argType, ARGUMENT argStr) {
	QueryArgument queryArgument;

	switch (argType) {
	case ARGUMENT_TYPE::SYNONYM: {
		QueryElement elem = parseSyn(argStr);
		if (elem.hasError()) {
			queryArgument.setErrorMessage(elem.getErrorMessage());
			break;
		}
		queryArgument.setArgElem(elem);
		queryArgument.setArgStr(argStr);
		break;
	}
	case ARGUMENT_TYPE::UNDERSCORE: {
		if (argStr != "_") {
			queryArgument.setErrorMessage(SYNTAX_ERROR);
			break;
		}
		queryArgument.setArgStr("");
		break;
	}
	case ARGUMENT_TYPE::INTEGER: {
		argStr = parseInteger(argStr);
		if (argStr == SYNTAX_ERROR || argStr == SEMANTIC_ERROR) {
			queryArgument.setErrorMessage(argStr);
			break;
		}
		queryArgument.setArgStr(argStr);
		break;
	}
	case ARGUMENT_TYPE::IDENT: {
		argStr = parseIdentWithQuotationMarks(argStr);
		if (argStr == SYNTAX_ERROR) {
			queryArgument.setErrorMessage(SYNTAX_ERROR);
			break;
		}
		queryArgument.setArgStr(argStr);
		break;
	}
	case ARGUMENT_TYPE::ATTRIBUTE_REF: {
		QueryElement elem = parseAttrRef(argStr);
		if (elem.hasError()) {
			queryArgument.setErrorMessage(elem.getErrorMessage());
			break;
		}
		queryArgument.setArgElem(elem);
		queryArgument.setArgStr(argStr);
		break;
	}
	}

	if (queryArgument.hasError()) {
		queryArgument.setArgType(ARGUMENT_TYPE::INVALID);
	}
	else {
		queryArgument.setArgType(argType);
	}
	
	return queryArgument;
}

/*
* Extracts (possibly multiple) token(s) as an argument
* Returns the extracted argument as a string
*/
string QueryPreprocessor::extractArgument() {
	string s = queryTokens.front();
	queryTokens.pop_front();
	if (s == "\"") {
		// Extract (valid) ident or (valid) expression spec
		while (!queryTokens.empty() && queryTokens.front() != "\"") {
			s.append(queryTokens.front());
			queryTokens.pop_front();
		}
		if (!queryTokens.empty() && queryTokens.front() == "\"") {
			s.append(queryTokens.front());
			queryTokens.pop_front();
		}
		return s;
	}
	else if (s == "_") {
		if (!queryTokens.empty() && queryTokens.front() == "\"") {
			// Extract (valid) expression spec
			s.append(queryTokens.front());
			queryTokens.pop_front();
			while (!queryTokens.empty() && queryTokens.front() != "\"") {
				s.append(queryTokens.front());
				queryTokens.pop_front();
			}
			if (!queryTokens.empty() && queryTokens.front() == "\"") {
				s.append(queryTokens.front());
				queryTokens.pop_front();
			}
			if (!queryTokens.empty() && queryTokens.front() == "_") {
				s.append(queryTokens.front());
				queryTokens.pop_front();
			}
		}
		return s;
	}
	else {
		// Extract argument as a single token
		return s;
	}
}

/*
* Extracts list of arguments between parentheses for such that and pattern clause
* Returns the list of arguments or an empty list if there is any syntax error
*/
list<string> QueryPreprocessor::extractArgumentsBetweenParentheses() {
	list<string> argList;

	if (queryTokens.empty()) {
		// Missing "("
		queryObject.errorMessage = SYNTAX_ERROR;
		return argList;
	}

	if (queryTokens.front() != "(") {
		// Does not start with "("
		queryObject.errorMessage = SYNTAX_ERROR;
		return argList;
	}
	queryTokens.pop_front();

	bool checkCommaOrCloseParanthesis = false;
	bool isCompleteArgs = false;
	while (!queryTokens.empty()) {
		if (checkCommaOrCloseParanthesis) {
			string s = queryTokens.front();
			queryTokens.pop_front();
			if (s == ",") {
				checkCommaOrCloseParanthesis = false;
			}
			else if (s == ")") {
				isCompleteArgs = true;
				break;
			}
			else {
				// Missing comma or close paranthesis
				argList.clear();
				queryObject.errorMessage = SYNTAX_ERROR;
				return argList;
			}
		}
		else {
			argList.push_back(extractArgument());
			checkCommaOrCloseParanthesis = true;
		}
	}
	if (!isCompleteArgs) {
		// Incomplete arguments
		argList.clear();
		queryObject.errorMessage = SYNTAX_ERROR;
		return argList;
	}

	return argList;
}

/*
* Validates arguments in such-that clause
* Returns a query object containing information about the such-that clause
*/
void QueryPreprocessor::validateSuchThatClauseArgs(SUCH_THAT_REL_TYPE relType, list<string> argList) {
	SuchThatClause stc;
	vector<set<ARGUMENT_TYPE>> validArgsTypes = mapSuchThatRelTypeToArgsTypes[relType];
	vector<set<DESIGN_ENTITY>> validArgsSynTypes = mapSuchThatRelTypeToArgsSynTypes[relType];

	int numOfArgs = validArgsTypes.size();
	if (argList.size() != numOfArgs) {
		// Invalid no. of arguments
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	for (int i = 0; i < numOfArgs; i++) {
		bool isValidArg = false;
		string argStr = argList.front();
		argList.pop_front();
		for (ARGUMENT_TYPE const& argType : validArgsTypes[i]) {
			QueryArgument arg = validateArgument(argType, argStr);
			if (arg.getErrorMessage() == SEMANTIC_ERROR) {
				// Terminate due to semantic error
				queryObject.errorMessage = SEMANTIC_ERROR;
				return;
			}
			if (arg.getErrorMessage() == SYNTAX_ERROR) {
				// Continue checking for other argument types
				continue;
			}
			
			if (argType == ARGUMENT_TYPE::SYNONYM) {
				DESIGN_ENTITY argSynType = arg.getArgElem().getSynType();
				if (validArgsSynTypes[i].find(argSynType) == validArgsSynTypes[i].end()) {
					// Invalid argument synonym type
					queryObject.errorMessage = SEMANTIC_ERROR;
					return;
				}
				stc.addClauseSynonym(arg.getArgElem().getSynIndex());
			}
			isValidArg = true;
			if (i == 0) {
				stc.firstArg = arg;
			}
			if (i == 1) {
				stc.secondArg = arg;
			}
			break;
		}
		if (!isValidArg) {
			if ((relType == SUCH_THAT_REL_TYPE::MODIFIES || relType == SUCH_THAT_REL_TYPE::USES) && 
				i == 0 && 
				!validateArgument(ARGUMENT_TYPE::UNDERSCORE, argStr).hasError()) {
				// First argument of Modifies and Uses cannot be "_"
				queryObject.errorMessage = SEMANTIC_ERROR;
				return;
			}
			// No valid argument type
			queryObject.errorMessage = SYNTAX_ERROR;
			return;
		}
	}

	stc.relType = relType;
	queryObject.addSuchThatClause(stc, ++clauseIndex);
}

/*
* Parses and validates such-that clause
* Returns a query object containing information about the such-that clause
*/
void QueryPreprocessor::parseSuchThatClause() {
	if (queryTokens.empty()) {
		// Missing relationship type
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	// Extract and validate relationship type
	string relTypeStr = queryTokens.front();
	queryTokens.pop_front();
	if (mapStrToSuchThatRelType.find(relTypeStr) == mapStrToSuchThatRelType.end()) {
		// Invalid relationship type
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	SUCH_THAT_REL_TYPE relType = mapStrToSuchThatRelType[relTypeStr];

	// Extract and validate arguments
	list<string> argList = extractArgumentsBetweenParentheses();
	if (argList.empty()) {
		// Invalid arguments syntax
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	validateSuchThatClauseArgs(relType, argList);
}

/*
* Validates arguments in pattern clause
* Returns a query object containing information about the pattern clause
*/
void QueryPreprocessor::validatePatternClauseArgs(PatternClause pc, list<string> argList) {
	DESIGN_ENTITY patternDesignEntity = pc.patternSynType;
	vector<set<ARGUMENT_TYPE>> validArgsTypes = mapPatternSynTypeToArgsTypes[patternDesignEntity];
	vector<set<DESIGN_ENTITY>> validArgsSynTypes = mapPatternSynTypeToArgsSynTypes[patternDesignEntity];
	
	int numOfArgs = validArgsTypes.size();
	if (argList.size() != numOfArgs) {
		// Invalid no. of arguments
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	for (int i = 0; i < numOfArgs; i++) {
		bool isValidArg = false;
		string argStr = argList.front();
		argList.pop_front();
		for (ARGUMENT_TYPE const& argType : validArgsTypes[i]) {
			if (argType == ARGUMENT_TYPE::EXPRESSION_SPEC) {
				QueryPattern pattern = parseExpressionSpec(argStr);
				if (pattern.patternMatchType == PATTERN_MATCH_TYPE::INVALID) {
					// Invalid expression spec
					queryObject.errorMessage = SYNTAX_ERROR;
					return;
				}
				isValidArg = true;
				pc.pattern = pattern;
				break;
			}

			QueryArgument arg = validateArgument(argType, argStr);
			if (arg.getErrorMessage() == SEMANTIC_ERROR) {
				// Terminate due to semantic error
				queryObject.errorMessage = SEMANTIC_ERROR;
				return;
			}
			if (arg.getErrorMessage() == SYNTAX_ERROR) {
				// Continue checking for other argument types
				continue;
			}

			if (argType == ARGUMENT_TYPE::SYNONYM) {
				DESIGN_ENTITY argSynType = arg.getArgElem().getSynType();
				if (validArgsSynTypes[i].find(argSynType) == validArgsSynTypes[i].end()) {
					// Invalid argument synonym type
					queryObject.errorMessage = SEMANTIC_ERROR;
					return;
				}
				pc.addClauseSynonym(arg.getArgElem().getSynIndex());
			}
			isValidArg = true;
			if (i == 0) {
				pc.arg = arg;
			}
			break;
		}
		if (!isValidArg) {
			// No valid argument type
			queryObject.errorMessage = SYNTAX_ERROR;
			return;
		}
	}

	queryObject.addPatternClause(pc, ++clauseIndex);
}

/*
* Parses and validates pattern clause
* Returns a query object containing information about the pattern clause
*/
void QueryPreprocessor::parsePatternClause() {
	PatternClause pc;

	if (queryTokens.empty()) {
		// Missing pattern synonym type
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	// Extract and validate pattern synonym type
	string synonym = queryTokens.front();
	queryTokens.pop_front();
	if (!isSynonymDeclared(synonym)) {
		if (parseSynonym(synonym) == SYNTAX_ERROR) {
			// Invalid pattern synonym
			queryObject.errorMessage = SYNTAX_ERROR;
		}
		else {
			// Pattern synonym is not declared
			queryObject.errorMessage = SEMANTIC_ERROR;
		}
	}
	int synIndex = declaredSynonymsToIndex[synonym];
	DESIGN_ENTITY designEntity = declaredSynIndexToDesignEntity[synIndex];
	if (!validPatternSynType.count(designEntity)) {
		// Invalid pattern synonym type
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	pc.patternSynType = designEntity;
	pc.synIndex = synIndex;
	pc.addClauseSynonym(synIndex);

	// Extract and validate arguments
	list<string> argList = extractArgumentsBetweenParentheses();
	if (argList.empty()) {
		// Invalid arguments syntax
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	validatePatternClauseArgs(pc, argList);
}

/*
* Validates arguments in with clause
* Returns a query object containing information about the with clause
*/
void QueryPreprocessor::validateWithClauseArgs(list<string> argList) {
	WithClause wc;
	WITH_ARG_TYPE firstArgType;
	WITH_ARG_TYPE secondArgType;

	// Note: parseWithClause method ensures that argList is of size 2 only
	for (int i = 0; i < 2; i++) {
		bool isValidArg = false;
		string argStr = argList.front();
		argList.pop_front();
		for (ARGUMENT_TYPE const& argType : validWithArgsTypes[i]) {
			QueryArgument arg = validateArgument(argType, argStr);
			if (arg.getErrorMessage() == SEMANTIC_ERROR) {
				// Terminate due to semantic error
				queryObject.errorMessage = SEMANTIC_ERROR;
				return;
			}
			if (arg.getErrorMessage() == SYNTAX_ERROR) {
				// Continue checking for other argument types
				continue;
			}

			if (argType == ARGUMENT_TYPE::SYNONYM) {
				DESIGN_ENTITY argSynType = arg.getArgElem().getSynType();
				if (validWithArgsSynTypes[i].find(argSynType) == validWithArgsSynTypes[i].end()) {
					// Invalid argument synonym type
					queryObject.errorMessage = SEMANTIC_ERROR;
					return;
				}
				wc.addClauseSynonym(arg.getArgElem().getSynIndex());
			}
			else if (argType == ARGUMENT_TYPE::ATTRIBUTE_REF) {
				wc.addClauseSynonym(arg.getArgElem().getSynIndex());
			}
			isValidArg = true;
			if (i == 0) {
				wc.firstArg = arg;
				firstArgType = argType == ARGUMENT_TYPE::ATTRIBUTE_REF
					? mapAttrRefTypeToGeneralWithArgType[arg.getArgElem().getAttrType()]
					: mapArgTypeToGeneralWithArgType[argType];
			}
			else if (i == 1) {
				wc.secondArg = arg;
				secondArgType = argType == ARGUMENT_TYPE::ATTRIBUTE_REF
					? mapAttrRefTypeToGeneralWithArgType[arg.getArgElem().getAttrType()]
					: mapArgTypeToGeneralWithArgType[argType];
			}
			break;
		}
		if (!isValidArg) {
			// No valid argument type
			queryObject.errorMessage = SYNTAX_ERROR;
			return;
		}
	}

	// Validate compatability of the two arguments
	if (firstArgType != secondArgType) {
		queryObject.errorMessage = SEMANTIC_ERROR;
		return;
	}

	queryObject.addWithClause(wc, ++clauseIndex);
}

/*
* Parses and validates with clause
* Returns a query object containing information about the with clause
*/
void QueryPreprocessor::parseWithClause() {
	list<string> argList;

	if (queryTokens.empty()) {
		// Missing first argument
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	// Extract first argument
	argList.push_back(extractArgument());

	if (queryTokens.empty()) {
		// Missing "="
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	// Extract "="
	if (queryTokens.front() != "=") {
		// Next token is not "="
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	queryTokens.pop_front();

	if (queryTokens.empty()) {
		// Missing second argument
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	// Extract second argument
	argList.push_back(extractArgument());

	// Validate arguments
	validateWithClauseArgs(argList);
}

/*
* Splits clauses into individual clause
* Returns a query object containing information about the parsed split clauses
*/
void QueryPreprocessor::splitAndParseClauses() {
	while (!queryTokens.empty()) {
		string first = queryTokens.front();
		queryTokens.pop_front();

		if (first == "and") {
			if (prevClauseType == CLAUSE_TYPE::INVALID) {
				// Invalid query clause starting with "and"
				queryObject.errorMessage = SYNTAX_ERROR;
				return;
			}
			first = mapClauseTypeToFirstWord[prevClauseType];
			if (first == "such") {
				queryTokens.push_front("that");
			}
		}

		if (first == "such") {
			if (queryTokens.empty()) {
				// Missing "that" keyword
				queryObject.errorMessage = SYNTAX_ERROR;
				return;
			}
			string second = queryTokens.front();
			queryTokens.pop_front();
			if (second != "that") {
				// Next token is not "that"
				queryObject.errorMessage = SYNTAX_ERROR;
				return;
			}
			parseSuchThatClause();
			prevClauseType = CLAUSE_TYPE::SUCH_THAT;
		}
		else if (first == "pattern") {
			parsePatternClause();
			prevClauseType = CLAUSE_TYPE::PATTERN;
		}
		else if (first == "with") {
			parseWithClause();
			prevClauseType = CLAUSE_TYPE::WITH;
		}
		else {
			// Invalid start of such that, pattern or with clause
			queryObject.errorMessage = SYNTAX_ERROR;
			return;
		}

		// Check for error
		if (queryObject.hasError()) {
			return;
		}
	}
}

/*
* Parses and validates result clause
* Returns a query object containing infromation from the parsed result clause
*/
void QueryPreprocessor::parseResultClause() {
	QuerySelectResult qsr;

	if (queryTokens.empty()) {
		// No result clause
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	string first = queryTokens.front();
	queryTokens.pop_front();

	if (first == "<") {
		// Parse as tuple of multiple elements
		bool checkCommaOrCloseAngleBracket = false;
		bool isCompleteTuple = false;
		while (!queryTokens.empty()) {
			if (checkCommaOrCloseAngleBracket) {
				string s = queryTokens.front();
				queryTokens.pop_front();
				if (s == ",") {
					checkCommaOrCloseAngleBracket = false;
				}
				else if (s == ">") {
					isCompleteTuple = true;
					break;
				}
				else {
					// Missing comma or close angle bracket
					queryObject.errorMessage = SYNTAX_ERROR;
					return;
				}
			}
			else {
				string element = queryTokens.front();
				queryTokens.pop_front();
				QueryElement elem = parseElem(element);
				if (elem.hasError()) {
					// Invalid element
					queryObject.errorMessage = elem.getErrorMessage();
					return;
				}
				qsr.addToSelectElementList(elem);
				qsr.addSelectSynonym(elem.getSynIndex());
				checkCommaOrCloseAngleBracket = true;
			}
		}
		if (!isCompleteTuple) {
			// Incomplete tuple
			queryObject.errorMessage = SYNTAX_ERROR;
			return;
		}
		queryObject.setSelectResult(qsr);
	}
	else {
		// Parse as one element
		QueryElement elem = parseElem(first);
		if (elem.hasError()) {
			if (first != "BOOLEAN") {
				// Invalid element
				queryObject.errorMessage = elem.getErrorMessage();
				return;
			}
			// Parse as BOOLEAN
			qsr.setSelectBoolean(true);
			queryObject.setSelectResult(qsr);
			return;
		}
		qsr.addToSelectElementList(elem);
		qsr.addSelectSynonym(elem.getSynIndex());
		queryObject.setSelectResult(qsr);
	}
}

/*
* Parses and validates declarations
* Returns a query object containing infromation from the parsed declarations
*/
void QueryPreprocessor::parseDeclarations() {
	if (queryTokens.empty()) {
		// No "Select" keyword
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}

	if (queryTokens.front() == "Select") {
		queryTokens.pop_front();
		return;
	}

	// Extract and validate design entity
	string designEntityStr = queryTokens.front();
	queryTokens.pop_front();
	designEntityStr = parseDesignEntity(designEntityStr);
	if (designEntityStr == SYNTAX_ERROR) {
		// Invalid design entity
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	DESIGN_ENTITY designEntity = mapStrToDesignEntity[designEntityStr];

	// Extract and validate synonym(s)
	bool checkCommaOrSemicolon = false;
	bool isCompleteDeclaration = false;
	while (!queryTokens.empty()) {
		if (checkCommaOrSemicolon) {
			string s = queryTokens.front();
			queryTokens.pop_front();
			if (s == ",") {
				checkCommaOrSemicolon = false;
			}
			else if (s == ";") {
				isCompleteDeclaration = true;
				break;
			}
			else {
				// Missing comma or semicolon
				queryObject.errorMessage = SYNTAX_ERROR;
				return;
			}
		}
		else {
			string synonym = queryTokens.front();
			queryTokens.pop_front();
			synonym = parseSynonym(synonym);
			if (synonym == SYNTAX_ERROR) {
				// Invalid synonym
				queryObject.errorMessage = SYNTAX_ERROR;
				return;
			}
			if (isSynonymDeclared(synonym)) {
				// Repeated synonym
				queryObject.errorMessage = SEMANTIC_ERROR;
				// Continue checking
			}
			synonymIndex++;
			declaredSynIndexToDesignEntity[synonymIndex] = designEntity;
			declaredSynonymsToIndex[synonym] = synonymIndex;
			checkCommaOrSemicolon = true;
		}
	}
	if (!isCompleteDeclaration) {
		// Incomplete declaration
		queryObject.errorMessage = SYNTAX_ERROR;
		return;
	}
	
	// Recursion to get more declarations if any
	parseDeclarations();
}

/*
* Tokenises PQL query
*/
void QueryPreprocessor::tokeniseQuery() {
	string token;
	for (char& c : query) {
		if (WHITESPACE_CHARS.find(c) == WHITESPACE_CHARS.end()) {
			if (TERMINAL_SYMBOLS.find(c) != TERMINAL_SYMBOLS.end()) {
				if (token == "prog" && c == '_') {
					token.push_back(c);
				}
				else {
					if (token.length() != 0) {
						queryTokens.push_back(token);
						token.clear();
					}
					string s(1, c);
					queryTokens.push_back(s);
				}
			}
			else {
				token.push_back(c);
			}
		}
		else {
			if (token.length() != 0) {
				queryTokens.push_back(token);
				token.clear();
			}
		}
	}
	if (token.length() != 0) {
		queryTokens.push_back(token);
		token.clear();
	}
}

/*
* Parses and validates PQL query
* Returns a query object containing information from the parsed query
*/
QueryObject& QueryPreprocessor::parseQuery() {
	queryObject.errorMessage = NO_ERROR;

	// Tokenise query
	tokeniseQuery();

	// Parse and validate declarations
	parseDeclarations();
	if (queryObject.errorMessage == SYNTAX_ERROR) {
		queryObject.handleError();
		return queryObject;
	}
	// Continue checking if result clause is "Select BOOLEAN" if there is a semantic error

	// Parse and validate result clause
	parseResultClause();
	if (queryObject.hasError()) {
		queryObject.handleError();
		return queryObject;
	}

	// Split, parse and validate such-that, pattern and with clauses
	splitAndParseClauses();
	if (queryObject.hasError()) {
		queryObject.handleError();
		return queryObject;
	}

	return queryObject;
}
