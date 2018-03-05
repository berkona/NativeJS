/*
 * THIS IS AN AUTO-GENERATED FILE -- DO NOT MODIFY 
 */

#ifndef AUTOGEN_Stmt_H
#define AUTOGEN_Stmt_H

#include <vector>

#include "Expr.h"
#include "Token.h"

namespace tjs {

namespace Stmt {

class Visitor;

class Stmt {
public:
	virtual ~Stmt() {}

	virtual void accept(Visitor& v) = 0;
};

class Block;
class Expression;
class Function;
class If;
class Var;
class Return;
class While;
class FunDecl	;

class Visitor {

public:
	virtual ~Visitor() {}
	virtual void visit(Block& v) = 0;
	virtual void visit(Expression& v) = 0;
	virtual void visit(Function& v) = 0;
	virtual void visit(If& v) = 0;
	virtual void visit(Var& v) = 0;
	virtual void visit(Return& v) = 0;
	virtual void visit(While& v) = 0;
	virtual void visit(FunDecl	& v) = 0;
};

class Block : public Stmt {

public:
	std::vector<Stmt*>* statements;

	Block(std::vector<Stmt*>* statements) : statements(statements) {}
	virtual ~Block() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Expression : public Stmt {

public:
	Expr::Expr* expression;

	Expression(Expr::Expr* expression) : expression(expression) {}
	virtual ~Expression() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Function : public Stmt {

public:
	Token* name;
	std::vector<Token*>* parameters;
	std::vector<Stmt*>* body;

	Function(Token* name, std::vector<Token*>* parameters, std::vector<Stmt*>* body) : name(name), parameters(parameters), body(body) {}
	virtual ~Function() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class If : public Stmt {

public:
	Expr::Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;

	If(Expr::Expr* condition, Stmt* thenBranch, Stmt* elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
	virtual ~If() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Var : public Stmt {

public:
	Token* name;
	Expr::Expr* initializer;

	Var(Token* name, Expr::Expr* initializer) : name(name), initializer(initializer) {}
	virtual ~Var() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Return : public Stmt {

public:
	Token* keyword;
	Expr::Expr* value;

	Return(Token* keyword, Expr::Expr* value) : keyword(keyword), value(value) {}
	virtual ~Return() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class While : public Stmt {

public:
	Expr::Expr* condition;
	Stmt* body;

	While(Expr::Expr* condition, Stmt* body) : condition(condition), body(body) {}
	virtual ~While() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class FunDecl : public Stmt {

public:
	Token* name;
	std::vector<std::string> params;

	FunDecl	(Token* name, std::vector<std::string> params) : name(name), params(params) {}
	virtual ~FunDecl	() {}

	void accept(Visitor& v) { v.visit(*this); };

};

}
}

#endif
