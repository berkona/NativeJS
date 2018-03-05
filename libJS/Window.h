/*
 * Window.h
 *
 *  Created on: Feb 28, 2018
 *      Author: jon
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "JSObject.h"

namespace tjs {

class Window : public Scope {
public:
	Window();
	virtual ~Window();
};

class Console : public JSObject {
public:
	Console();
	virtual ~Console();
};

} /* namespace tjs */

#endif /* WINDOW_H_ */
