# pragma once

#include"location.hh"
#include"parser.tab.hh"
#include"parser_stype.h"
#include"scanner.hpp"

class GologScanner : public gologFlexLexer {

    private:

    public:
        Golog_YYSTYPE* yylval = nullptr;

        explicit GologScanner(std::istream* in): gologFlexLexer(in) {}
        virtual ~GologScanner(){};

        using FlexLexer::yylex;

        virtual int yylex(Golog_YYSTYPE* lval, yy::GologParser::location_type* loc);
        // YY_DECL defined in lexer.l
        // Method body created by flex in lexer.yy.c
};

// TODO. Can this cause problems? Is it necessary?
inline int gologFlexLexer::yylex() {
    return 0; 
}

inline int gologFlexLexer::yywrap() {
    return 1;
}