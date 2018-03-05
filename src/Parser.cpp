/*
 * Parser.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#include "Parser.h"

// internal deps
#include "Stmt.h"
#include "Expr.h"
#include "JSType.h"
#include "util.h"
#include "Exceptions.h"

using namespace std;

namespace tjs {

Parser::Parser() {
	lexer = NULL;
	_hadError = false;
}

Parser::~Parser() {
	lexer = NULL;
}

Stmt::Block* Parser::parse(Lexer* l) {
	lexer = l;
	_hadError = false;

	std::vector<Stmt::Stmt*>* statements = new std::vector<Stmt::Stmt*>();
	while (!lexer->isAtEnd()) {
		statements->push_back(declaration());
	}

	return new Stmt::Block(statements);
}

Stmt::Stmt* Parser::declaration() {
	try {
		if (match(TokenType::VAR)) return varDeclaration();
		if (match(TokenType::FUNC)) return fnDeclaration();
		return statement();
	} catch (CompileError& e) {
		cerr << e.what() << endl;
		_hadError = true;
		synchronize();
		return NULL;
	}
}

Stmt::Stmt* Parser::varDeclaration() {
	Token* id = consume(TokenType::IDENTIFIER, "Expected identifier after var");
	Expr::Expr* exp = NULL;
	if (match(TokenType::EQUAL)) {
		exp = expression();
	}
	consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
	return new Stmt::Var(id, exp);
}

Stmt::Stmt* Parser::fnDeclaration() {
	Token* name = consume(TokenType::IDENTIFIER, "Expected function name.");
	consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");
	std::vector<Token*>* parameters = new std::vector<Token*>();
	if (!check(TokenType::RIGHT_PAREN)) {
		do {
			parameters->push_back(consume(TokenType::IDENTIFIER, "Expected parameter name"));
		} while (match(TokenType::COMMA));
	}
	consume(TokenType::RIGHT_PAREN, "Expected ')' after parameter list");
	consume(TokenType::LEFT_BRACE, "Expected '{' before function body");
	std::vector<Stmt::Stmt*>* body = block();
	return new Stmt::Function(name, parameters, body);
}

std::vector<Stmt::Stmt*>* Parser::block() {
	std::vector<Stmt::Stmt*>* statements = new std::vector<Stmt::Stmt*>();

	while (!check(TokenType::RIGHT_BRACE) && !lexer->isAtEnd()) {
	  statements->push_back(declaration());
	}

	consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
	return statements;
}

Stmt::Stmt* Parser::statement() {
	if (match(TokenType::RETURN)) return returnStatement();
	if (match(TokenType::WHILE)) return whileStatement();
	if (match(TokenType::LEFT_BRACE))
		return new Stmt::Block(block());
	if (match(TokenType::FOR)) return forStatement();
	if (match(TokenType::IF)) return ifStatement();

	return expressionStatement();
}

Stmt::Stmt* Parser::returnStatement() {
	Token* keyword = lexer->previous();
	Expr::Expr* expr = NULL;
	if (!check(TokenType::SEMICOLON)) {
		expr = expression();
	}
	consume(TokenType::SEMICOLON, "Expected ';' after return value.");
	return new Stmt::Return(keyword, expr);
}

Stmt::Stmt* Parser::whileStatement() {
	consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
	Expr::Expr* condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");
	Stmt::Stmt* body = statement();
	return new Stmt::While(condition, body);
}

Stmt::Stmt* Parser::forStatement() {
	consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");
	Stmt::Stmt* initializer;
	if (match(TokenType::SEMICOLON)) {
		initializer = NULL;
	} else if (match(TokenType::VAR)) {
		initializer = varDeclaration();
	}  else {
		initializer = expressionStatement();
	}

	Expr::Expr* condition = NULL;
	if (!check(TokenType::SEMICOLON)) {
		condition = expression();
	}
	consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

	Expr::Expr* increment = NULL;
	if (!check(TokenType::SEMICOLON)) {
		increment = expression();
	}
	consume(TokenType::RIGHT_PAREN, "Expected ';' after for clauses.");

	Stmt::Stmt* body = statement();

	if (increment != NULL) {
		vector<Stmt::Stmt*>* newBody = new vector<Stmt::Stmt*>();
		newBody->push_back(body);
		newBody->push_back(new Stmt::Expression(increment));
		body = new Stmt::Block(newBody);
	}

	if (condition == NULL)
		condition = new Expr::Literal(JSType::BOOL, new bool(true));

	body = new Stmt::While(condition, body);

	if (initializer != NULL) {
		vector<Stmt::Stmt*>* newBody = new vector<Stmt::Stmt*>();
		newBody->push_back(initializer);
		newBody->push_back(body);
		body = new Stmt::Block(newBody);
	}

	return body;
}

Stmt::Stmt* Parser::ifStatement() {
	consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
	Expr::Expr* condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");

	Stmt::Stmt* thenBranch = statement();
    Stmt::Stmt* elseBranch = NULL;
    if (match(TokenType::ELSE)) {
    	elseBranch = statement();
    }
    return new Stmt::If(condition, thenBranch, elseBranch);
}

Stmt::Stmt* Parser::expressionStatement() {
	Expr::Expr* expr = expression();
	consume(TokenType::SEMICOLON, "Expected ';' after expression.");
	return new Stmt::Expression(expr);
}

Expr::Expr* Parser::expression() {
	return assignment();
}

Expr::Expr* Parser::assignment() {
	Expr::Expr* expr = or_expr();
	if (match(TokenType::EQUAL)) {
		Token* equals = lexer->previous();
		Expr::Expr* value = assignment();

		if (dynamic_cast<Expr::Variable*>(expr)) {
			Expr::Variable* v = dynamic_cast<Expr::Variable*>(expr);
			Token* name = v->name;
			return new Expr::Assign(name, v);
		} else if (Expr::Get* g = dynamic_cast<Expr::Get*>(expr)) {
			return new Expr::Set(g->object, g->name, value);
		}

		throw CompileError("Invalid assignment target.", *equals);
	}
	return expr;
}

Expr::Expr* Parser::or_expr() {
	Expr::Expr* expr = and_expr();
	while(match(TokenType::LOG_OR)) {
		Token* op = lexer->previous();
		Expr::Expr* right = and_expr();
		expr = new Expr::Binary(expr, op, right);
	}
	return expr;
}

Expr::Expr* Parser::and_expr() {
	Expr::Expr* expr = equality();
	while (match(TokenType::LOG_AND)) {
		Token* op = lexer->previous();
		Expr::Expr* right = equality();
		expr = new Expr::Binary(expr, op, right);
	}
	return expr;
}

Expr::Expr* Parser::equality() {
	Expr::Expr* expr = comparison();
	while (match(TokenType::EQUAL_EQUAL) || match(TokenType::BANG_EQUAL)) {
		Token* op = lexer->previous();
		Expr::Expr* right = comparison();
		expr = new Expr::Binary(expr, op, right);
	}
	return expr;
}

Expr::Expr* Parser::comparison() {
	Expr::Expr* expr = addition();
	while (match(TokenType::GREATER)
		|| match(TokenType::GREATER_EQUAL)
		|| match(TokenType::LESS)
		|| match(TokenType::LESS_EQUAL)) {
		Token* op = lexer->previous();
		Expr::Expr* right = addition();
		expr = new Expr::Binary(expr, op, right);
	}
	return expr;
}

Expr::Expr* Parser::addition() {
	Expr::Expr* expr = multiplication();
	while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
		Token* op = lexer->previous();
		Expr::Expr* right = multiplication();
		expr = new Expr::Binary(expr, op, right);
	}
	return expr;
}

Expr::Expr* Parser::multiplication() {
	Expr::Expr* expr = unary();
	while (match(TokenType::STAR) || match(TokenType::SLASH)) {
		Token* op = lexer->previous();
		Expr::Expr* right = unary();
		expr = new Expr::Binary(expr, op, right);
	}
	return expr;
}

Expr::Expr* Parser::unary() {
	if (match(TokenType::BANG) || match(TokenType::MINUS)) {
		Token* op = lexer->previous();
		Expr::Expr* right = unary();
		return new Expr::Unary(op, right);
	}

	return call();
}

Expr::Expr* Parser::call() {
	Expr::Expr* expr = primary();
	while (true) {
		if (match(TokenType::LEFT_PAREN)) {
			expr = finishCall(expr);
		}
		else if (match(TokenType::DOT)) {
			Token* name = consume(TokenType::IDENTIFIER, "Expected property name after '.'.");
			expr = new Expr::Get(expr, name);
		}
		else {
			break;
		}
	}
	return expr;
}

Expr::Expr* Parser::finishCall(Expr::Expr* callee) {
	std::vector<Expr::Expr*>* arguments = new std::vector<Expr::Expr*>();
	if (!check(TokenType::RIGHT_PAREN)) {
		do {
			arguments->push_back(expression());
		} while (match(TokenType::COMMA));
	}
	Token* paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
	return new Expr::Call(callee, paren, arguments);
}

Expr::Expr* Parser::primary() {
	if (match(TokenType::LIT_FALSE))
		return new Expr::Literal(JSType::BOOL, new bool(false));

	if (match(TokenType::LIT_TRUE))
		return new Expr::Literal(JSType::BOOL, new bool(true));

	if (match(TokenType::LIT_NULL))
		return new Expr::Literal(JSType::NIL, NULL);

	if (match(TokenType::STRING))
		return new Expr::Literal(JSType::STRING, new string(*((string*)lexer->previous()->literal)));

	if (match(TokenType::NUMBER))
		return new Expr::Literal(JSType::NUMBER, new double(*((double*)lexer->previous()->literal)));

	if (match(TokenType::IDENTIFIER)) {
		Token* old = lexer->previous();
		Token* cpy = new Token(old->type, old->lexeme, new string(*((string*)old->literal)), old->line);
		return new Expr::Variable(cpy);
	}

	if (match(TokenType::LEFT_PAREN)) {
		Expr::Expr* expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		return new Expr::Grouping(expr);
	}

	throw CompileError("Expected expression", *lexer->peek());
}

void Parser::synchronize() {
	lexer->advance();

	while (!lexer->isAtEnd()) {
		if (lexer->previous()->type == TokenType::SEMICOLON) return;

		switch (lexer->peek()->type) {
			case TokenType::FUNC:
			case TokenType::VAR:
			case TokenType::FOR:
			case TokenType::IF:
			case TokenType::WHILE:
			case TokenType::RETURN:
				// sane point to restart the parse
			  return;
			default: break;
		}

		lexer->advance();
	}
}

Token* Parser::consume(TokenType type, string message) {
	if (check(type)) return lexer->advance();
	throw CompileError(message, *lexer->peek());
}

bool Parser::match(TokenType expected) {
	if (check(expected)) {
		lexer->advance();
		return true;
	}
	return false;
}

bool Parser::check(TokenType expected) {
	if (lexer->isAtEnd()) return false;
	return lexer->peek()->type == expected;
}

} /* end namespace tjs */
