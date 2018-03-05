/*
 * Preprocessor.cpp
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#include "Preprocessor.h"

#include <stdexcept>
#include <algorithm>

namespace tjs {

Preprocessor::Preprocessor() {}

Preprocessor::~Preprocessor() {}

ModData* Preprocessor::preprocess(Stmt::Block& mod) {
	auto scopes = resolverPass.process(mod);
	auto newMod = aggregatorPass.aggregate(mod);

	// TODO flattening
	// TODO concat files

	ModData* result = new ModData();
	result->ast = newMod;
	result->scopes = scopes;
	return result;
}

} /* namespace tjs */
