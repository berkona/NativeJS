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
	JSObject* alloc();

private:
	std::stack<ScopeData> scopes;
	std::vector<JSObject*> freeList;
};

} /* namespace tjs */

#endif /* OBJECTMANAGER_H_ */
