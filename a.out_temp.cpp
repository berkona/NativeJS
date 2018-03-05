
#include <iostream>
#include "JSObject.h"
#include "Ops.h"
#include "Window.h"


using namespace std;
using namespace tjs;

JSObject* fib(Scope* scope) {
if (lt((scope->get("n")), (2))) {
return scope->get("n");
}
JSObject* tempVar_0 = scope->get("fib");
JSObject* tempVar_1[] = { new Num((sub((scope->get("n")), (1))), NULL), };
Function* tempVar_2 = dynamic_cast<Function*>(tempVar_0);
if (tempVar_2 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_3 = tempVar_2->call(1, tempVar_1);
JSObject* tempVar_4 = scope->get("fib");
JSObject* tempVar_5[] = { new Num((sub((scope->get("n")), (2))), NULL), };
Function* tempVar_6 = dynamic_cast<Function*>(tempVar_4);
if (tempVar_6 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_7 = tempVar_6->call(1, tempVar_5);
return new Num((add((tempVar_3), (tempVar_7))), NULL);
return NULL;
}
JSObject* __main__(Scope* scope) {
scope->set("nCalls", new Num((0), NULL));
scope->set("fib",  new Function(&fib, scope, new std::vector<std::string> {"n", } ));
scope->set("n", new Num((10), NULL));
JSObject* tempVar_8 = scope->get("fib");
JSObject* tempVar_9 = scope->get("n");
JSObject* tempVar_10[] = { tempVar_9, };
Function* tempVar_11 = dynamic_cast<Function*>(tempVar_8);
if (tempVar_11 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_12 = tempVar_11->call(1, tempVar_10);
scope->set("f_n", tempVar_12);
JSObject* tempVar_13 = (scope->get("console")->get("log"));
JSObject* tempVar_14 = new Str(("fib(10)"), NULL);
JSObject* tempVar_15[] = { tempVar_14, };
Function* tempVar_16 = dynamic_cast<Function*>(tempVar_13);
if (tempVar_16 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_17 = tempVar_16->call(1, tempVar_15);
JSObject* tempVar_18 = (scope->get("console")->get("log"));
JSObject* tempVar_19 = scope->get("f_n");
JSObject* tempVar_20[] = { tempVar_19, };
Function* tempVar_21 = dynamic_cast<Function*>(tempVar_18);
if (tempVar_21 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_22 = tempVar_21->call(1, tempVar_20);
JSObject* tempVar_23 = (scope->get("console")->get("log"));
JSObject* tempVar_24 = new Str(("nCalls"), NULL);
JSObject* tempVar_25[] = { tempVar_24, };
Function* tempVar_26 = dynamic_cast<Function*>(tempVar_23);
if (tempVar_26 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_27 = tempVar_26->call(1, tempVar_25);
JSObject* tempVar_28 = (scope->get("console")->get("log"));
JSObject* tempVar_29 = scope->get("nCalls");
JSObject* tempVar_30[] = { tempVar_29, };
Function* tempVar_31 = dynamic_cast<Function*>(tempVar_28);
if (tempVar_31 == NULL) throw runtime_error("Expression evaluated to an uncallable object");
JSObject* tempVar_32 = tempVar_31->call(1, tempVar_30);
return NULL;
}


int main(int argc, const char** argv) {
	try {
		//cout << "calling __main__ manually" << endl;
		Scope* window = new Window();
		//cout << "created window at " << window << endl;
		__main__(window);
		//cout << "deleting window at " << window << endl;
		delete window;
		//cout << "done calling __main__" << endl;
	} catch (std::exception& err) {
		cerr << "Unexpected exception encountered: " << err.what() << endl;
		return 1;
	}
	return 0;
}

