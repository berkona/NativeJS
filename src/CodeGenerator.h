/*
 * CodeGenerator.h
 *
 *  Created on: Feb 24, 2018
 *      Author: jon
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include <stack>
#include <vector>

#include "Expr.h"
#include "Stmt.h"
#include "JSType.h"
#include "preprocessors/Preprocessor.h"

namespace tjs {

struct ExprData {
	std::string var;
	std::string code;
	JSType type;
};

class CodeGenerator: public Stmt::Visitor, public Expr::Visitor {
public:
	CodeGenerator();
	virtual ~CodeGenerator();

	std::string generate(ModData& stmts);

	void visit(Stmt::Block& v) override;
	void visit(Stmt::Expression& v) override;
	void visit(Stmt::Function& v) override;
	void visit(Stmt::If& v) override;
	void visit(Stmt::Var& v) override;
	void visit(Stmt::Return& v) override;
	void visit(Stmt::While& v) override;
	void visit(Stmt::FunDecl& v) override;

	ExprData generate(Expr::Expr &root);

	void visit(Expr::Assign& v) override;
	void visit(Expr::Binary& v) override;
	void visit(Expr::Grouping& v) override;
	void visit(Expr::Call& v) override;
	void visit(Expr::Get& v) override;
	void visit(Expr::Literal& v) override;
	void visit(Expr::Unary& v) override;
	void visit(Expr::Set& v) override;
	void visit(Expr::Variable& v) override;

private:
	int indentLevel = 0;
	ModData modData;

	std::string value;
	ExprData exprValue;
};

} /* namespace tjs */

#endif /* CODEGENERATOR_H_ */
