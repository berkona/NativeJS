/*
 * GenerateAST.cpp
 *
 *  Created on: Feb 23, 2018
 *      Author: jon
 */

#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>

using namespace std;

void defineClass(ofstream& fs, string baseType, string type, string* fields, int nFields) {
	fs << "class " << type << " : public " << baseType << " {" << endl;
	fs << endl;
	fs << "public:" << endl;
	for (int i = 0; i < nFields; i++) {
		fs << "\t" << fields[i] << ";" << endl;
	}
	fs << endl;
	fs << "\t" << type << "(";
	for (int i = 0; i < nFields; i++) {
		// convert ptr fields to pass by reference
		string param = fields[i];
		fs << param;
		if (i < nFields -1) {
			fs << ", ";
		}
	}
	fs << ") : ";
	for (int i = 0; i < nFields; i++) {
		string fieldDef = fields[i];
		string::size_type pos = fieldDef.find(" ");
		string fieldName = fieldDef.substr(pos+1);
		fs << fieldName << "(" << fieldName << ")";
		if (i < nFields - 1) {
			fs << ", ";
		}
	}
	fs << " {}" << endl;

	fs << '\t' << "virtual ~" << type << "() {}" << endl;

	fs << endl;
//	fs << '\t' << "template <typename R>" << endl;
	fs << "\t" << "void accept(Visitor& v) { v.visit(*this); };" << endl;
	fs << endl;

	fs << "};" << endl;
	fs << endl;
}


string trim(const string& str) {
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first) return str;
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

class TypeDef {
public:
	string name;
	vector<string> params;

	TypeDef(string name, vector<string> params) : name(name), params(params) {}
};

void defineVistor(ofstream& fs, vector<TypeDef>& typeDefs) {
	// define vistor
//	fs << "template <typename R>" << endl;
	fs << "class Visitor {" << endl;
	fs << endl;
	fs << "public:" << endl;
	fs << '\t' << "virtual ~Visitor() {}" << endl;
	int length = typeDefs.size();
	for (int i = 0; i < length; i++) {
		TypeDef t = typeDefs[i];
		fs << '\t' << "virtual void visit(" << t.name << "& v) = 0;" << endl;
	}

	fs << "};" << endl;
	fs << endl;
}

void defineSource(
		string prefix,
		string baseType,
		string* subclasses,
		int length,
		string includes
	) {

	// first get a list of class names and a list of params for them
	vector<TypeDef> typeDefs {};
	for (int i = 0; i < length; i++) {
		string classDef = subclasses[i];
		string::size_type split_pos = classDef.find(":");
		string className = trim(classDef.substr(0, split_pos));
		string param_list = trim(classDef.substr(split_pos + 1));
		vector<string> params {};
		string::size_type comma_pos;
		do {
			comma_pos = param_list.find(",");
			string this_param = trim(param_list.substr(0, comma_pos));
			params.push_back(this_param);
			param_list = param_list.substr(comma_pos +1);
		} while (comma_pos != string::npos);

		TypeDef t (className, params);
		typeDefs.push_back(t);
	}

	ofstream fs ((prefix + baseType + ".h").c_str());
	fs << "/*" << endl;
	fs << " * THIS IS AN AUTO-GENERATED FILE -- DO NOT MODIFY " << endl;
	fs << " */" << endl;
	fs << endl;
	fs << "#ifndef AUTOGEN_" << baseType << "_H" << endl;
	fs << "#define AUTOGEN_" << baseType << "_H" << endl;
	fs << endl;

	fs << includes << endl;
	fs << endl;

	fs << "namespace tjs {" << endl;
	fs << endl;
	fs << "namespace " << baseType << " {" << endl;
	fs << endl;

	// forward declare the Visitor
//	fs << "template <typename R>" << endl;
	fs << "class Visitor;" << endl;
	fs << endl;

	// define base class
	fs << "class " << baseType << " {" << endl;
	fs << "public:" << endl;
	fs << '\t' << "virtual ~" << baseType << "() {}" << endl;
	fs << endl;
//	fs << '\t' << "template <typename R>" << endl;
	fs << "\t" << "virtual void accept(Visitor& v) = 0;" << endl;
	fs << "};" << endl;
	// end define base class
	fs << endl;

	// forward declare subclasses
	for (int i = 0; i < length; i++) {
		fs << "class " << typeDefs[i].name << ";" << endl;
	}
	fs << endl;

	// now fully define the Visitor
	defineVistor(fs, typeDefs);

	// define subclasses
	for (int i = 0; i < length; i++) {
		TypeDef t = typeDefs[i];
		defineClass(fs, baseType, t.name, t.params.data(), t.params.size());
	}

	fs << "}" << endl; // end namespace <baseType>
	fs << "}" << endl; // end namespace tjs
	fs << endl;
	fs << "#endif" << endl;
}

int main(int argc, const char** argv) {
	array<string, 9> exprs = { {
		"Assign   : Token* name, Expr* value",
		"Binary   : Expr* left, Token* op, Expr* right",
		"Grouping : Expr* expression",
		"Call     : Expr* callee, Token* paren, std::vector<Expr*>* arguments",
		"Get      : Expr* object, Token* name",
		"Literal  : JSType type, void* value",
		"Unary    : Token* op, Expr* right",
		"Set      : Expr* object, Token* name, Expr* value",
		//"Logical  : Expr* left, Token* op, Expr* right",
		"Variable : Token* name"
	} };

	defineSource("src/", "Expr", exprs.data(), exprs.size(), "#include <vector>\n\n#include \"Token.h\"\n#include \"JSType.h\"");

	array<string, 8> statements { {
		"Block      : std::vector<Stmt*>* statements",
		"Expression : Expr::Expr* expression",
		"Function   : Token* name, std::vector<Token*>* parameters, std::vector<Stmt*>* body",
		"If         : Expr::Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
		"Var        : Token* name, Expr::Expr* initializer",
		"Return     : Token* keyword, Expr::Expr* value",
		"While      : Expr::Expr* condition, Stmt* body",
		"FunDecl	: Token* name, std::vector<std::string> paramList",
	} };

	defineSource("src/", "Stmt", statements.data(), statements.size(), "#include <vector>\n\n#include \"Expr.h\"\n#include \"Token.h\"");
}
