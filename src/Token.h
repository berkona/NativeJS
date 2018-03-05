/*
 * Token.h
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream>

namespace tjs {

enum TokenType {
	LEFT_PAREN, RIGHT_PAREN,
	LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT,
	MINUS, PLUS,
	SEMICOLON, SLASH, STAR,

	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	BIT_AND, LOG_AND,
	BIT_OR, LOG_OR,

	IDENTIFIER, STRING, NUMBER,
	IF, ELSE, WHILE, FOR,
	RETURN, VAR, FUNC, THIS,
	LIT_TRUE, LIT_FALSE, LIT_NULL,
	EOF_TOKEN
};

inline std::ostream& operator<<(std::ostream& os, TokenType type) {
	switch (type) {
		case LEFT_PAREN: os << "("; break;
		case RIGHT_PAREN: os << ")"; break;
		case LEFT_BRACE: os << "{"; break;
		case RIGHT_BRACE: os << "}"; break;
		case COMMA: os << ","; break;
		case DOT: os << "."; break;
		case MINUS: os << "-"; break;
		case PLUS: os << "+"; break;
		case SEMICOLON: os << ";"; break;
		case STAR: os << "*"; break;
		case SLASH: os << "/"; break;
		case BANG: os << "!"; break;
		case BANG_EQUAL: os << "!="; break;
		case EQUAL: os << "="; break;
		case EQUAL_EQUAL: os << "=="; break;
		case GREATER: os << ">"; break;
		case GREATER_EQUAL: os << ">="; break;
		case LESS: os << "<"; break;
		case LESS_EQUAL: os << ">="; break;
		case BIT_AND: os << "&"; break;
		case LOG_AND: os << "&&"; break;
		case BIT_OR: os << "|"; break;
		case LOG_OR: os << "||"; break;
		case IDENTIFIER: os << "id"; break;
		case STRING: os << "string"; break;
		case NUMBER: os << "number"; break;
		case IF: os << "if"; break;
		case ELSE: os << "else"; break;
		case WHILE: os << "while"; break;
		case FOR: os << "for"; break;
		case RETURN: os << "return"; break;
		case VAR: os << "var"; break;
		case FUNC: os << "function"; break;
		case THIS: os << "this"; break;
		case LIT_TRUE: os << "true"; break;
		case LIT_FALSE: os << "false"; break;
		case EOF_TOKEN: os << "EOF"; break;
		default: os.setstate(std::ios_base::failbit);

	}
	return os;
}

class Token {
public:
	Token(TokenType type, std::string* lexeme, void* literal, int line)
		: type(type), lexeme(lexeme), literal(literal), line(line) {}

	virtual ~Token() {
//		delete lexeme;
//		if (literal != NULL) {
////			std::cout << "Freeing memory @ " << literal << std::endl;
//			delete literal;
//		}
	}

	// don't mess with these unless you really mean it
	TokenType type;
	std::string* lexeme;
	void* literal;
	int line;
};

} /* namespace tjs */

#endif /* TOKEN_H_ */
