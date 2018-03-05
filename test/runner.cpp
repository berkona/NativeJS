/*
 * runner.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: jon
 */

#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "LexerTest.h"
#include "ParserTest.h"

bool runAllTests(int argc, char const *argv[]) {
    cute::xml_file_opener xmlfile(argc, argv);
    cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
    auto runner = cute::makeRunner(lis, argc, argv);

    cute::suite lexerSuite = make_suite_LexerTest();
    cute::suite parserSuite = make_suite_ParserTest();

    bool success = true;
    success = runner(lexerSuite, "Lexer") && success;
    success = runner(parserSuite, "Parser") && success;
    return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
