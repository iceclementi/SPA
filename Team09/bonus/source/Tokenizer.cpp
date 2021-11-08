#include "Tokenizer.h"

namespace Tokenizer {

    TOKEN_TYPE getType(string s) {
        auto result = mapToEnum.find(s);
        if (result != mapToEnum.end()) {
            return result->second;
        }
        else {
            return TOKEN_TYPE::VARIABLE;
        }
    }

    TOKEN_TYPE getType(char c) {
        string s = "";
        s.push_back(c);
        auto result = mapToEnum.find(s);
        if (result != mapToEnum.end()) {
            return result->second;
        }
        else {
            throw "unexpected symbol";
        }
    }

    bool isNumber(string toCheck) {
        if (toCheck.size() == 1) {
            regex re("[0-9]");
            return regex_match(toCheck, re);
        } else {
            regex re("[1-9][0-9]*");
            return regex_match(toCheck, re);
        }
    }

    bool isName(string toCheck) {
        regex re("[a-zA-z][a-zA-Z0-9]*");
        return regex_match(toCheck, re);
    }

    Token determineIdentifier(string s, bool isAssignment) {
        if (isName(s)) {
            if (isAssignment) {
                return { TOKEN_TYPE::VARIABLE, s };
            }
            else {
                TOKEN_TYPE type = getType(s);
                isVariable = true;
                return { type, s };
            }
        }
        else if (isNumber(s)) {
            return { TOKEN_TYPE::CONSTANT, s };
        }
        else {
            throw "wrong syntax for variable";
        }

    }

    Token getDelimiterToken(char c) {
        TOKEN_TYPE type = getType(c);
        string s(1, c);
        return { type, s };
    }

    Token getConditionalToken(string s) {
        TOKEN_TYPE type = getType(s);
        return { type, s };
    }

    Token getLogicalToken(string s) {
        TOKEN_TYPE type = getType(s);
        return { type, s };
    }

    queue<Token> tokenize(istream& file, bool isExpression) {
        isVariable = false;
        queue<Token> tokens;
        char c;
        string identifier = "";
        string conditional = "";
        string logical = "";

        while (file.get(c)) {
            if (isalnum(c)) {
                identifier.push_back(c);
            }
            else {
                if (!identifier.empty()) {
                    while (isspace(c)) {
                        file.get(c);
                        if (file.eof()) {
                            break;
                        }
                    }

                    if (getType(identifier) == TOKEN_TYPE::THEN && (c == '{')) {
                        Token token = determineIdentifier(identifier, false);
                        tokens.emplace(token);
                    }
                    else {
                        isVariable = isVariable || (c == '=') || isExpression;
                        Token token = determineIdentifier(identifier, isVariable);
                        tokens.emplace(token);
                    }
         
                    identifier = "";
                }

                switch (c) {
                case ';': case '{':
                    tokens.emplace(getDelimiterToken(c));
                    isVariable = false;
                    break;
                case '(': case ')': case '}': case '%': 
                case '+': case '-': case '*': case '/':
                    tokens.emplace(getDelimiterToken(c));
                    break;
                case '>': case '<': case '!': case '=':
                    conditional.push_back(c);
                    if (file.peek() == '=') {
                        file.get(c);
                        conditional.push_back(c);
                    }
                    tokens.emplace(getConditionalToken(conditional));
                    conditional = "";
                    break;
                case '&': case '|':
                    logical.push_back(c);
                    if (file.peek() != c) {
                        throw "wrong syntax for logical";
                    }
                    else {
                        file.get(c);
                        logical.push_back(c);
                        tokens.emplace(getLogicalToken(logical));
                        logical = "";
                    }
                    break;
                default:
                    if (isspace(c)) {
                        break;
                    }
                    else if (isalnum(c)) {
                        identifier.push_back(c);
                        break;
                    }

                    string unknown(1, c);
                    throw "\nunknown character " + unknown + " in program";
                }

                if (file.eof()) {
                    break;
                }
            }
        }

        if (!identifier.empty()) {
            isVariable = isVariable || isExpression;
            Token token = determineIdentifier(identifier, isVariable);
            tokens.emplace(token);
        }

        return tokens;
    }

    queue<Token> tokenize(istream& file) {
        return tokenize(file, false);
    }

    queue<Token> tokenize(string& str) {
        istringstream stream(str);
        return tokenize(stream, true);
    }
}
