/*
 * Lexer.h
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <unordered_map>

#include "Token.h"

namespace tjs {

class Lexer {
public:
	Lexer() { init(); setSource(""); }
	Lexer(std::string source) { init(); setSource(source); }
	virtual ~Lexer();

	void setSource(std::string source);
	Token* advance();
	Token* previous();
	Token* peek();
	bool check(TokenType type);
	bool isAtEnd();
private:
	std::string source;

	unsigned int tokenStart;
	unsigned int tokenCurr;
	unsigned int line;

	Token* prev;
	Token* next;

	std::unordered_map<std::string, TokenType> keywords;

	void init();
	Token* scanToken();
	Token* createToken(TokenType type, void* literal);
	Token* string();
	Token* number();
	Token* identifier();
	char nextChar();
	char peekChar();
	char peekNext();
	bool isDigit(char c);
	bool isAlpha(char c);
	bool isAlnum(char c);
	bool matchChar(char expected);
	bool endOfSource();
};

} /* namespace tjs */

#endif /* LEXER_H_ */
