#include "golog_program.h"
#include "trans.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>

// Function to initialize the symbol table with predefined symbols
void initialize_symbols() {
    // Adding actions
    create_symbol(ACTION, "A");
    create_symbol(ACTION, "B");
    create_symbol(FLUENT, "C");
}

bool compare_symbols(const Symbol* sym1, const Symbol* sym2) {
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

struct golog_program_comparator {
    bool operator() (const golog_program* p1, const golog_program* p2) const {
        return !compare_programs(p1, p2);
    }
};


std::set<golog_program*, golog_program_comparator> getSyntacticClosure(golog_program* root) {
    std::set<golog_program*, golog_program_comparator> syntactic_closure;
    
    Symbol* true_sym = (Symbol*) malloc(sizeof(Symbol));
    if (true_sym) {
        true_sym->name = strdup("True");
        true_sym->sym_type = FLUENT;
    }
    golog_program* nil = create_program(TEST, add_symbol(true_sym), NULL);
    
    syntactic_closure.insert(root);
    syntactic_closure.insert(nil);

    std::set<golog_program*, golog_program_comparator> sc_left_program;
    std::set<golog_program*, golog_program_comparator> sc_right_program;

    switch(root->constr_type) {
        case SEQUENCE:
            sc_left_program = getSyntacticClosure(root->left);
            sc_right_program = getSyntacticClosure(root->right);

            for (golog_program* prog : sc_left_program) {
                golog_program* seq_prog = create_program(SEQUENCE, prog, root->right);
                syntactic_closure.insert(seq_prog);
            }
            
            for (golog_program* prog : sc_right_program)
                syntactic_closure.insert(prog);
            break;
        case BRANCH:
            sc_left_program = getSyntacticClosure(root->left);
            sc_right_program = getSyntacticClosure(root->right);

            for (golog_program* prog : sc_left_program)
                syntactic_closure.insert(prog);
            for (golog_program* prog : sc_right_program)
                syntactic_closure.insert(prog);
            break;
        case ITER:
            golog_program* iter_prog = create_program(SEQUENCE, root->left, root);
            syntactic_closure.insert(iter_prog);
            break;
    }

    return syntactic_closure;
}

int main(int argc, char** argv) {
    const char* input_program = "A;B";

    // Initialize symbol table
    initialize_symbols();

    golog_program* program = getAST(input_program);
    std::cout << "Parsed Golog program: ";
    print_program(program);
    std::cout << std::endl;

    std::cout << "Syntactic Closure: ";
    std::cout << std::endl;
    std::set<golog_program*, golog_program_comparator> syntactic_closure = getSyntacticClosure(program);

    for (golog_program* prog : syntactic_closure) {
        print_program(prog);
        std::cout << std::endl;
    }
    
    for (golog_program* prog : syntactic_closure)
        destroy_program(prog);

    return 0;
}
