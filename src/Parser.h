/*
 * Parser.h
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <sstream>
#include "Lexer.h"
#include "Stmt.h"
#include "Expr.h"

namespace tjs {

class Parser {
public:
	Parser();
	virtual ~Parser();

	Stmt::Block* parse(Lexer* lex);
	bool hadError() { return _hadError; }
private:
	bool _hadError;
	Lexer* lexer;

	// grammar rules
	Stmt::Stmt* declaration();
	Stmt::Stmt* varDeclaration();
	Stmt::Stmt* fnDeclaration();
	std::vector<Stmt::Stmt*>* block();

	Stmt::Stmt* statement();
	Stmt::Stmt* returnStatement();
	Stmt::Stmt* whileStatement();
	Stmt::Stmt* forStatement();
	Stmt::Stmt* ifStatement();
	Stmt::Stmt* expressionStatement();

	// expressions, in order of precedence
	Expr::Expr* expression();
	Expr::Expr* assignment();
	Expr::Expr* or_expr();
	Expr::Expr* and_expr();
	Expr::Expr* equality();
	Expr::Expr* comparison();
	Expr::Expr* addition();
	Expr::Expr* multiplication();
	Expr::Expr* unary();
	Expr::Expr* call();
	Expr::Expr* finishCall(Expr::Expr* expr);
	Expr::Expr* primary();

	// util functions
	void synchronize();
	Token* consume(TokenType type, std::string message);
	bool match(TokenType expected);
	bool check(TokenType expected);
};

} /* end namespace tjs */

#endif /* PARSER_H_ */
