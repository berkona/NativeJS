/*
 * ExprGenerator.h
 *
 *  Created on: Feb 25, 2018
 *      Author: jon
 */

#ifndef EXPRGENERATOR_H_
#define EXPRGENERATOR_H_

#include <sstream>
#include <string>

#include "Expr.h"
#include "JSType.h"

namespace tjs {

struct ExprData {
	std::string var;
	std::string code;
	JSType type;
};

class ExprGenerator : public Expr::Visitor {
public:
	ExprGenerator();
	virtual ~ExprGenerator();

	ExprData generate(Expr::Expr &root);

	void visit(Expr::Assign& v) override;
	void visit(Expr::Binary& v) override;
	void visit(Expr::Grouping& v) override;
	void visit(Expr::Call& v) override;
	void visit(Expr::Get& v) override;
	void visit(Expr::Literal& v) override;
	void visit(Expr::Unary& v) override;
	void visit(Expr::Set& v) override;
	//void visit(Expr::Logical& v) override;
	void visit(Expr::Variable& v) override;

private:
	ExprData value;
};

} /* namespace tjs */

#endif /* EXPRGENERATOR_H_ */
