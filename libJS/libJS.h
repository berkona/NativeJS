/*
 * libJS.h
 *
 *  Created on: Mar 4, 2018
 *      Author: jon
 */

#ifndef LIBJS_H_
#define LIBJS_H_

#include <string>
#include <array>

namespace tjs {

inline std::array<std::string, 3> libJSSource() {
	std::array<std::string, 3> all = { {
		"libJS/JSObject.cpp",
		"libJS/Ops.cpp",
		"libJS/Window.cpp"
	} };
	return all;
}

inline std::array<std::string, 4> libJSIncludes() {
	std::array<std::string, 4> all = { {
		"<iostream>",
		"\"JSObject.h\"",
		"\"Ops.h\"",
		"\"Window.h\"",
	} };
	return all;
}

}

#endif /* LIBJS_H_ */
