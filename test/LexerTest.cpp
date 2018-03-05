/*
 * LexerTest.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#include <array>

#include "cute.h"

#include "LexerTest.h"
#include "../src/Lexer.h"
#include "../src/Token.h"

using namespace std;
using namespace tjs;

void Lexer_emptyFile() {
	string empty = "";
	Lexer emptyLexer(empty);
	ASSERT(emptyLexer.isAtEnd());
	ASSERT(emptyLexer.peek()->type == TokenType::EOF_TOKEN);
	ASSERT(emptyLexer.advance()->type == TokenType::EOF_TOKEN);
	ASSERT(emptyLexer.peek()->type == TokenType::EOF_TOKEN);
	ASSERT(emptyLexer.previous()->type == TokenType::EOF_TOKEN);
	ASSERT(emptyLexer.isAtEnd());
}

static void checkSequence(string str, TokenType* expected, int n) {
	Lexer lex(str);
	for (int i = 0; i < n; i++) {
		Token* next = lex.advance();
		ASSERT_EQUAL(expected[i], next->type);
	}
}

void Lexer_singleTokens() {
	string single_toks = "(){},.-+;*/=!><&|";
	std::array<TokenType, 18> expected = { {
			TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN,
			TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
			TokenType::COMMA, TokenType::DOT,
			TokenType::MINUS, TokenType::PLUS,
			TokenType::SEMICOLON, TokenType::STAR,
			TokenType::SLASH, TokenType::EQUAL,
			TokenType::BANG, TokenType::GREATER,
			TokenType::LESS, TokenType::BIT_AND,
			TokenType::BIT_OR, TokenType::EOF_TOKEN
	} };

	checkSequence(single_toks, expected.data(), expected.size());

	// TODO check some exotic token sequences
}

void Lexer_complexTokens() {
	string tokens = "!= == >= <= && || ";
	Lexer lex(tokens);
	std::array<TokenType, 7> expected = { {
			TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL,
			TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL,
			TokenType::LOG_AND, TokenType::LOG_OR, TokenType::EOF_TOKEN
	} };

	checkSequence(tokens, expected.data(), expected.size());

	// TODO check some exotic token sequences
}

void Lexer_keywords() {
	string tokens = "if else while for return function this true false";
	std::array<TokenType, 10> expected = { {
			TokenType::IF, TokenType::ELSE,
			TokenType::WHILE, TokenType::FOR,
			TokenType::RETURN, TokenType::FUNC, TokenType::THIS,
			TokenType::LIT_TRUE, TokenType::LIT_FALSE,
			TokenType::EOF_TOKEN,
	} };

	checkSequence(tokens, expected.data(), expected.size());

	// TODO is JS case sensitive for keywords?
}

void Lexer_ignoreComments() {
	// line comments
	string tokens = "foo // this is a comment\n bar";
	Lexer lex(tokens);
	ASSERT_EQUAL(TokenType::IDENTIFIER, lex.advance()->type);
	ASSERT_EQUAL(TokenType::IDENTIFIER, lex.advance()->type);
	ASSERT(lex.isAtEnd());
	ASSERT_EQUAL(TokenType::EOF_TOKEN, lex.peek()->type);

	// line comment terminated by EOF
	tokens = "foo // this is a comment";
	Lexer lexLineEOF(tokens);
	ASSERT_EQUAL(TokenType::IDENTIFIER, lexLineEOF.advance()->type);
	ASSERT(lexLineEOF.isAtEnd());
	ASSERT_EQUAL(TokenType::EOF_TOKEN, lexLineEOF.peek()->type);


	// in-line block comments
	tokens = "foo /* this is a comment */ bar";
	Lexer lex2(tokens);
	ASSERT_EQUAL(TokenType::IDENTIFIER, lex2.advance()->type);
	ASSERT(!lex2.isAtEnd());
	ASSERT_EQUAL(TokenType::IDENTIFIER, lex2.advance()->type);
	ASSERT(lex2.isAtEnd());
	ASSERT_EQUAL(TokenType::EOF_TOKEN, lex2.peek()->type);

	// multiline block comments
	tokens = "foo /* this \n is \n a \n multiple \n line \n comment */ bar";
	Lexer lex3(tokens);
	ASSERT_EQUAL(TokenType::IDENTIFIER, lex3.advance()->type);
	ASSERT(!lex3.isAtEnd());
	ASSERT_EQUAL(TokenType::IDENTIFIER, lex3.advance()->type);
	ASSERT(lex3.isAtEnd());
	ASSERT_EQUAL(TokenType::EOF_TOKEN, lex3.peek()->type);

	// check unterminated block comments
	tokens = "foo /* this is a malformed comment";
	Lexer lex4(tokens);
	ASSERT_THROWS(lex4.advance(), std::exception);
}

void Lexer_string() {
	string source = "\"foo\" \"bar\"";
	Lexer lex(source);
	Token* a = lex.advance();

	ASSERT_EQUAL(TokenType::STRING, a->type);
	ASSERT_EQUAL("\"foo\"", *a->lexeme);
	ASSERT_EQUAL("foo", *((string*) a->literal ));

	Token* b = lex.advance();
	ASSERT_EQUAL(TokenType::STRING, b->type);
	ASSERT_EQUAL("\"bar\"", *b->lexeme);
	ASSERT_EQUAL("bar", *((string*) b->literal ));

	lex.advance();
	ASSERT(lex.isAtEnd());

	// unterminated string
	source = "foo \"bad string";
	Lexer badLex(source);
	ASSERT_THROWS(badLex.advance(), std::exception);
}

void Lexer_number() {
	string source = "0 3.14";
	Lexer lex(source);
	Token* num = lex.advance();

	ASSERT_EQUAL(TokenType::NUMBER, num->type);
	ASSERT_EQUAL(0.0, *((double*)num->literal));

	num = lex.advance();
	ASSERT_EQUAL(TokenType::NUMBER, num->type);
	ASSERT_EQUAL(3.14, *((double*)num->literal));
}

void Lexer_identifier() {
	string source = "foo foo_bar _foobar";
	Lexer lex(source);
	Token* id = lex.advance();

	ASSERT_EQUAL(TokenType::IDENTIFIER, id->type);
	ASSERT_EQUAL("foo", *((string*) id->literal ));

	id = lex.advance();
	ASSERT_EQUAL(TokenType::IDENTIFIER, id->type);
	ASSERT_EQUAL("foo_bar", *((string*) id->literal ));

	id = lex.advance();
	ASSERT_EQUAL(TokenType::IDENTIFIER, id->type);
	ASSERT_EQUAL("_foobar", *((string*) id->literal ));
}

cute::suite make_suite_LexerTest(){
	cute::suite s;
	s.push_back(CUTE(Lexer_emptyFile));
	s.push_back(CUTE(Lexer_singleTokens));
	s.push_back(CUTE(Lexer_complexTokens));
	s.push_back(CUTE(Lexer_keywords));
	s.push_back(CUTE(Lexer_ignoreComments));
	s.push_back(CUTE(Lexer_string));
	s.push_back(CUTE(Lexer_number));
	s.push_back(CUTE(Lexer_identifier));
	return s;
}
