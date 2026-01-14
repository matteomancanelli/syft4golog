#pragma once

#include"golog_program.hpp"

struct Golog_YYSTYPE {
    // Information that can be read by the parser
    golog_ptr program;
    formula_ptr formula;
    std::string symbol_name;
    // Constructor
    Golog_YYSTYPE() = default;
    // Destructor
    ~Golog_YYSTYPE() = default;
    // Copy constructor and assignment
    Golog_YYSTYPE(const Golog_YYSTYPE&) = default;
    Golog_YYSTYPE& operator=(const Golog_YYSTYPE&) = default;
    // Move constructor and assignment
    Golog_YYSTYPE(Golog_YYSTYPE&&) = default;
    Golog_YYSTYPE& operator=(Golog_YYSTYPE&&) = default;
};