#ifndef TRANS_H
#define	TRANS_H

#include "golog_program.h"
#include "golog_parser.h"
#include "golog_lexer.h"

#ifdef	__cplusplus
extern "C"
{
#endif

golog_program* getAST (const char* input);

#ifdef	__cplusplus
}
#endif

#endif	/* TRANS_H */

