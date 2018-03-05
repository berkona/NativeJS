//============================================================================
// Name        : TurboJS.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

//#include <stdio.h>

#include "Lexer.h"
#include "Stmt.h"
#include "Parser.h"
#include "ASTPrinter.h"
#include "preprocessors/Preprocessor.h"
#include "CodeGenerator.h"
#include "Exceptions.h"
#include "util.h"

#include "../libJS/libJS.h"

using namespace std;
using namespace tjs;

enum OutputType {
	AST,
	SRC,
	BIN,
};

struct Config {
	bool printHelp = false;
	string cc = "clang++";
	string ccOpts;
	string output;
	OutputType outputType = BIN;
	vector<string> files;
};

Config parseArgs(int argc, char const ** argv) {
	Config cfg;
	bool hasOutputType = false;
	for (int i = 1; i < argc; i++) {
		string arg = string(argv[i]);
		if (arg.compare("-h") == 0 || arg.compare("--help") == 0) {
			cfg.printHelp = true;
		}
		else if (arg.compare("-o") == 0 || arg.compare("--out") == 0) {
			if (i == argc-1)
				throw std::runtime_error("--out requires an argument after it");
			cfg.cc = argv[++i];
		}
		else if (arg.compare("--cc") == 0) {
			if (i == argc-1)
				throw std::runtime_error("--cc requires an argument after it");
			cfg.cc = argv[++i];
		}
		else if (arg.compare("--cc-opts") == 0) {
			if (i == argc-1)
				throw std::runtime_error("--cc requires an argument after it");
			cfg.ccOpts = argv[++i];
		}
		else if (arg.compare("--ast") == 0) {
			if (hasOutputType)
				throw std::runtime_error("Can only define one of --ast, --src, or --bin");

			cfg.outputType = AST;
			hasOutputType = true;
		}
		else if (arg.compare("--src") == 0) {
			if (hasOutputType)
				throw std::runtime_error("Can only define one of --ast, --src, or --bin");

			cfg.outputType = SRC;
			hasOutputType = true;
		}
		else if (arg.compare("--bin") == 0) {
			if (hasOutputType)
				throw std::runtime_error("Can only define one of --ast, --src, or --bin");

			cfg.outputType = SRC;
			hasOutputType = true;
		}
		else if (arg.size() > 0 && arg[0] == '-') {
			throw std::runtime_error("Unrecognized option \"" + arg + "\"");
		}
		else {
			cfg.files.push_back(arg);
		}
	}

	// short-circuit the sanity checks if printHelp is true
	if (cfg.printHelp) return cfg;

	//sanity checks
	if (cfg.files.size() == 0)
		throw std::runtime_error("You must specify at least one file to compile");

	if (cfg.output == "") {
		cfg.output = "a.out";
	}

	return cfg;
}

std::string helpText = R"END(Usage: tjs [options] file...
Options:
  --help				Display this information
  --out <file>			Output to this file (defaults to stdout)
  --cc <command>		Change the C compiler used (defaults to clang++)
  --cc-opts	<opt_str>	Pass the following string to the C compiler
  --ast					Output the AST of input files only
  --src					Output the generated C source code only	
  --bin					Output an executable binary file
Aliases:
  -h					sc. --help
  -o <file>				sc. --out
)END";

void printHelp() {
	cout << helpText;
}

int main(int argc, char const **argv) {
	Config cfg;
	try {
		cfg = parseArgs(argc, argv);
	} catch (std::runtime_error &e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	if (cfg.printHelp) {
		printHelp();
		return EXIT_SUCCESS;
	}

	for (string fileName : cfg.files) {
		cout << "Processing file " << fileName << endl;

		ifstream ifs (fileName);
		if (!ifs.is_open()) {
			throw runtime_error("Could not find file \"" + fileName + "\"");
		}

		string source( ( istreambuf_iterator<char>(ifs) ), ( istreambuf_iterator<char>() ) );

		Lexer lexer (source);

		Parser p;
		auto mod =  p.parse(&lexer);
		if (p.hadError()) {
			throw runtime_error("Parsing file \"" + fileName + "\" failed");
		}

		Preprocessor pp;
		auto modData = pp.preprocess(*mod);

		if (hadError()) {
			return EXIT_FAILURE;
		}

		if (cfg.outputType == AST) {
			ASTPrinter printer (cout);
			modData->ast->accept(printer);
			return EXIT_SUCCESS;
		}

		ofstream srcOut;
		if (cfg.outputType == SRC) {
			srcOut.open(cfg.output, ios_base::out);
		} else {
			srcOut.open(cfg.output + "_temp.cpp", ios_base::out);
		}

		CodeGenerator cg;
		srcOut << cg.generate(*modData);
		if (cfg.outputType == SRC || hadError())
			return hadError() ? EXIT_FAILURE : EXIT_SUCCESS;

		stringstream cmd;
		cmd << cfg.cc;

		// standard opts
		cmd << ' ' << "-o " << cfg.output;
		cmd << ' ' << "-IlibJS";
		cmd << ' ' << "-std=c++11";

		// requested opts
		cmd << ' ' << cfg.ccOpts;
		cmd << ' ' << cfg.output + "_temp.cpp";

		// libJS source follows
		auto srcFiles = libJSSource();
		for (string srcFile : srcFiles) {
			cmd << ' ' << srcFile;
		}

		cout << "Running cmd '" << cmd.str() << "'" << endl;
		FILE* clang = popen(cmd.str().c_str(), "w");
		if (errno) perror("Could not open clang++");

		int err = pclose(clang);
		if (err) {
			perror("Could not close clang++ cmd");
		}
	}

	return EXIT_SUCCESS;
}
