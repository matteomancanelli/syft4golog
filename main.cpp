#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/spirit/home/qi/detail/expectation_failure.hpp>
#include <boost/config/detail/suffix.hpp>

#include "parser/parser.h"
#include "model/formula.h"
#include "model/logger.h"
#include "model/reference.h"
#include "model/action.h"
#include "model/effect_axiom.h"
#include "model/fluent.h"
#include "model/procedural.h"
#include "model/platform/semantics.h"

int main() {
    try {
        // Example input program (could be loaded from a file)
        std::string gologProgram = R"(
            proc example {
                a; b; c
            }
        )";

        // Use parse_string from gologpp
        gologpp::parser::parse_string(gologProgram);

        // If you want to parse from a file, use parse_file:
        // gologpp::parser::parse_file("path_to_golog_file.golog");

        std::cout << "Parsed program successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}