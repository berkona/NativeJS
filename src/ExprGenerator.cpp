/*
 * ExprGenerator.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: jon
 */

#include "ExprGenerator.h"

#include <stdexcept>

#include "util.h"
#include "Exceptions.h"

using namespace std;

namespace tjs {

static string toObject(JSType t, string expr) {
	switch (t) {
	case JSType::BOOL:
		return "new Bool((" + expr + "), NULL)";
	case JSType::NUMBER:
		return "new Num((" + expr + "), NULL)";
	case JSType::STRING:
		return "new Str((" + expr + "), NULL)";
	// other cases are null, undefined, object, and function
	// which are already JSObject's
	default:
		return expr;
	}
}

ExprGenerator::ExprGenerator() {}

ExprGenerator::~ExprGenerator() {}

ExprData ExprGenerator::generate(Expr::Expr& root) {
	root.accept(*this);
	return value;
}

void ExprGenerator::visit(Expr::Assign &v) {
	v.value->accept(*this);

	string exprCode = (value.var.size() > 0) ? value.var : value.code;
	exprCode = toObject(value.type, exprCode);

	value = {
		"",
		"scope->set(\"" + *(v.name->lexeme) + "\", " + exprCode + ")",
		JSType::UNDEFINED,
	};
}

void ExprGenerator::visit(Expr::Binary &v) {
	v.left->accept(*this);

	stringstream ss;
	string leftCode;
	if (value.var.size() > 0) {
		ss << value.code;
		leftCode = value.var;
	} else {
		leftCode = value.code;
	}

	v.right->accept(*this);
	string rightCode;
	if (value.var.size() > 0) {
		ss << value.code;
		rightCode = value.var;
	} else {
		rightCode = value.code;
	}

	string op;
	JSType rType;
	switch (v.op->type) {
		case TokenType::BANG_EQUAL:
			op = "neq";
			rType = JSType::BOOL;
			break;

		case TokenType::EQUAL_EQUAL:
			op = "eq";
			rType = JSType::BOOL;
			break;

		case TokenType::GREATER:
			op = "gt";
			rType = JSType::BOOL;
			break;

		case TokenType::GREATER_EQUAL:
			op = "gte";
			rType = JSType::BOOL;
			break;

		case TokenType::LESS:
			op = "lt";
			rType = JSType::BOOL;
			break;

		case TokenType::LESS_EQUAL:
			op = "lte";
			rType = JSType::BOOL;
			break;

		case TokenType::LOG_AND:
			op = "l_and";
			rType = JSType::BOOL;
			break;

		case TokenType::LOG_OR:
			op = "l_or";
			rType = JSType::BOOL;
			break;

		case TokenType::MINUS:
			op = "sub";
			rType = JSType::NUMBER;
			break;

		case TokenType::PLUS:
			op = "add";
			rType = JSType::NUMBER;
			break;

		case TokenType::SLASH:
			op = "div";
			rType = JSType::NUMBER;
			break;

		case TokenType::STAR:
			op = "mult";
			rType = JSType::NUMBER;
			break;

		default:
			throw CompileError("unsupported binary op", *v.op);
	}

	string expr = op + "((" + leftCode + "), (" + rightCode + "))";

	value = {
		expr,
		ss.str(),
		rType,
	};
}

void ExprGenerator::visit(Expr::Call &v) {
	//throw runtime_error("unsuported op");
	v.callee->accept(*this);

	stringstream ss;
	string calleeVar;
	if (value.var.size() > 0) {
		ss << value.code;
		calleeVar = value.var;
	} else {
		calleeVar = tempID();
		ss << "JSObject* " << calleeVar << " = " << value.code << ";" << endl;
	}

	int n = (*(v.arguments)).size();
	std::vector<string> paramVars;
	for (Expr::Expr* paramExpr : *v.arguments) {
		paramExpr->accept(*this);

		string paramVar;
		if (value.var.size() > 0) {
			ss << value.code;
			paramVar = toObject(value.type, value.var);
		}  else {
			paramVar = tempID();
			ss << "JSObject* " << paramVar << " = " << toObject(value.type, value.code) << ";" << endl;
		}

		paramVars.push_back(paramVar);
	}

	string argList = tempID();
	ss << "JSObject* " << argList << "[] = { ";
	for (string var : paramVars) {
		ss << var << ", ";
	}
	ss << "};" << endl;

	// try to cast callee to Function
	string fnVar = tempID();
	ss << "Function* " << fnVar << " = dynamic_cast<Function*>(" << calleeVar << ");" << endl;
	ss << "if (" << fnVar << " == NULL) throw runtime_error(\"Expression evaluated to an uncallable object\");" << endl;

	string resultVar = tempID();
	ss << "JSObject* " << resultVar << " = ";
	ss << "call(" << fnVar << ", " << argList << ", " << n << ");" << endl;
	value = {
		resultVar,
		ss.str(),
		JSType::OBJECT,
	};
}

void ExprGenerator::visit(Expr::Get &v) {
	v.object->accept(*this);
	string objCode = (value.var.size() > 0) ? value.var : value.code;
	value = {
		"",
		"(" + objCode + "->get(\"" + *v.name->lexeme + "\"))",
		JSType::OBJECT,
	};
}

void ExprGenerator::visit(Expr::Grouping &v) {
	v.expression->accept(*this);
}

void ExprGenerator::visit(Expr::Literal &v) {
	stringstream ss;
	switch (v.type) {
		case JSType::BOOL:
			ss << *((bool*)v.value);
			break;
		case JSType::NUMBER:
			ss << *((double*)v.value);
			break;
		case JSType::STRING:
			ss << *((string*)v.value);
			break;
		default:
			throw CompileError("unsupported literal type");
	}

	value = {
		"",
		ss.str(),
		v.type,
	};
}

void ExprGenerator::visit(Expr::Set &v) {
	throw CompileError("unsupported set operation", *v.name);
}

void ExprGenerator::visit(Expr::Unary &v) {
	v.right->accept(*this);

	string op;
	JSType rType;
	switch (v.op->type) {
		case TokenType::BANG:
			op = "not";
			rType = JSType::BOOL;
			break;
		case TokenType::MINUS:
			op = "neg";
			rType = JSType::NUMBER;
			break;
		default: throw CompileError("unsupported unary type", *v.op);
	}

	string inner = (value.var.size() > 0) ? value.var : value.code;
	value = {
		"",
		op + '(' + inner + ')',
		rType,
	};
}

void ExprGenerator::visit(Expr::Variable &v) {
	value = {
		"",
		"scope->get(\"" + *v.name->lexeme + "\")",
		JSType::OBJECT,
	};
}

} /* namespace tjs */
