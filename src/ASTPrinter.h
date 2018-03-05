/*
 * ASTPrinter.h
 *
 *  Created on: Feb 24, 2018
 *      Author: jon
 */

#ifndef ASTPRINTER_H_
#define ASTPRINTER_H_

#include "Expr.h"
#include "Stmt.h"
#include "JSType.h"
#include "Exceptions.h"

namespace tjs {

class ASTPrinter: public Stmt::Visitor, Expr::Visitor {
public:
	ASTPrinter(std::ostream& output) : output(output) {
		tabLevel = 0;
	}

	virtual ~ASTPrinter() {}

	void visit(Stmt::Block& v) override {
		printTabs();
		output << "Block {" << std::endl;
		tabLevel++;
		for (Stmt::Stmt* s : *v.statements) {
			s->accept(*this);
		}
		tabLevel--;
		printTabs();
		output << "}" << std::endl;
	}

	void visit(Stmt::Expression& v) override {
		printTabs();
		output << "ExprStmt (" << std::endl;
		tabLevel++;
		v.expression->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Stmt::Function& v) override {
		printTabs();
		output << "Func [" << *(v.name->lexeme) << "] (";
		for (Token* t : *v.parameters) {
			output << *(t->lexeme) << ", ";
		}
		output << ") {" << std::endl;
		tabLevel++;
		for (Stmt::Stmt* s : *v.body) {
			s->accept(*this);
		}
		tabLevel--;
		printTabs();
		output << "}" << std::endl;
	}

	void visit(Stmt::If& v) override {
		printTabs();
		output << "If (" << std::endl;

		tabLevel++;
		v.condition->accept(*this);
		tabLevel--;

		printTabs();
		output << ") {" << std::endl;

		tabLevel++;
		v.thenBranch->accept(*this);
		tabLevel--;

		printTabs();
		output << "}";

		if (v.elseBranch != NULL) {
			output << " {" << std::endl;
			tabLevel++;
			v.elseBranch->accept(*this);
			tabLevel--;
			printTabs();
			output << "}" << std::endl;
		} else {
			output << std::endl;
		}
	}

	void visit(Stmt::Var& v) override {
		printTabs();
		output << "Var [" << *(v.name->lexeme) << "] (" << std::endl;

		tabLevel++;
		if (v.initializer != NULL) v.initializer->accept(*this);
		tabLevel--;

		printTabs();
		output << ")" << std::endl;
	}

	void visit(Stmt::Return& v) override {
		printTabs();
		output << "Return [" << *(v.keyword->lexeme) << "] (" << std::endl;

		tabLevel++;
		v.value->accept(*this);
		tabLevel--;

		printTabs();
		output << ")" << std::endl;
	}

	void visit(Stmt::While& v) override {
		printTabs();
		output << "While (" << std::endl;

		tabLevel++;
		v.condition->accept(*this);
		tabLevel--;

		printTabs();
		output << ") {" << std::endl;

		tabLevel++;
		v.body->accept(*this);
		tabLevel--;

		printTabs();
		output << "}" << std::endl;
	}

	void visit(Stmt::FunDecl& v) override {
		printTabs();
		output << "FunDecl [" << *(v.name->lexeme) << "]" << std::endl;
	}

	void visit(Expr::Assign& v) override {
		printTabs();
		output << "Assign [" << *(v.name->lexeme) << "] (" << std::endl;
		tabLevel++;
		v.value->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Expr::Binary& v) override {
		printTabs();
		output << "Binary (" << std::endl;
		tabLevel++;
		v.left->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
		printTabs();
		output << v.op->type << std::endl;
		printTabs();
		output << "(" << std::endl;
		tabLevel++;
		v.right->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Expr::Grouping& v) override {
		printTabs();
		output << "Grouping (" << std::endl;
		tabLevel++;
		v.expression->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Expr::Call& v) override {
		printTabs();
		output << "Call [" << std::endl;
		tabLevel++;
		v.callee->accept(*this);
		tabLevel--;
		printTabs();
		output << "] (" << std::endl;
		for (Expr::Expr* arg : *v.arguments) {
			tabLevel++;
			arg->accept(*this);
			tabLevel--;
			printTabs();
			output << ", " << std::endl;
		}
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Expr::Get& v) override {
		printTabs();
		output << "Get [" << *(v.name->lexeme) << "] (" << std::endl;
		tabLevel++;
		v.object->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Expr::Literal& v) override {
		printTabs();
		output << "Literal ( ";
		switch (v.type) {
			case JSType::NIL:
				output << "null";
				break;
			case JSType::BOOL:
				output << *((bool*) v.value);
				break;
			case JSType::NUMBER:
				output << *((double*) v.value);
				break;
			case JSType::STRING:
				output << *((std::string*) v.value);
				break;
		}
		output << " )" << std::endl;
	}

	void visit(Expr::Unary& v) override {
		printTabs();
		output << "Unary [" << v.op->type << "] (" << std::endl;
		tabLevel++;
		v.right->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

	void visit(Expr::Set& v) override {
		printTabs();
		output << "Set [" << *(v.name->lexeme) << "] (" << std::endl;
		tabLevel++;
		v.object->accept(*this);
		tabLevel--;
		printTabs();
		output << ") (" << std::endl;
		tabLevel++;
		v.value->accept(*this);
		tabLevel--;
		printTabs();
		output << ")" << std::endl;
	}

//	void visit(Expr::Logical& v) override {
//		printTabs();
//		output << "Log (" << std::endl;
//		tabLevel++;
//		v.left->accept(*this);
//		tabLevel--;
//		printTabs();
//		output << ") " << v.op->type << " (" << std::endl;
//		tabLevel++;
//		v.right->accept(*this);
//		tabLevel--;
//		printTabs();
//		output << ")" << std::endl;
//	}

	void visit(Expr::Variable& v) override {
		printTabs();
		output << "VarExpr(" << *(v.name->lexeme) << ")" << std::endl;
	}

private:
	std::ostream& output;
	int tabLevel;

	void printTabs() {
		for (int i = 0; i < tabLevel; i++) {
			output << ' ';
		}
	}
};

} /* namespace tjs */

#endif /* ASTPRINTER_H_ */
