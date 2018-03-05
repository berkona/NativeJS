/*
 * Exceptions.h
 *
 *  Created on: Feb 27, 2018
 *      Author: jon
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <sstream>
#include <string>

#include "Token.h"

namespace tjs {

class CompileError : std::exception {
public:
	std::string reason;
	int line;
	std::string lexeme;

	CompileError(const std::string reason) : reason(reason) {
		line = -1;
		lexeme = "";
	}

	CompileError(const std::string reason, Token token)
		: reason(reason) {
		line = token.line;
		lexeme = token.type == TokenType::EOF_TOKEN ? "end" : *token.lexeme;
	}

	virtual const char* what() const noexcept {
		if (line == -1) {
			return ("[Unknown location] " + reason).c_str();
		} else {
			std::ostringstream ss;
			ss << "[line " << line << " at \"" << lexeme << "\"] " << reason;
			return ss.str().c_str();
		}
	}

};

}


#endif /* EXCEPTIONS_H_ */
