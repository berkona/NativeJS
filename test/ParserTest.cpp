/*
 * ParserTest.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: jon
 */

#include "cute.h"

#include "../src/Parser.h"
#include "../src/Stmt.h"
#include "../src/Expr.h"
#include "../src/ASTPrinter.h"

using namespace std;
using namespace tjs;

static void assertLitBool(string source, bool expected) {
	Lexer lex(source);
	Parser p;
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());

	Stmt::Stmt* stmt = (*(blk->statements))[0];
	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));

	Stmt::Expression* expr = dynamic_cast<Stmt::Expression*>(stmt);
	ASSERT(dynamic_cast<Expr::Literal*>(expr->expression));

	Expr::Literal* lit = dynamic_cast<Expr::Literal*>(expr->expression);
	ASSERT_EQUAL(expected, *((bool*)lit->value));
}

void Parser_boolStmt() {
	// test literal bool stmts
	assertLitBool("true;", true);
	assertLitBool("false;", false);
}

void Parser_nullStmt() {
	Lexer lex("null;");
	Parser p;
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());
	Stmt::Stmt* stmt = (*(blk->statements))[0];

	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));
	ASSERT(dynamic_cast<Expr::Literal*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	));

	Expr::Literal* lit = dynamic_cast<Expr::Literal*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	);
	ASSERT(lit->value == NULL);
}

void Parser_stringStmt() {
	// test literal string stmt
	Lexer lex("\"foo bar\";");
	Parser p;
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());
	Stmt::Stmt* stmt = (*(blk->statements))[0];
	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));

	ASSERT(dynamic_cast<Expr::Literal*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	));

	Expr::Literal* lit = dynamic_cast<Expr::Literal*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	);

	ASSERT_EQUAL("foo bar", *((string*)lit->value));
}

void Parser_numberStmt() {
	// test literal number stmt
	Lexer lex("2.7;");
	Parser p;
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());
	Stmt::Stmt* stmt = (*(blk->statements))[0];
	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));

	ASSERT(dynamic_cast<Expr::Literal*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	));

	Expr::Literal* lit = dynamic_cast<Expr::Literal*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	);

	ASSERT_EQUAL(2.7, *((double*)lit->value));
}

void Parser_idStmt() {
	Lexer lex("foobar;");
	Parser p;
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());
	Stmt::Stmt* stmt = (*(blk->statements))[0];

	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));

	ASSERT(dynamic_cast<Expr::Variable*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	));

	Expr::Variable* lit = dynamic_cast<Expr::Variable*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	);

	Token* tok = (Token*)(lit->name);
	ASSERT_EQUAL("foobar", *((string*)tok->literal));
}

void Parser_groupingStmt() {
	// empty grouping
	Lexer lex;
	Parser p;

	// grouping w/ primary
	lex.setSource("(foobar);");
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());
	Stmt::Stmt* stmt = (*(blk->statements))[0];
	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));

	ASSERT(dynamic_cast<Expr::Grouping*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	));

	Expr::Grouping* grp = dynamic_cast<Expr::Grouping*>(
			dynamic_cast<Stmt::Expression*>(stmt)->expression
	);

	ASSERT(dynamic_cast<Expr::Variable*>(grp->expression));

	Expr::Variable* lit = dynamic_cast<Expr::Variable*>(grp->expression);

	Token* tok = (Token*)(lit->name);
	ASSERT_EQUAL("foobar", *((string*)tok->literal));

	lex.setSource("();");
	p.parse(&lex);
	ASSERT(p.hadError());
}

void Parser_complexExpr() {
	Lexer lex("3 * 1 <= 3;");
	Parser p;
	auto r = p.parse(&lex);
	ASSERT(!p.hadError());
	ASSERT(dynamic_cast<Stmt::Block*>(r));

	Stmt::Block* blk = dynamic_cast<Stmt::Block*>(r);
	ASSERT_EQUAL(1, blk->statements->size());
	Stmt::Stmt* stmt = (*(blk->statements))[0];
	ASSERT(dynamic_cast<Stmt::Expression*>(stmt));

	Stmt::Expression* expr = dynamic_cast<Stmt::Expression*>(stmt);
	ASTPrinter printer (cout);
	printer.visit(*expr);

	// TODO
}


cute::suite make_suite_ParserTest() {
	cute::suite s;
	s.push_back(CUTE(Parser_boolStmt));
	s.push_back(CUTE(Parser_nullStmt));
	s.push_back(CUTE(Parser_stringStmt));
	s.push_back(CUTE(Parser_numberStmt));
	s.push_back(CUTE(Parser_idStmt));
	s.push_back(CUTE(Parser_groupingStmt));
	s.push_back(CUTE(Parser_complexExpr));
	return s;
}
