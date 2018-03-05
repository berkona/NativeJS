/*
 * Aggregator.cpp
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#include <stdexcept>

#include "Aggregator.h"

#include "../util.h"

namespace tjs {

Stmt::Block* Aggregator::aggregate(Stmt::Block& mod) {
	currentLevel = 0;
	globalStatements = {};
	module = new Stmt::Block(new std::vector<Stmt::Stmt*>());

	mod.accept(*this);

	module->statements->push_back(
			new Stmt::Function(
					new Token(TokenType::FUNC, new std::string("__main__"), NULL, -1),
					new std::vector<Token*>(),
					new std::vector<Stmt::Stmt*>(globalStatements)
			)
	);

	return module;
}

void Aggregator::visit(Stmt::Block& v) {
	for (Stmt::Stmt* s : *v.statements) {
		s->accept(*this);
	}
}

void Aggregator::visit(Stmt::Function& v) {
	module->statements->push_back(&v);
	std::vector<std::string> paramList;
	for (Token* t : *v.parameters) {
		paramList.push_back(*t->lexeme);
	}
	globalStatements.push_back(new Stmt::FunDecl(v.name, paramList));
}

void Aggregator::visit(Stmt::Expression& v) {
	globalStatements.push_back(&v);
}

void Aggregator::visit(Stmt::If& v) {
	globalStatements.push_back(&v);
}

void Aggregator::visit(Stmt::Var& v) {
	globalStatements.push_back(&v);
}

void Aggregator::visit(Stmt::While& v) {
	globalStatements.push_back(&v);
}

void Aggregator::visit(Stmt::Return& v) {
	error(*v.keyword, "Cannot return from global scope");
}

void Aggregator::visit(Stmt::FunDecl& v) {
	error(*v.name, "unsupported statement type");
}

} /* namespace tjs */
