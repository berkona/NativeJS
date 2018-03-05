/*
 * util.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: jon
 */

#include "util.h"

#include <string>
#include <sstream>
#include <iostream>

#include "Token.h"

namespace tjs {

static int nextTempId = 0;

std::string tempID() {
	std::stringstream ss;
	ss << "tempVar_" << nextTempId;
	nextTempId++;
	return ss.str();
}

static bool errorFlag = false;

bool hadError() {
	return errorFlag;
}

void hadError(bool error) {
	errorFlag = error;
}

void report(int line, std::string where, std::string level, std::string message) {
	std::cerr << level << ": [line " << line << " at \"" << where << "\"] " << message << std::endl;
}

void error(Token token, std::string message) {
	hadError(true);
	if (token.type == TokenType::EOF_TOKEN) {
		report(token.line, "end", "ERROR", message);
	} else {
		report(token.line, *token.lexeme, "ERROR", message);
	}
}

void warn(Token token, std::string message) {
	if (token.type == TokenType::EOF_TOKEN) {
		report(token.line, "end", "WARNING", message);
	} else {
		report(token.line, *token.lexeme, "WARNING", message);
	}
}

}


