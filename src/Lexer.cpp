/*
 * Lexer.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "Lexer.h"

namespace tjs {

void Lexer::init() {
	prev = NULL;
	next = NULL;
	keywords.insert({
		{"if", TokenType::IF},
		{"else", TokenType::ELSE},
		{"while", TokenType::WHILE},
		{"for", TokenType::FOR},
		{"return", TokenType::RETURN},
		{"var", TokenType::VAR},
		{"function", TokenType::FUNC},
		{"this", TokenType::THIS},
		{"true", TokenType::LIT_TRUE},
		{"false", TokenType::LIT_FALSE},
		{"null", TokenType::LIT_NULL},
	});
}

Lexer::~Lexer() {
//	std::cout << "Tearing down lexer" << std::endl;
	delete prev;
	// this can happen if we end up here after an exception in advance
	if (next != prev) delete next;
}

void Lexer::setSource(std::string src) {
	source = src;
	tokenStart = 0;
	tokenCurr = 0;
	line = 1;
//	delete prev;
	if (next != prev) delete next;
	prev = NULL;
	next = scanToken();
}

Token* Lexer::advance() {
	// TODO is not deleting here a memory leak??
//	delete prev;
	prev = next;
	next = scanToken();
	return prev;
}

/**
 * Get last token in the stream
 */
Token* Lexer::previous() {
	return prev;
}

/**
 * Get current token in the stream without advancing
 */
Token* Lexer::peek() {
	return next;
}

/**
 * Returns whether the current token is the EOF
 */
bool Lexer::isAtEnd() {
	return next->type == TokenType::EOF_TOKEN;
}

Token* Lexer::scanToken() {
	while (!endOfSource()) {
		tokenStart = tokenCurr;
		char c = nextChar();
		switch (c) {
			case '(': return createToken(TokenType::LEFT_PAREN, NULL);
			case ')': return createToken(TokenType::RIGHT_PAREN, NULL);
			case '{': return createToken(TokenType::LEFT_BRACE, NULL);
			case '}': return createToken(TokenType::RIGHT_BRACE, NULL);
			case ',': return createToken(TokenType::COMMA, NULL);
			case '.': return createToken(TokenType::DOT, NULL);
			case '-': return createToken(TokenType::MINUS, NULL);
			case '+': return createToken(TokenType::PLUS, NULL);
			case ';': return createToken(TokenType::SEMICOLON, NULL);
			case '*': return createToken(TokenType::STAR, NULL);
			case '!': return createToken(matchChar('=') ? TokenType::BANG_EQUAL : TokenType::BANG, NULL);
			case '=': return createToken(matchChar('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL, NULL);
			case '>': return createToken(matchChar('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER, NULL);
			case '<': return createToken(matchChar('=') ? TokenType::LESS_EQUAL : TokenType::LESS, NULL);
			case '&': return createToken(matchChar('&') ? TokenType::LOG_AND : TokenType::BIT_AND, NULL);
			case '|': return createToken(matchChar('|') ? TokenType::LOG_OR : TokenType::BIT_OR, NULL);
			case '/':
				if (matchChar('/')) {
					while (peekChar() != '\n' && !endOfSource())
						nextChar();
					break;
				} else if (matchChar('*')) {
					while (!endOfSource()) {
						if (peekChar() == '*' && peekNext() == '/') {
							break;
						} else if (peekChar() == '\n') {
							line++;
							nextChar();
						} else {
							nextChar();
						}
					}
					if (endOfSource()) {
						throw std::runtime_error("Unterminated block comment");
					} else {
						nextChar();
						nextChar();
						break;
					}
				} else {
					return createToken(TokenType::SLASH, NULL);
				}
			case ' ':
			case '\t':
			case '\r':
				break;
			case '\n':
				line++;
				break;
			case '"':
				return string();
			default:
				if (isDigit(c)) {
					return number();
				} else if (isAlpha(c)) {
					return identifier();
				} else {
					throw std::runtime_error("Unexpected character");
				}
		}
	}
	return createToken(TokenType::EOF_TOKEN, NULL);
}

Token* Lexer::createToken(TokenType type, void* literal) {
	std::string text = source.substr(tokenStart, tokenCurr - tokenStart);
	return new Token(type, new std::string(text), literal, line);
}

Token* Lexer::string() {
	while (peekChar() != '"' && !endOfSource()) {
		if (peekChar() == '\n') line++;
		nextChar();
	}

	if (endOfSource()) {
		throw std::runtime_error("Unterminated string.");
	}
	// consume the other '"'
	nextChar();

	std::string substr = source.substr(tokenStart + 1, (tokenCurr - tokenStart) - 2);
	std::string* literal = new std::string(substr);
	return createToken(TokenType::STRING, literal);
}

Token* Lexer::number() {
	while (isDigit(peekChar())) nextChar();

	if (peekChar() == '.' && isDigit(peekNext())) {
		nextChar();
		while (isDigit(peekChar())) nextChar();
	}

	double value = std::strtod(source.substr(tokenStart, tokenCurr - tokenStart).c_str(), NULL);
	double* lit = new double(value);
	return createToken(TokenType::NUMBER, lit);
}

Token* Lexer::identifier() {
	while (isAlnum(peekChar())) nextChar();
	std::string text = source.substr(tokenStart, tokenCurr - tokenStart);
	auto type = keywords.find(text);
	if (type != keywords.end()) {
		return createToken(type->second, NULL);
	} else {
		std::string* name = new std::string(text);
		return createToken(TokenType::IDENTIFIER, name);
	}
}

char Lexer::nextChar() {
	tokenCurr++;
	return source[tokenCurr - 1];
}

char Lexer::peekChar() {
	if (endOfSource()) return '\0';
	return source[tokenCurr];
}

char Lexer::peekNext() {
	if (tokenCurr + 1 >= source.length()) return '\0';
	return source[tokenCurr + 1];
}

bool Lexer::isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c == '_');
}

bool Lexer::isAlnum(char c) {
	return isDigit(c) || isAlpha(c);
}

bool Lexer::matchChar(char expected) {
	if (endOfSource() || source[tokenCurr] != expected)
		return false;
	tokenCurr++;
	return true;
}

bool Lexer::endOfSource() {
	return tokenCurr >= source.length();
}

} /* namespace tjs */
