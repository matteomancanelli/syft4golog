#include "golog_program.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

static std::set<Symbol*> set_of_symbols;

// Create a new symbol
Symbol* create_symbol(SymbolType type, const char* name) {
    Symbol* symbol = (Symbol*) malloc(sizeof(Symbol));
    if (symbol) {
        symbol->name = strdup(name);
        symbol->sym_type = type;
    }

    set_of_symbols.insert(symbol);
    return symbol;
}

// Identify the symbol type by its name
SymbolType identify_symbol(const char* name) {
    for (Symbol* symbol : set_of_symbols) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol->sym_type;
        }
    }
    return SYM_UNK;
}

// Destroy a symbol
void destroy_symbol(Symbol* symbol) {
    if (symbol) {
        free(symbol->name);
        free(symbol);
    }
}

// Create a variable node in the Golog program
golog_program* add_symbol(const Symbol* symbol) {
    golog_program* node = (golog_program*) malloc(sizeof(golog_program));
    if (node) {
        node->constr_type = SYMBOL;
        node->left = NULL;
        node->right = NULL;
        node->symbol = create_symbol(symbol->sym_type, symbol->name);
    }
    return node;
}

// Create an operation node in the Golog program
golog_program* create_program(ConstructType type, golog_program* left, golog_program* right) {
    golog_program* node = (golog_program*) malloc(sizeof(golog_program));
    if (node) {
        node->constr_type = type;
        node->left = left;
        node->right = right;
        node->symbol = NULL;
    }
    return node;
}

// Print the Golog program
void print_program(golog_program *root) {
    if (root == NULL) return;

    if (root->constr_type == SYMBOL) {
        printf("%s", root->symbol->name);
    } else {
        printf("(");
        print_program(root->left);

        switch (root->constr_type) {
            case TEST: printf("?"); break;
            case SEQUENCE: printf(";"); break;
            case BRANCH: printf("|"); break;
            case ITER: printf("*"); break;
            default: break;
        }
        
        if (root->right) {
            print_program(root->right);
        }
        printf(")");
    }
}

// Destroy the Golog program tree
void destroy_program(golog_program* root) {
    if (root == NULL) return;

    destroy_symbol(root->symbol);
    destroy_program(root->left);
    destroy_program(root->right);
    free(root);
}

/*
bool compare_symbols(Symbol* sym1, Symbol* sym2) {
    if (sym1 == NULL && sym2 == NULL) return true;
    if (sym1 == NULL || sym2 == NULL) return false;

    return (sym1->sym_type == sym2->sym_type) && (strcmp(sym1->name, sym2->name) == 0);
}

bool compare_programs(const golog_program* p1, const golog_program* p2) {
    if (p1 == NULL && p2 == NULL) return true;
    if (p1 == NULL || p2 == NULL) return false;

    if (p1->constr_type != p2->constr_type) return false;
    if (!compare_symbols(p1->symbol, p2->symbol)) return false;

    return compare_programs(p1->left, p2->left) && compare_programs(p1->right, p2->right);
}

bool golog_program_comparator::operator() (const golog_program* p1, const golog_program* p2) const {
    return compare_programs(p1, p2);
}
*/