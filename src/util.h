/*
 * util.h
 *
 *  Created on: Feb 27, 2018
 *      Author: jon
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <sstream>
#include <iostream>

#include "Token.h"

namespace tjs {

std::string tempID();

bool hadError();
void hadError(bool error);

void error(int line, std::string where, std::string message);
void error(Token token, std::string message);
void warn(Token token, std::string message);

}

#endif /* UTIL_H_ */
