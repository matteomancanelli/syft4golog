#include "trans.h"
#include <stdlib.h>

golog_program* getAST (const char *input) {
    golog_program* program;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
    
    if (yylex_init (&scanner)) {
      return NULL;
    }

    state = yy_scan_string (input, scanner);
    if (yyparse (&program, scanner)) {
        return NULL;
    }

    yy_delete_buffer (state, scanner);
    yylex_destroy (scanner);

    return program;
}
