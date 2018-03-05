/*
 * THIS IS AN AUTO-GENERATED FILE -- DO NOT MODIFY 
 */

#ifndef AUTOGEN_Expr_H
#define AUTOGEN_Expr_H

#include <vector>

#include "Token.h"
#include "JSType.h"

namespace tjs {

namespace Expr {

class Visitor;

class Expr {
public:
	virtual ~Expr() {}

	virtual void accept(Visitor& v) = 0;
};

class Assign;
class Binary;
class Grouping;
class Call;
class Get;
class Literal;
class Unary;
class Set;
//class Logical;
class Variable;

class Visitor {

public:
	virtual ~Visitor() {}
	virtual void visit(Assign& v) = 0;
	virtual void visit(Binary& v) = 0;
	virtual void visit(Grouping& v) = 0;
	virtual void visit(Call& v) = 0;
	virtual void visit(Get& v) = 0;
	virtual void visit(Literal& v) = 0;
	virtual void visit(Unary& v) = 0;
	virtual void visit(Set& v) = 0;
//	virtual void visit(Logical& v) = 0;
	virtual void visit(Variable& v) = 0;
};

class Assign : public Expr {

public:
	Token* name;
	Expr* value;

	Assign(Token* name, Expr* value) : name(name), value(value) {}
	virtual ~Assign() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Binary : public Expr {

public:
	Expr* left;
	Token* op;
	Expr* right;

	Binary(Expr* left, Token* op, Expr* right) : left(left), op(op), right(right) {}
	virtual ~Binary() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Grouping : public Expr {

public:
	Expr* expression;

	Grouping(Expr* expression) : expression(expression) {}
	virtual ~Grouping() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Call : public Expr {

public:
	Expr* callee;
	Token* paren;
	std::vector<Expr*>* arguments;

	Call(Expr* callee, Token* paren, std::vector<Expr*>* arguments) : callee(callee), paren(paren), arguments(arguments) {}
	virtual ~Call() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Get : public Expr {

public:
	Expr* object;
	Token* name;

	Get(Expr* object, Token* name) : object(object), name(name) {}
	virtual ~Get() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Literal : public Expr {

public:
	JSType type;
	void* value;

	Literal(JSType type, void* value) : type(type), value(value) {}
	virtual ~Literal() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Unary : public Expr {

public:
	Token* op;
	Expr* right;

	Unary(Token* op, Expr* right) : op(op), right(right) {}
	virtual ~Unary() {}

	void accept(Visitor& v) { v.visit(*this); };

};

class Set : public Expr {

public:
	Expr* object;
	Token* name;
	Expr* value;

	Set(Expr* object, Token* name, Expr* value) : object(object), name(name), value(value) {}
	virtual ~Set() {}

	void accept(Visitor& v) { v.visit(*this); };

};

//class Logical : public Expr {
//
//public:
//	Expr* left;
//	Token* op;
//	Expr* right;
//
//	Logical(Expr* left, Token* op, Expr* right) : left(left), op(op), right(right) {}
//	virtual ~Logical() {}
//
//	void accept(Visitor& v) { v.visit(*this); };
//
//};

class Variable : public Expr {

public:
	Token* name;

	Variable(Token* name) : name(name) {}
	virtual ~Variable() {}

	void accept(Visitor& v) { v.visit(*this); };

};

}
}

#endif
