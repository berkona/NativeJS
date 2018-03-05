/*
 * Window.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: jon
 */

#include <iostream>

#include "Window.h"

#include "JSObject.h"

namespace tjs {

Window::Window() : Scope(NULL) {
	set("console", new Console());
}

Window::~Window() {
	// TODO Auto-generated destructor stub
}

static JSObject* log(Scope* scope) {
	std::cout << scope->get("message")->toStr() << std::endl;
	return NULL;
}

Console::Console() : JSObject(NULL) {
	set("log", new Function(&log, NULL, new std::vector<std::string> { "message" }));
}

Console::~Console() {

}

} /* namespace tjs */
