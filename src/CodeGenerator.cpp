/*
 * CodeGenerator.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: jon
 */

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "CodeGenerator.h"
#include "Exceptions.h"
#include "JSType.h"

#include "util.h"

#include "../libJS/libJS.h"

using namespace std;

namespace tjs {

// helper method for pretty identation
static void ident(ostream& os, int level) {
//	while (level-- > 0) os << '\t';
}

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

static string deref(JSType t, string expr) {
	switch (t) {
	case JSType::OBJECT:
		return "*(" + expr + ")";
	default:
		return expr;
	}
}

static string toBool(JSType t, string expr) {
	switch (t) {
		case JSType::OBJECT:
		case JSType::FUNCTION:
		case JSType::NIL:
		case JSType::UNDEFINED:
			return "(" + expr + ")->toBool()";
		case JSType::STRING:
			return "(" + expr + ").size() > 0";
		case JSType::NUMBER:
			return "Num((" + expr + "), NULL).toBool()";
		default:
			return expr;
	}
}

static string includeToken = "//{TJS_INCLUDES}";
static string gblStmtToken = "//{TJS_GENERATED_CODE}";

static string templateFileName = "template.txt";

CodeGenerator::CodeGenerator() {}

CodeGenerator::~CodeGenerator() {}

std::string CodeGenerator::generate(ModData& mod) {
	indentLevel = 0;
	value = "";
	modData = mod;

	modData.ast->accept(*this);

	stringstream ss {};

	ifstream tmplIFS { templateFileName };
	string mainTemplate( ( istreambuf_iterator<char>(tmplIFS) ), ( istreambuf_iterator<char>() ) );

	string includeStr = "";
	auto includes = libJSIncludes();
	for (string s : includes) {
		includeStr += "#include " + s + "\n";
	}

	string::size_type incl_pos = mainTemplate.find(includeToken);
	mainTemplate = mainTemplate.replace(incl_pos, includeToken.size(), includeStr);

	string::size_type r_pos = mainTemplate.find(gblStmtToken);
	mainTemplate = mainTemplate.replace(r_pos, gblStmtToken.size(), value);

	ss << mainTemplate << endl;
	return ss.str();
}

// stmt generators

void CodeGenerator::visit(Stmt::Block &v) {
	//indentLevel++;
	stringstream ss;
	for (Stmt::Stmt* stmt : *v.statements) {
		stmt->accept(*this);
		//ident(ss, indentLevel);
		ss << value;
	}
	//indentLevel--;
	value = ss.str();
}

void CodeGenerator::visit(Stmt::Var &v) {
	string varName = *v.name->lexeme;

	ExprData initializer = {
		"undefined",
		"",
		JSType::UNDEFINED,
	};

	if (v.initializer != NULL) {
		initializer = generate(*v.initializer);
	}

	stringstream ss;
	if (initializer.var.size() > 0) {
		ss << initializer.code;
		ss << "scope->set(\"" << varName << "\", " << toObject(initializer.type, initializer.var) << ");" << endl;
	} else {
		ss << "scope->set(\"" << varName << "\", " << toObject(initializer.type, initializer.code) << ");" << endl;
	}
	value = ss.str();
}

void CodeGenerator::visit(Stmt::Expression &v) {
	ExprData expr = generate(*v.expression);
	stringstream ss;
	ss << expr.code;
	if (expr.var.size() == 0)
		ss << ';' << endl;
	value = ss.str();
}

void CodeGenerator::visit(Stmt::If &v) {
	ExprData cond = generate(*v.condition);

	string inner = (cond.var.size() > 0) ? cond.var : cond.code;
	stringstream ss;
	ss << "if (" << toBool(cond.type, inner) << ") {" << endl;

	indentLevel++;
	v.thenBranch->accept(*this);
	ident(ss, indentLevel);
	ss << value;
	indentLevel--;
	ss << "}" << endl;

	if (v.elseBranch != NULL) {
		ss << "else {" << endl;
		indentLevel++;
		v.elseBranch->accept(*this);
		ident(ss, indentLevel);
		ss << value;
		indentLevel--;
		ss << "}" << endl;
	}

	value = ss.str();
}

void CodeGenerator::visit(Stmt::Return &v) {
	ExprData expr = generate(*v.value);
	stringstream ss;
	if (expr.var.size() > 0) {
		ss << expr.code;
		ss << "return " << toObject(expr.type, expr.var) << ";" << endl;
	} else {
		ss << "return " << toObject(expr.type, expr.code) << ";" << endl;
	}
	value = ss.str();
}

void CodeGenerator::visit(Stmt::While &v) {
	ExprData cond = generate(*v.condition);
	string condCode = (cond.var.size() > 0) ? cond.var : cond.code;
	stringstream ss;
	ss << "while (" + toBool(cond.type, condCode) + ") {" << endl;

	indentLevel++;
	v.body->accept(*this);
	ident(ss, indentLevel);
	ss << value;
	indentLevel--;
	ident(ss, indentLevel);
	ss << "}" << endl;
	value = ss.str();
}

void CodeGenerator::visit(Stmt::Function &v) {
	string fnName = *v.name->lexeme;

	stringstream ss;
	ss << "JSObject* " << fnName << "(Scope* scope) {" << endl;
	indentLevel++;

	for (Stmt::Stmt* s : *v.body) {
		s->accept(*this);
		ident(ss, indentLevel);
		ss << value;
	}

	// prevent a ud2 error, by forcing a return before end of body
	ss << "return NULL;" << endl;
	indentLevel--;
	ss << "}" << endl;
	value = ss.str();
}

void CodeGenerator::visit(Stmt::FunDecl& v) {
	stringstream ss;
	ss << "scope->set(\"" << *v.name->lexeme << "\", ";
	ss << " new Function(&" << *v.name->lexeme << ", scope, ";
	ss << "new std::vector<std::string> {";
	// param list
	for (int i = 0; i < v.params.size(); i++) {
		ss << '"' << v.params[i] << '"' << ',' << ' ';
	}
	ss << "} )" << ");" << endl;
	value = ss.str();
}

ExprData CodeGenerator::generate(Expr::Expr& root) {
	root.accept(*this);
	return exprValue;
}

void CodeGenerator::visit(Expr::Assign &v) {
	v.value->accept(*this);

	string exprCode = (exprValue.var.size() > 0) ? exprValue.var : exprValue.code;
	exprCode = toObject(exprValue.type, exprCode);

	exprValue = {
		"",
		"scope->set(\"" + *(v.name->lexeme) + "\", " + exprCode + ")",
		JSType::UNDEFINED,
	};
}

void CodeGenerator::visit(Expr::Binary &v) {
	v.left->accept(*this);

	stringstream ss;
	string leftCode;
	JSType leftType = exprValue.type;
	if (exprValue.var.size() > 0) {
		ss << exprValue.code;
		leftCode = exprValue.var;
	} else {
		leftCode = exprValue.code;
	}

	v.right->accept(*this);
	string rightCode;
	JSType rightType = exprValue.type;
	if (exprValue.var.size() > 0) {
		ss << exprValue.code;
		rightCode = exprValue.var;
	} else {
		rightCode = exprValue.code;
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

	string expr = op + "((" + deref(leftType, leftCode) + "), (" + deref(rightType, rightCode) + "))";

	exprValue = {
		expr,
		ss.str(),
		rType,
	};
}

void CodeGenerator::visit(Expr::Call &v) {
	//throw runtime_error("unsuported op");
	v.callee->accept(*this);

	stringstream ss;
	string calleeVar;
	if (exprValue.var.size() > 0) {
		ss << exprValue.code;
		calleeVar = exprValue.var;
	} else {
		calleeVar = tempID();
		ss << "JSObject* " << calleeVar << " = " << exprValue.code << ";" << endl;
	}

	int n = (*(v.arguments)).size();
	std::vector<string> paramVars;
	for (Expr::Expr* paramExpr : *v.arguments) {
		paramExpr->accept(*this);

		string paramVar;
		if (exprValue.var.size() > 0) {
			ss << exprValue.code;
			paramVar = toObject(exprValue.type, exprValue.var);
		}  else {
			paramVar = tempID();
			ss << "JSObject* " << paramVar << " = " << toObject(exprValue.type, exprValue.code) << ";" << endl;
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
	ss << fnVar << "->call(" << n << ", " << argList << ");" << endl;
	exprValue = {
		resultVar,
		ss.str(),
		JSType::OBJECT,
	};
}

void CodeGenerator::visit(Expr::Get &v) {
	v.object->accept(*this);
	string objCode = (exprValue.var.size() > 0) ? exprValue.var : exprValue.code;
	exprValue = {
		"",
		"(" + objCode + "->get(\"" + *v.name->lexeme + "\"))",
		JSType::OBJECT,
	};
}

void CodeGenerator::visit(Expr::Grouping &v) {
	v.expression->accept(*this);
}

void CodeGenerator::visit(Expr::Literal &v) {
	stringstream ss;
	switch (v.type) {
		case JSType::BOOL:
			ss << *((bool*)v.value);
			break;
		case JSType::NUMBER:
			ss << *((double*)v.value);
			break;
		case JSType::STRING:
			ss << '"' << *((string*)v.value) << '"';
			break;
		default:
			throw CompileError("unsupported literal type");
	}

	exprValue = {
		"",
		ss.str(),
		v.type,
	};
}

void CodeGenerator::visit(Expr::Set &v) {
	throw CompileError("unsupported set operation", *v.name);
}

void CodeGenerator::visit(Expr::Unary &v) {
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

	string inner = (exprValue.var.size() > 0) ? exprValue.var : exprValue.code;
	exprValue = {
		"",
		op + '(' + inner + ')',
		rType,
	};
}

void CodeGenerator::visit(Expr::Variable &v) {
	int depth = (*modData.scopes)[&v];
	stringstream ss;
	ss << "scope->get(\"" << *v.name->lexeme << "\", " << depth << ")";
	exprValue = {
		"",
		ss.str(),
		JSType::OBJECT,
	};
}

} /* namespace tjs */
