/*
 * Resolver.cpp
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#include "Resolver.h"

#include <stack>
#include <unordered_map>
#include "../util.h"
#include "../Exceptions.h"

namespace tjs {

Resolver::Resolver() {}

Resolver::~Resolver() {}

std::unordered_map<Expr::Expr*, int>* Resolver::process(Stmt::Block& mod) {
	definedVars.empty();
	scopeMap = new std::unordered_map<Expr::Expr*, int>();
	// push global scope
	definedVars.push_back(new std::unordered_map<std::string, bool> () );
	mod.accept(*this);
	definedVars.pop_back();
	return scopeMap;
}

void Resolver::visit(Stmt::Block& v) {
	for (Stmt::Stmt* s : *v.statements) {
		s->accept(*this);
	}
}

void Resolver::visit(Stmt::Function& v) {
	declare(*v.name);
	define(*v.name);
	definedVars.push_back(new std::unordered_map<std::string, bool> () );
	for (Token* param : *v.parameters) {
		declare(*param);
		define(*param);
	}
	for (Stmt::Stmt* stmt : *v.body) {
		stmt->accept(*this);
	}
	definedVars.pop_back();
}

void Resolver::visit(Stmt::If& v) {
	v.condition->accept(*this);
	v.thenBranch->accept(*this);
	if (v.elseBranch != NULL)
		v.elseBranch->accept(*this);
}

void Resolver::visit(Stmt::While& v) {
	v.condition->accept(*this);
	v.body->accept(*this);
}

void Resolver::visit(Stmt::Var& v) {
	declare(*v.name);
	if (v.initializer != NULL) {
		v.initializer->accept(*this);
	}
	define(*v.name);
}

void Resolver::visit(Stmt::Return& v) {
	if (v.value != NULL)
		v.value->accept(*this);
}

void Resolver::visit(Stmt::Expression& v) {
	v.expression->accept(*this);
}

void Resolver::visit(Expr::Assign& v) {
	v.value->accept(*this);
	resolveLocal(&v, *v.name);
}

void Resolver::visit(Expr::Binary& v) {
	v.left->accept(*this);
	v.right->accept(*this);
}

void Resolver::visit(Expr::Grouping& v) {
	v.expression->accept(*this);
}

void Resolver::visit(Expr::Call& v) {
	v.callee->accept(*this);
	for (Expr::Expr* arg : *v.arguments) {
		arg->accept(*this);
	}
}

void Resolver::visit(Expr::Get& v) {
	v.object->accept(*this);
}

void Resolver::visit(Expr::Literal& v) {
	// noop
}

void Resolver::visit(Expr::Unary& v) {
	v.right->accept(*this);
}

void Resolver::visit(Expr::Set& v) {
	v.object->accept(*this);
	v.value->accept(*this);
}

//void Resolver::visit(Expr::Logical& v) {
//	v.left->accept(*this);
//	v.right->accept(*this);
//}

void Resolver::visit(Expr::Variable& v) {
	auto scope = definedVars.back();
	auto result = scope->find(*v.name->lexeme);
	bool isDeclared = result != scope->end();
	bool isDefined = isDeclared && result->second;
	if (isDeclared && !isDefined) {
		error(*v.name, "Cannot read local variable in its own initializer.");
	}
	resolveLocal(&v, *v.name);
}

void Resolver::visit(Stmt::FunDecl& v) {
	error(*v.name, "unsupported");
}

void Resolver::declare(Token name) {
	auto scope = definedVars.back();
	auto result = scope->insert({ *name.lexeme, false });
	if (!result.second) {
		error(name, "Variable with this name already declared in this scope.");
	}
}

void Resolver::define(Token name) {
	auto scope = definedVars.back();
	(*scope)[*name.lexeme] = true;
}

void Resolver::resolveLocal(Expr::Expr* expr, Token name) {
	std::string varName = *name.lexeme;
	for (int i = definedVars.size()-1; i >= 0; i--) {
		auto scope = definedVars[i];
		if (scope->find(varName) != scope->end()) {
			scopeMap->insert({ expr, definedVars.size() - i - 1});
			return;
		}
	}

	//warn(name, "Variable \"" + varName + "\" is not declared in any enclosing scopes");
}

} /* namespace tjs */
