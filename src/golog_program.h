#ifndef golog_program_H
#define	golog_program_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C"
{
#endif

// SymbolType Enum
typedef enum _SymbolType {
    ACTION,
    FLUENT,
    OBJECT,
    SYM_UNK
} SymbolType;

// Symbol Struct
typedef struct _Symbol {
    SymbolType sym_type;
    char* name;
} Symbol;

// ConstructType Enum
typedef enum _ConstructType {
    TEST,
    SEQUENCE,
    BRANCH,
    ITER,
    SYMBOL,
    CONSTR_UNK
} ConstructType;

// Golog Program Struct
typedef struct _golog_program {
    ConstructType constr_type;
    struct _golog_program* left;
    struct _golog_program* right;

    Symbol* symbol;
} golog_program;


// Symbol Management
Symbol* create_symbol(SymbolType type, const char* name);
SymbolType identify_symbol(const char* name);
void destroy_symbol(Symbol* symbol);

// Program Management
golog_program* add_symbol(const Symbol* symbol);
golog_program* create_program(ConstructType type, golog_program* left, golog_program* right);
void print_program(golog_program* root);
void destroy_program(golog_program* root);

// Comparison functions for golog_program
//bool compare_symbols(const Symbol* sym1, const Symbol* sym2);
//bool compare_programs(const golog_program* p1, const golog_program* p2);

// Custom comparator for std::set
//struct golog_program_comparator {
//    bool operator() (const golog_program* p1, const golog_program* p2) const;
//};

#ifdef	__cplusplus
}
#endif

#endif	/* golog_program_H */