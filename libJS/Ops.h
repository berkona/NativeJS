/*
 * Ops.h
 *
 *  Created on: Mar 2, 2018
 *      Author: jon
 */

#ifndef OPS_H_
#define OPS_H_

#include "JSObject.h"

namespace tjs {

double add(double a, double b);
double add(JSObject& a, JSObject& b);
double add(JSObject& a, double b);
double add(double b, JSObject& a);

double sub(double a, double b);
double sub(JSObject& a, JSObject& b);
double sub(JSObject& a, double b);
double sub(double a, JSObject b);

bool lt(double a, double b);
bool lt(JSObject& a, JSObject& b);
bool lt(JSObject& a, double b);
bool lt(double a, JSObject& b);

}

#endif /* OPS_H_ */
