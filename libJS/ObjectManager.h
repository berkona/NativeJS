/*
 * ObjectManager.h
 *
 *  Created on: Mar 4, 2018
 *      Author: jon
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include <stack>

#include "JSObject.h"

namespace tjs {

struct ScopeData {
	Scope* scope;
	std::vector<JSObject*> allocated;
};

class ObjectManager {
public:
	ObjectManager();
	virtual ~ObjectManager();

	void newScope(Scope* scope);
	void endScope();

	void bubbleUp(JSObject* obj);

	JSObject* allocObj();
	Function* allocFn();
	Bool* allocBool();
	Num* allocNum();
	Str* allocStr();

private:
	std::stack<ScopeData> scopes;
	std::vector<JSObject*> freeObj;
	std::vector<Function*> freeFn;
	std::vector<Bool*> freeBool;
	std::vector<Num*> freeNum;
	std::vector<Str*> freeStr;
};

} /* namespace tjs */

#endif /* OBJECTMANAGER_H_ */
