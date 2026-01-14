%skeleton "lalr1.cc" /* -*- C++ -*- */
%require  "3.0"
%debug 
%defines
%define api.parser.class {GologParser}

%code requires {
    #include "golog_program.hpp"
    #include "parser_stype.h"
    class GologDriver;
    class GologScanner;

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif
}

%parse-param { GologScanner  &scanner  }
%parse-param { GologDriver  &d  }

%code {
    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "scanner.hpp"
    #include "golog_driver.hpp"

    #undef yylex
    #define yylex scanner.yylex
}

%define parse.assert

%define api.value.type {Golog_YYSTYPE}

%type<program> input golog_program
%type<formula> propositional_formula
%type<symbol_name> SYMBOL

%token              LPAR
%token              RPAR
%token              LSPAR
%token              RSPAR
%token              TRUE_
%token              FALSE_
%token              SYMBOL
%token              NEWLINE
%token              END_OF_FILE    0

%left               EQUIVALENCE
%right              IMPLICATION
%left               OR
%left               AND
%right              NOT

%token              NIL
%token              UND
%left               TEST
%left               SEQUENCE
%left               CHOICE
%left               ITERATION

%nonassoc           LPAR
%nonassoc           LSPAR

%locations

%start input

%%

input : golog_program           { $$ = $1; d.result = $$; };

propositional_formula : 
    propositional_formula EQUIVALENCE propositional_formula         { $$ = d.add_PropositionalLogicEquivalence($1, $3); }
    | propositional_formula IMPLICATION propositional_formula       { $$ = d.add_PropositionalLogicImplication($1, $3); }
    | propositional_formula OR propositional_formula                { $$ = d.add_PropositionalLogicDisjunction($1, $3); }
    | propositional_formula AND propositional_formula               { $$ = d.add_PropositionalLogicConjunction($1, $3); }
    | NOT propositional_formula                                     { $$ = d.add_PropositionalLogicNegation($2); }
    | TRUE_                                                         { $$ = d.add_PropositionalLogicTrue(); }
    | FALSE_                                                        { $$ = d.add_PropositionalLogicFalse(); }
    | SYMBOL                                                        { $$ = d.add_PropositionalLogicAtom($1); }
    ;

propositional_formula : LSPAR propositional_formula RSPAR             { $$ = $2; };

golog_program : golog_program SEQUENCE golog_program                { $$ = d.add_GologProgramSequence($1, $3); }
                | golog_program CHOICE golog_program                { $$ = d.add_GologProgramChoice($1, $3); }
                | golog_program ITERATION                           { $$ = d.add_GologProgramIteration($1); }
                | propositional_formula TEST                        { $$ = d.add_GologProgramTest($1); }
                | NIL                                               { $$ = d.add_GologProgramNil(); }
                | UND                                               { $$ = d.add_GologProgramUnd(); }
                | SYMBOL                                            { $$ = d.add_GologProgramAction($1); }

golog_program : LPAR golog_program RPAR                             { $$ = $2; };

%%

void yy::GologParser::error(const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}