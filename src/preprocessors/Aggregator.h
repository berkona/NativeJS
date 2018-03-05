/*
 * Aggregator.h
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#ifndef AGGREGATOR_H_
#define AGGREGATOR_H_

#include "../Stmt.h"

namespace tjs {

class Aggregator : public Stmt::Visitor {
public:
	Aggregator() {}
	virtual ~Aggregator() {}

	Stmt::Block* aggregate(Stmt::Block& mod);

	void visit(Stmt::Block& v) override;
	void visit(Stmt::Expression& v) override;
	void visit(Stmt::Function& v) override;
	void visit(Stmt::If& v) override;
	void visit(Stmt::Var& v) override;
	void visit(Stmt::Return& v) override;
	void visit(Stmt::While& v) override;
	void visit(Stmt::FunDecl& v) override;

private:
	int currentLevel = 0;
	std::vector<Stmt::Stmt*> globalStatements;
	Stmt::Block* module = NULL;
};

} /* namespace tjs */

#endif /* AGGREGATOR_H_ */
