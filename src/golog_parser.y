%{
#include "golog_program.h"
#include "golog_parser.h"
#include "golog_lexer.h"

int yyerror(golog_program** program, yyscan_t scanner, const char* msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(1);
}
%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
}
 
%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { golog_program** program }
%parse-param { yyscan_t scanner }

%union {
	Symbol* symbol;
	golog_program* program;
}

%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_TEST
%token TOKEN_SEQUENCE
%token TOKEN_BRANCH
%token TOKEN_ITER
%token TOKEN_IF
%token TOKEN_THEN
%token TOKEN_ELSE
%token TOKEN_WHILE
%token TOKEN_DO
%token TOKEN_PROC
%token TOKEN_END
%token <symbol> TOKEN_ACTION
%token <symbol> TOKEN_FLUENT
%token <symbol> TOKEN_OBJECT

%type <program> statement
%type <program> condition

%%

input
	: statement { *program = $1; }
	;

condition
	: TOKEN_FLUENT                                { $$ = add_symbol($1); }
	| TOKEN_LPAREN condition TOKEN_RPAREN         { $$ = $2; }
    // Extend condition handling here with FOL logic as needed
	;

statement
	: TOKEN_ACTION                                            			{ $$ = add_symbol($1);	                        		}
	| TOKEN_LPAREN statement TOKEN_RPAREN                        		{ $$ = $2;	                                    		}
	| condition TOKEN_TEST		                                		{ $$ = create_program( TEST, add_symbol($1), NULL );    }
	| statement TOKEN_SEQUENCE statement	                            { $$ = create_program( SEQUENCE, $1, $3 );				}
	| statement TOKEN_BRANCH statement	                            	{ $$ = create_program( BRANCH, $1, $3 );				}
	| statement TOKEN_ITER     		                            		{ $$ = create_program( ITER, $1, NULL );				}
	| TOKEN_IF condition TOKEN_THEN statement TOKEN_ELSE statement TOKEN_END	{ $$ = create_program( BRANCH, create_program( SEQUENCE, create_program( TEST, add_symbol($2), NULL ), $4 ), create_program( SEQUENCE, create_program( TEST, add_symbol($2), NULL ), $6 ) );	}
	| TOKEN_WHILE condition TOKEN_DO statement TOKEN_END              	{ $$ = create_program( SEQUENCE, create_program( ITER, create_program( SEQUENCE, create_program( TEST, add_symbol($2), NULL), $4 ) , NULL ), create_program( TEST, add_symbol($2), NULL ) );    		}
	;

%%
