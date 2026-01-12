%option debug
%option yyclass="GologScanner"
%option noyywrap
%option c++

%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexGologScanner */
#include "scanner_internal.hpp"
#undef  YY_DECL
#define YY_DECL int GologScanner::yylex(Golog_YYSTYPE *lval, yy::GologParser::location_type *loc )

using token = yy::GologParser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END_OF_FILE )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%%

%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

"("                         { return token::LPAR; }
")"                         { return token::RPAR; }
"["                         { return token::LSPAR; }
"]"                         { return token::RSPAR; }

"true"                      { return token::TRUE_; }
"false"                     { return token::FALSE_; }
"!"                         { return token::NOT; }
"&&"                        { return token::AND; }
"||"                        { return token::OR; }
"->"                        { return token::IMPLICATION; }
"<->"                       { return token::EQUIVALENCE; }

"nil"                       { return token::NIL; }
"und"                       { return token::UND; }
"?"                         { return token::TEST; }
";"                         { return token::SEQUENCE; }
"|"                         { return token::CHOICE; }
"*"                         { return token::ITERATION; }

[a-z_][a-z0-9_]*|(\"[^\"\t\r\n]+\")|(\'[^\'\t\r\n]+\') {
                                yylval->symbol_name = yytext;
                                return token::SYMBOL;
                            }

\n                          {
                               // Update line number
                               loc->lines();
                               return token::NEWLINE;
                            }
[\t\r\n ]+                  { ; }

%%