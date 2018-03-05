/*
 * Preprocessor.h
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include <stack>
#include <unordered_map>

#include "../Stmt.h"

#include "Aggregator.h"
#include "Resolver.h"

namespace tjs {

struct ModData {
	Stmt::Block* ast;
	std::unordered_map<Expr::Expr*, int>* scopes;
};

class Preprocessor {
public:
	Preprocessor();
	virtual ~Preprocessor();

	ModData* preprocess(Stmt::Block& mod);

private:
	Aggregator aggregatorPass;
	Resolver resolverPass;
};

} /* namespace tjs */

#endif /* PREPROCESSOR_H_ */
