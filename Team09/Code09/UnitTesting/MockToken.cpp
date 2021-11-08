#include "stdafx.h"
#include "MockToken.h"

using namespace Tokenizer;

namespace MockToken {
	
	Token MockToken::createToken(string v) {
		TOKEN_TYPE type;
		auto e = mapToEnum.find(v);
		type = e->second;
		return { type, v };
	};

	Token MockToken::createToken(string v, bool isVariable) {
		if (isVariable) {
			return { TOKEN_TYPE::VARIABLE, v };
		}
		else {
			return { TOKEN_TYPE::CONSTANT, v };
		}
	};

	queue<Token> MockToken::allSymbols() {
		vector<string> all = {"procedure", "+","-","*","/","%","!","&&","||",">","<",">=","<=","==","!=","{","}",
							   "(",")",";","=", };

		queue<Token> result;

		for (string t : all) {
			Token temp = createToken(t);
			result.emplace(temp);
		}

		return result;
	}

	queue<Token> MockToken::allIdentifier() {
		vector<string> all = {"print", "read", "call", "while", "if"};

		queue<Token> result;

		for (string t : all) {
			Token temp = createToken(t);
			result.emplace(temp);
		}
		result.emplace(createToken("variable", 1));

		return result;
	}

	queue<Token> MockToken::callStmtTokens(string procedure) {

		queue<Token> result;

		Token call = createToken("call");
		Token procedureName = createToken(procedure ,1);
		Token semiColon = createToken(";");

		result.emplace(call);
		result.emplace(procedureName);
		result.emplace(semiColon);

		return result;
	}

	queue<Token> MockToken::readStmtTokens(string modifiedVariable) {
		queue<Token> result;

		Token read = createToken("read");
		Token variable = createToken(modifiedVariable, 1);
		Token semiColon = createToken(";");

		result.emplace(read);
		result.emplace(variable);
		result.emplace(semiColon);

		return result;
	}

	queue<Token> MockToken::printStmtTokens(string usedVariable) {
		queue<Token> result;
		Token print = createToken("print");
		Token variable = createToken(usedVariable, 1);
		Token semiColon = createToken(";");

		result.emplace(print);
		result.emplace(variable);
		result.emplace(semiColon);

		return result;
	}

	
	queue<Token> createTokens(vector<string> expr) {
		queue<Token> result;

		for (string t : expr) {
			if (isNumber(t)) {
				result.emplace(createToken(t, 0));
			}
			else if (isName(t)) {
				result.emplace(createToken(t, 1));
			}
			else {
				result.emplace(createToken(t));
			}

		}

		return result;
	};


	queue<Token> MockToken::create_test1() {
		queue<Token> result;

		result.emplace(createToken("procedure"));
		result.emplace(createToken("name", 1));
		
		result.emplace(createToken("{"));
		
		result.emplace(createToken("variable",1));
		result.emplace(createToken("="));
		result.emplace(createToken("variable",1 ));
		result.emplace(createToken("+"));
		result.emplace(createToken("variable",1));
		result.emplace(createToken(";"));
		
		result.emplace(createToken("variable", 1));
		result.emplace(createToken("="));
		result.emplace(createToken("variable", 1));
		result.emplace(createToken("-"));
		result.emplace(createToken("1", 0));
		result.emplace(createToken(";"));

		
		result.emplace(createToken("variable", 1));
		result.emplace(createToken("="));
		result.emplace(createToken("100", 0));
		result.emplace(createToken("/"));
		result.emplace(createToken("variable",1));
		result.emplace(createToken(";"));
		
		result.emplace(createToken("variable", 1));
		result.emplace(createToken("="));
		result.emplace(createToken("0", 0));
		result.emplace(createToken("*"));
		result.emplace(createToken("1", 0));
		result.emplace(createToken(";"));
		
		result.emplace(createToken("if"));
		result.emplace(createToken("("));
		result.emplace(createToken("x", 1));
		result.emplace(createToken("<"));
		result.emplace(createToken("0", 0));
		result.emplace(createToken(")"));
		result.emplace(createToken("then"));
		result.emplace(createToken("{"));

		result.emplace(createToken("x", 1));
		result.emplace(createToken("="));
		result.emplace(createToken("x", 1));
		result.emplace(createToken("%"));
		result.emplace(createToken("2", 0));
		result.emplace(createToken(";"));
		
		result.emplace(createToken("}"));
		result.emplace(createToken("else"));
		result.emplace(createToken("{"));

		result.emplace(createToken("x", 1));
		result.emplace(createToken("="));
		result.emplace(createToken("2", 0));
		result.emplace(createToken("%"));
		result.emplace(createToken("x", 1));
		result.emplace(createToken(";"));

		result.emplace(createToken("}"));

		result.emplace(createToken("while"));
		result.emplace(createToken("("));
		result.emplace(createToken("("));
		result.emplace(createToken("("));
		result.emplace(createToken("while", 1));
		result.emplace(createToken("<="));
		result.emplace(createToken("1", 0));
		result.emplace(createToken(")"));
		result.emplace(createToken("&&"));
		result.emplace(createToken("("));
		result.emplace(createToken("if", 1));
		result.emplace(createToken(">"));
		result.emplace(createToken("2", 0));
		result.emplace(createToken(")"));
		result.emplace(createToken(")"));
		
		result.emplace(createToken("||"));

		result.emplace(createToken("("));
		result.emplace(createToken("("));
		result.emplace(createToken("call", 1));
		result.emplace(createToken(">="));
		result.emplace(createToken("3", 0));
		result.emplace(createToken(")"));
		result.emplace(createToken("&&"));
		result.emplace(createToken("("));
		result.emplace(createToken("procedure", 1));
		result.emplace(createToken("!="));
		result.emplace(createToken("3", 0));
		result.emplace(createToken(")"));
		result.emplace(createToken(")"));

		result.emplace(createToken("||"));
		result.emplace(createToken("("));
		result.emplace(createToken("then", 1));
		result.emplace(createToken("=="));
		result.emplace(createToken("4", 0));
		result.emplace(createToken(")"));

		result.emplace(createToken(")"));
		result.emplace(createToken("{"));

		result.emplace(createToken("read"));
		result.emplace(createToken("read",1));
		result.emplace(createToken(";"));
		
		result.emplace(createToken("print"));
		result.emplace(createToken("print", 1));
		result.emplace(createToken(";"));

		result.emplace(createToken("call"));
		result.emplace(createToken("call", 1));
		result.emplace(createToken(";"));

		result.emplace(createToken("procedure", 1));
		result.emplace(createToken("="));
		result.emplace(createToken("call", 1));
		result.emplace(createToken("+"));
		result.emplace(createToken("read", 1));
		result.emplace(createToken("-"));
		result.emplace(createToken("print", 1));
		result.emplace(createToken("*"));
		result.emplace(createToken("procedure", 1));
		result.emplace(createToken("%"));
		result.emplace(createToken("if", 1));
		result.emplace(createToken("+"));
		result.emplace(createToken("then", 1));
		result.emplace(createToken("-"));
		result.emplace(createToken("else", 1));
		result.emplace(createToken("*"));
		result.emplace(createToken("while", 1));
		result.emplace(createToken(";"));

		result.emplace(createToken("}"));
		result.emplace(createToken("}"));
		
		return result;
	};
}