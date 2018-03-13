/*
 * Resolver.h
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#ifndef RESOLVER_H_
#define RESOLVER_H_

#include <stack>
#include <unordered_map>

#include "../Stmt.h"
#include "../Expr.h"

namespace tjs {

class Resolver : public Stmt::Visitor, public Expr::Visitor {
public:
	Resolver();
	virtual ~Resolver();

	std::unordered_map<Expr::Expr*, int>* process(Stmt::Block& mod);

	void visit(Stmt::Block& v) override;
	void visit(Stmt::Expression& v) override;
	void visit(Stmt::Function& v) override;
	void visit(Stmt::If& v) override;
	void visit(Stmt::Var& v) override;
	void visit(Stmt::Return& v) override;
	void visit(Stmt::While& v) override;
	void visit(Stmt::FunDecl& v) override;

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
	std::unordered_map<std::string, int>* displacementMap = NULL;
	std::unordered_map<Expr::Expr*, int>* scopeMap = NULL;
	std::vector< std::unordered_map<std::string, bool>* > definedVars;

	void declare(Token name);
	void define(Token name);
	void resolveLocal(Expr::Expr* expr, Token name);
};

} /* namespace tjs */

#endif /* RESOLVER_H_ */
