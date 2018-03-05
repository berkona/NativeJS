/*
 * JSObject.h
 *
 *  Created on: Feb 26, 2018
 *      Author: jon
 */

#ifndef JSOBJECT_H_
#define JSOBJECT_H_

#include <unordered_map>
#include <vector>

namespace tjs {

class JSObject;

class Scope {
public:
	Scope(Scope* parent);
	virtual ~Scope();

	JSObject* get(std::string name);
	void set(std::string name, JSObject* value);

private:
	std::unordered_map<std::string, JSObject*> locals;
	Scope* parent;
};

class JSObject {
public:
	JSObject(JSObject* proto);
	virtual ~JSObject();

	JSObject* get(std::string name);
	void set(std::string name, JSObject* value);

	virtual bool toBool();
	virtual double toNum();
	virtual std::string toStr();

//	JSObject& operator+(JSObject& other);
//	JSObject& operator-(JSObject& other);
//
//	// relational operators
//	JSObject& operator<(JSObject& other);
//	JSObject& operator<=(JSObject& other);
//	JSObject& operator==(JSObject& other);
//	JSObject& operator!=(JSObject& other);
//	JSObject& operator>(JSObject& other);
//	JSObject& operator>=(JSObject& other);


private:
	std::unordered_map<std::string, JSObject*> props;
	JSObject* proto;

	int cmp(JSObject& other);
};

typedef JSObject* (*NativeFN)(Scope* scope);

class Function : public JSObject {
public:
	Function(NativeFN fn, Scope* closure, std::vector<std::string>* parameters);
	virtual ~Function();
	JSObject* call(int nArgs, JSObject** args);

private:
	NativeFN fn;
	Scope* closure;
	std::vector<std::string>* parameters;

	Scope* createScope(JSObject** args, int nArgs);

};

class Undef : public JSObject {
public:
	Undef(JSObject* proto) : JSObject(proto) {}
	virtual bool toBool() override;
	virtual double toNum() override;
	virtual std::string toStr() override;
};

class Null : public JSObject {
public:
	Null(JSObject* proto) : JSObject(proto) {}
	virtual bool toBool() override;
	virtual double toNum() override;
	virtual std::string toStr() override;
};

class Bool : public JSObject {
public:
	bool value;
	Bool(bool value, JSObject* proto) : JSObject(proto), value(value) {}
	virtual bool toBool() override;
	virtual double toNum() override;
	virtual std::string toStr() override;
};

class Num : public JSObject {
public:
	double value;
	Num(double value, JSObject* proto) : JSObject(proto), value(value) {}
	virtual bool toBool() override;
	virtual double toNum() override;
	virtual std::string toStr() override;
};

class Str : public JSObject {
public:
	std::string value;
	Str(std::string value, JSObject* proto) : JSObject(proto), value(value) {}
	virtual bool toBool() override;
	virtual double toNum() override;
	virtual std::string toStr() override;
};

#ifndef undefined
#define undefined new Undef(NULL)
#endif

#ifndef null
#define null new Null(NULL)
#endif

} /* namespace tjs */

#endif /* JSOBJECT_H_ */
