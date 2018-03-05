/*
 * Ops.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: jon
 */

#include "JSObject.h"

namespace tjs  {

double add(double a, double b) {
	return a + b;
}

double add(JSObject a, JSObject b) {
	return a.toNum() + b.toNum();
}

double add(JSObject a, double b) {
	return a.toNum() + b;
}

double add(double b, JSObject a) {
	return add(a, b);
}

double sub(double a, double b) {
	return a - b;
}

double sub(JSObject a, JSObject b) {
	return a.toNum() - b.toNum();
}

double sub(JSObject a, double b) {
	return a.toNum() - b;
}

double sub(double a, JSObject b) {
	return a - b.toNum();
}

bool lt(double a, double b) {
	return a < b;
}

bool lt(JSObject a, JSObject b) {
	return a.toNum() < b.toNum();
}

bool lt(JSObject a, double b) {
	return a.toNum() < b;
}

bool lt(double a, JSObject b) {
	return a < b.toNum();
}

}
