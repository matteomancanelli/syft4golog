#pragma once

// #define yyFlexLexer gologFlexLexer
// #include <FlexLexer.h>
// #undef yyFlexLexer

#undef yyFlexLexer
#define yyFlexLexer gologFlexLexer
#include <FlexLexer.h>

#include "scanner_internal.hpp"