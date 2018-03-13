/*
 * ObjectManager.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: jon
 */

#include "ObjectManager.h"

using namespace std;

namespace tjs {

ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager() {}

void ObjectManager::newScope(Scope* scope) {
	scopes.push(ScopeData {
		scope,
		vector<JSObject*> {},
	});
}

void ObjectManager::endScope() {
	for (JSObject* obj : scopes.top().allocated) {
		delete obj;
	}
}

void ObjectManager::bubbleUp(JSObject* obj) {
	auto itr = scopes.top().allocated.begin();
	auto end = scopes.top().allocated.end();
	for (; itr != end; ++itr) {
		if ((*itr) == obj) {
			scopes.top().allocated.erase(itr);
			// TODO
			return;
		}
	}

	throw new runtime_error("Object not allocated in current scope");
}

} /* namespace tjs */
