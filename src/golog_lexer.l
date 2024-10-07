%{
#include "golog_program.h"
#include "golog_parser.h"
#include <stdio.h>

%}

%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

LPAREN		"("
RPAREN		")"

TEST		\?
SEQUENCE    ";"
BRANCH		\|
ITER		\*
IF          "if|IF"
THEN        "then|THEN"
ELSE        "else|ELSE"
WHILE       "while|WHILE"
DO          "do|DO"
END         "end|END"

SYMBOL		[a-zA-Z_][a-zA-Z0-9_]*
WS			[ \r\n\t]*

%%

{WS}            { /* Skip blanks. */		}
{LPAREN}        { return TOKEN_LPAREN;		}
{RPAREN}        { return TOKEN_RPAREN;		}

{TEST}			{ return TOKEN_TEST;		}
{SEQUENCE}		{ return TOKEN_SEQUENCE;	}
{BRANCH}		{ return TOKEN_BRANCH;  	}
{ITER}			{ return TOKEN_ITER;		}
{IF}	    	{ return TOKEN_IF;	    	}
{THEN}			{ return TOKEN_THEN;		}
{ELSE}	        { return TOKEN_ELSE;	    }
{WHILE}		    { return TOKEN_WHILE;		}
{DO}		    { return TOKEN_DO;	        }
{END}			{ return TOKEN_END;			}

{SYMBOL}		{
					yylval->symbol = (Symbol*) malloc(sizeof(Symbol));
					yylval->symbol->name = strdup(yytext);
					yylval->symbol->sym_type = identify_symbol(yytext);

					if (yylval->symbol->sym_type == ACTION) { return TOKEN_ACTION; }
					if (yylval->symbol->sym_type == FLUENT) { return TOKEN_FLUENT; }
					if (yylval->symbol->sym_type == OBJECT) { return TOKEN_OBJECT; }
				}
.				{ 
					fprintf(stderr, "ERROR: Unrecognized symbol: %s\n", yytext); 
					exit(1); 
				}

%%