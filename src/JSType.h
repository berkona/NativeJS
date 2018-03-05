/*
 * LiteralType.h
 *
 *  Created on: Feb 24, 2018
 *      Author: jon
 */

#ifndef JSTYPE_H_
#define JSTYPE_H_


enum class JSType {
	// runtime defined
	//DYNAMIC,

	// lexically defined
	UNDEFINED,
	STRING,
	NUMBER,
	OBJECT,
	FUNCTION,
	BOOL,
	NIL,
};

#endif /* JSTYPE_H_ */
