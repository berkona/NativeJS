/*
 * JSObject.cpp
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <math.h>
#include <unordered_map>
#include <vector>

#include "JSObject.h"

namespace tjs {

Scope::Scope(Scope* parent) : parent(parent) {
//	std::cout << "[Scope] Created scope " << this << " with parent " << parent << std::endl;
}

Scope::~Scope() {
//	std::cout << "[~Scope] Deleting scope " << this << std::endl;
//	for (auto it = locals.begin(); it != locals.end(); ++it) {
//		std::cout << "[~Scope] Deleting object " << it->second << std::endl;
//		delete it->second;
//	}
}

JSObject* Scope::get(std::string name, int depth) {
	Scope* curr = this;
	for (int i = 0; i < depth; i++) {
		curr = curr->parent;
	}
	auto search = curr->locals.find(name);
	return search->second;
}

void Scope::set(std::string name, JSObject* value) {
	locals[name] = value;
}

JSObject::JSObject(JSObject* proto) : proto(proto) {
//	std::cout << "[JSObject] Created object " << this << std::endl;
}

JSObject::~JSObject() {
//	std::cout << "[~JSObject] Deleting object " << this << std::endl;
}

JSObject* JSObject::get(std::string name) {
	auto pair = props.find(name);
	if (pair != props.end()) {
		return pair->second;
	} else if (name == "prototype") {
		return proto;
	} else if (proto != NULL) {
		return proto->get(name);
	} else {
		return undefined;
	}
}

void JSObject::set(std::string name, JSObject* value) {
	if (name == "prototype") {
		proto = value;
	} else {
		props[name] = value;
	}
}

bool JSObject::toBool() {
	return true;
}

double JSObject::toNum() {
	throw std::runtime_error("[JSObject] number coercion for objects unsupported");
}

std::string JSObject::toStr() {
	throw std::runtime_error("[JSObject] string coercion for objects unsupported");
}

//JSObject& JSObject::operator+(JSObject& other) {
//	return *(new Num(toNum() + other.toNum(), NULL));
//}
//
//JSObject& JSObject::operator-(JSObject& other) {
//	return *(new Num(toNum() - other.toNum(), NULL));
//}
//
//JSObject& JSObject::operator<(JSObject& other) {
//	return *(new Bool(cmp(other) < 0, NULL));
//}
//
//JSObject& JSObject::operator<=(JSObject& other) {
//	return *(new Bool(cmp(other) <= 0, NULL));
//}
//
//JSObject& JSObject::operator>(JSObject& other) {
//	return *(new Bool(cmp(other) > 0, NULL));
//}
//
//JSObject& JSObject::operator>=(JSObject& other) {
//	return *(new Bool(cmp(other) >= 0, NULL));
//}
//
//JSObject& JSObject::operator==(JSObject& other) {
//	return *(new Bool(cmp(other) == 0, NULL));
//}
//
//JSObject& JSObject::operator!=(JSObject& other) {
//	return *(new Bool(cmp(other) != 0, NULL));
//}
//
//int JSObject::cmp(JSObject& other) {
//	return toNum() - other.toNum();
//}

Function::Function(NativeFN fn, Scope* closure, std::vector<std::string>* parameters)
	: JSObject(NULL), fn(fn), closure(closure) , parameters(parameters) {
//	std::cout << "[Function::Function] Created function" << std::endl;
}

Function::~Function() {
//	std::cout << "[Function::~Function] Deleted function" << std::endl;
}

Scope* Function::createScope(JSObject** args, int nArgs) {
	Scope* newScope = new Scope(closure);
	int i;
	for (i = 0; i < nArgs; i++) {
		newScope->set((*parameters)[i], args[i]);
	}
	// be true to JS behavior and allow for less args than parameters
	for ( ; i < parameters->size(); i++) {
		newScope->set((*parameters)[i], undefined);
	}
	return newScope;
}

JSObject* Function::call(int nArgs, JSObject** args) {
//	std::cout << "[Fuction::call] Creating scope before call" << std::endl;
	Scope* s = createScope(args, nArgs);
	JSObject* ret = fn(s);
//	std::cout << "[Fuction::call] Deleting scope after call" << std::endl;
	delete s;
	return ret;
}

bool Undef::toBool() { return false; }

double Undef::toNum() { return NAN; }

std::string Undef::toStr() { return "undefined"; }

bool Null::toBool() { return false; }

double Null::toNum() { return 0; }

std::string Null::toStr() { return "null"; }

bool Bool::toBool() { return value; }

double Bool::toNum() { return value ? 1 : 0; }

std::string Bool::toStr() { return value ? "true" : "false"; }

bool Num::toBool() { return !(isnan(value) || value == 0); }

double Num::toNum() { return value; }

std::string Num::toStr() {
	std::stringstream ss {};
	ss << value;
	return ss.str();
}

bool Str::toBool() { return value.size() > 0; }

double Str::toNum() { return std::stod(value); }

std::string Str::toStr() { return value; }

} /* namespace tjs */
